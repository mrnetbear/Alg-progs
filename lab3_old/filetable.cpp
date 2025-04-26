#include "filetable.hpp"
#include <iostream>
#include <algorithm>
#include <cstring>

// Конструктор с инициализацией файла
FileBackedTable::FileBackedTable(const std::string& filename, int msize1, int msize2) 
    : filename(filename), msize1(msize1), msize2(msize2), csize1(0), csize2(0) {
    
    // Открываем файл в режиме чтения/записи
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    
    if (!file) {
        // Если файл не существует, создаем новый
        file.open(filename, std::ios::binary | std::ios::out);
        file.close();
        file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
        
        // Инициализация заголовка
        FileHeader header;
        header.msize1 = msize1;
        header.msize2 = msize2;
        header.csize1 = 0;
        header.csize2 = 0;
        
        file.seekp(0);
        file.write(reinterpret_cast<const char*>(&header), sizeof(FileHeader));
        
        // Инициализация KeySpace1
        FileKeySpace1 emptyKs1;
        memset(emptyKs1.key, 0, MAX_STRING_LENGTH);
        memset(emptyKs1.par, 0, MAX_STRING_LENGTH);
        emptyKs1.first_item_offset = -1;
        
        for (int i = 0; i < msize1; i++) {
            file.write(reinterpret_cast<const char*>(&emptyKs1), sizeof(FileKeySpace1));
        }
        
        // Инициализация KeySpace2
        FileKeySpace2 emptyKs2;
        emptyKs2.busy = false;
        emptyKs2.key = 0;
        emptyKs2.first_node_offset = -1;
        
        for (int i = 0; i < msize2; i++) {
            file.write(reinterpret_cast<const char*>(&emptyKs2), sizeof(FileKeySpace2));
        }
        
        file.flush();
    } else {
        // Чтение заголовка существующего файла
        FileHeader header;
        file.seekg(0);
        file.read(reinterpret_cast<char*>(&header), sizeof(FileHeader));
        
        msize1 = header.msize1;
        msize2 = header.msize2;
        csize1 = header.csize1;
        csize2 = header.csize2;
    }
}

// Деструктор - сохраняет данные и закрывает файл
FileBackedTable::~FileBackedTable() {
    updateHeader();
    file.close();
}

// Вспомогательные методы

// Выделение места в файле
long FileBackedTable::allocateSpace(size_t size) {
    file.seekp(0, std::ios::end);
    return file.tellp();
}

// Обновление заголовка файла
void FileBackedTable::updateHeader() {
    FileHeader header;
    header.msize1 = msize1;
    header.msize2 = msize2;
    header.csize1 = csize1;
    header.csize2 = csize2;
    
    file.seekp(0);
    file.write(reinterpret_cast<const char*>(&header), sizeof(FileHeader));
    file.flush();
}

// Поиск пустой позиции в KeySpace1
long FileBackedTable::findEmptyKs1Position() const {
    for (long i = 0; i < msize1; i++) {
        FileKeySpace1 ks1;
        file.seekg(sizeof(FileHeader) + i * sizeof(FileKeySpace1));
        file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));
        
        if (strlen(ks1.key) == 0) {
            return sizeof(FileHeader) + i * sizeof(FileKeySpace1);
        }
    }
    return -1;
}

// Поиск позиции по ключу в KeySpace1
long FileBackedTable::findKs1Position(const std::string& key) const {
    for (long i = 0; i < msize1; i++) {
        FileKeySpace1 ks1;
        file.seekg(sizeof(FileHeader) + i * sizeof(FileKeySpace1));
        file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));
        
        if (strcmp(ks1.key, key.c_str()) == 0) {
            return sizeof(FileHeader) + i * sizeof(FileKeySpace1);
        }
    }
    return -1;
}

// Поиск позиции для вставки в KeySpace2
long FileBackedTable::findKs2InsertPosition(unsigned int key) const {
    int h1 = key % msize2;
    int h2 = 1 + (key % (msize2 - 1));
    
    for (int i = 0; i < msize2; i++) {
        int index = (h1 + i * h2) % msize2;
        long pos = sizeof(FileHeader) + msize1 * sizeof(FileKeySpace1) + index * sizeof(FileKeySpace2);
        
        FileKeySpace2 ks2;
        file.seekg(pos);
        file.read(reinterpret_cast<char*>(&ks2), sizeof(FileKeySpace2));
        
        if (!ks2.busy || ks2.key == key) {
            return pos;
        }
    }
    return -1;
}

// Поиск позиции по ключу в KeySpace2
long FileBackedTable::findKs2Position(unsigned int key) const {
    int h1 = key % msize2;
    int h2 = 1 + (key % (msize2 - 1));
    
    for (int i = 0; i < msize2; i++) {
        int index = (h1 + i * h2) % msize2;
        long pos = sizeof(FileHeader) + msize1 * sizeof(FileKeySpace1) + index * sizeof(FileKeySpace2);
        
        FileKeySpace2 ks2;
        file.seekg(pos);
        file.read(reinterpret_cast<char*>(&ks2), sizeof(FileKeySpace2));
        
        if (ks2.busy && ks2.key == key) {
            return pos;
        }
    }
    return -1;
}

// Обновление KeySpace1 в файле
void FileBackedTable::updateKs1At(long offset, const FileKeySpace1& ks1) {
    file.seekp(offset);
    file.write(reinterpret_cast<const char*>(&ks1), sizeof(FileKeySpace1));
    file.flush();
}

// Обновление KeySpace2 в файле
void FileBackedTable::updateKs2At(long offset, const FileKeySpace2& ks2) {
    file.seekp(offset);
    file.write(reinterpret_cast<const char*>(&ks2), sizeof(FileKeySpace2));
    file.flush();
}

// Запись элемента в файл
void FileBackedTable::writeItemAt(long offset, const FileItem& item) {
    file.seekp(offset);
    file.write(reinterpret_cast<const char*>(&item), sizeof(FileItem));
    file.flush();
}

// Запись узла в файл
void FileBackedTable::writeNodeAt(long offset, const FileNode2& node) {
    file.seekp(offset);
    file.write(reinterpret_cast<const char*>(&node), sizeof(FileNode2));
    file.flush();
}

// Запись информации в файл
long FileBackedTable::writeInfo(const std::string& str1, const std::string& str2) {
    FileInfo info;
    strncpy(info.str1, str1.c_str(), MAX_STRING_LENGTH);
    strncpy(info.str2, str2.c_str(), MAX_STRING_LENGTH);
    
    long offset = allocateSpace(sizeof(FileInfo));
    file.seekp(offset);
    file.write(reinterpret_cast<const char*>(&info), sizeof(FileInfo));
    file.flush();
    return offset;
}

// Чтение информации из файла
void FileBackedTable::readInfo(long offset, std::string& str1, std::string& str2) const {
    if (offset == -1) return;
    
    FileInfo info;
    file.seekg(offset);
    file.read(reinterpret_cast<char*>(&info), sizeof(FileInfo));
    
    str1 = info.str1;
    str2 = info.str2;
}

// Основные операции

// Добавление элемента
int FileBackedTable::addElement(const std::string& key1, const std::string& par, 
                               unsigned int key2, const std::string& infoStr1, 
                               const std::string& infoStr2) {
    // Проверка уникальности ключа
    std::string tmp1, tmp2;
    if (searchByCompositeKey(key1, key2, tmp1, tmp2)) {
        return -1; // Элемент уже существует
    }
    
    // Проверка существования родителя
    if (!par.empty() && findKs1Position(par) == -1) {
        return -2; // Родитель не найден
    }
    
    // Запись информации
    long info_offset = writeInfo(infoStr1, infoStr2);
    
    // Создание записи элемента
    FileItem newItem;
    strncpy(newItem.key1, key1.c_str(), MAX_STRING_LENGTH);
    newItem.key2 = key2;
    newItem.info_offset = info_offset;
    newItem.release = 1;
    newItem.next_offset = -1;
    newItem.ind1 = -1;
    newItem.ind2 = -1;
    
    // Добавление в KeySpace1
    long ks1_pos = findKs1Position(key1);
    if (ks1_pos == -1) {
        // Новая запись в KeySpace1
        ks1_pos = findEmptyKs1Position();
        if (ks1_pos == -1) return -3; // Нет места
        
        FileKeySpace1 ks1;
        strncpy(ks1.key, key1.c_str(), MAX_STRING_LENGTH);
        strncpy(ks1.par, par.c_str(), MAX_STRING_LENGTH);
        ks1.first_item_offset = allocateSpace(sizeof(FileItem));
        
        newItem.ind1 = (ks1_pos - sizeof(FileHeader)) / sizeof(FileKeySpace1);
        writeItemAt(ks1.first_item_offset, newItem);
        updateKs1At(ks1_pos, ks1);
        csize1++;
    } else {
        // Добавление в существующую цепочку
        FileKeySpace1 ks1;
        file.seekg(ks1_pos);
        file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));
        
        long last_item_offset = ks1.first_item_offset;
        FileItem item;
        while (true) {
            file.seekg(last_item_offset);
            file.read(reinterpret_cast<char*>(&item), sizeof(FileItem));
            if (item.next_offset == -1) break;
            last_item_offset = item.next_offset;
        }
        
        long new_item_offset = allocateSpace(sizeof(FileItem));
        newItem.ind1 = (ks1_pos - sizeof(FileHeader)) / sizeof(FileKeySpace1);
        item.next_offset = new_item_offset;
        writeItemAt(last_item_offset, item);
        writeItemAt(new_item_offset, newItem);
    }
    
    // Добавление в KeySpace2
    long ks2_pos = findKs2InsertPosition(key2);
    if (ks2_pos == -1) return -4; // Нет места
    
    FileKeySpace2 ks2;
    file.seekg(ks2_pos);
    file.read(reinterpret_cast<char*>(&ks2), sizeof(FileKeySpace2));
    
    FileNode2 newNode;
    newNode.info_offset = info_offset;
    newNode.release = 1;
    newNode.next_offset = -1;
    
    if (!ks2.busy) {
        // Новая запись в KeySpace2
        ks2.busy = true;
        ks2.key = key2;
        ks2.first_node_offset = allocateSpace(sizeof(FileNode2));
        
        newItem.ind2 = (ks2_pos - sizeof(FileHeader) - msize1 * sizeof(FileKeySpace1)) / sizeof(FileKeySpace2);
        writeNodeAt(ks2.first_node_offset, newNode);
        updateKs2At(ks2_pos, ks2);
        csize2++;
    } else {
        // Добавление новой версии
        long last_node_offset = ks2.first_node_offset;
        FileNode2 node;
        int release = 1;
        while (true) {
            file.seekg(last_node_offset);
            file.read(reinterpret_cast<char*>(&node), sizeof(FileNode2));
            if (node.next_offset == -1) break;
            last_node_offset = node.next_offset;
            release++;
        }
        
        long new_node_offset = allocateSpace(sizeof(FileNode2));
        newNode.release = release + 1;
        node.next_offset = new_node_offset;
        
        newItem.ind2 = (ks2_pos - sizeof(FileHeader) - msize1 * sizeof(FileKeySpace1)) / sizeof(FileKeySpace2);
        writeNodeAt(last_node_offset, node);
        writeNodeAt(new_node_offset, newNode);
    }
    
    // Обновляем индексы в Item
    FileKeySpace1 ks1;
    file.seekg(ks1_pos);
    file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));
    
    long item_offset = ks1.first_item_offset;
    while (item_offset != -1) {
        FileItem item;
        file.seekg(item_offset);
        file.read(reinterpret_cast<char*>(&item), sizeof(FileItem));
        
        if (item.key2 == key2) {
            item.ind1 = (ks1_pos - sizeof(FileHeader)) / sizeof(FileKeySpace1);
            item.ind2 = (ks2_pos - sizeof(FileHeader) - msize1 * sizeof(FileKeySpace1)) / sizeof(FileKeySpace2);
            writeItemAt(item_offset, item);
            break;
        }
        item_offset = item.next_offset;
    }
    
    updateHeader();
    return 0;
}

// Поиск по составному ключу
bool FileBackedTable::searchByCompositeKey(const std::string& key1, unsigned int key2,
                                         std::string& outStr1, std::string& outStr2) const {
    long ks1_pos = findKs1Position(key1);
    if (ks1_pos == -1) return false;
    
    FileKeySpace1 ks1;
    file.seekg(ks1_pos);
    file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));
    
    long item_offset = ks1.first_item_offset;
    while (item_offset != -1) {
        FileItem item;
        file.seekg(item_offset);
        file.read(reinterpret_cast<char*>(&item), sizeof(FileItem));
        
        if (item.key2 == key2) {
            readInfo(item.info_offset, outStr1, outStr2);
            return true;
        }
        
        item_offset = item.next_offset;
    }
    
    return false;
}

// Удаление по составному ключу
int FileBackedTable::deleteByCompositeKey(const std::string& key1, unsigned int key2) {
    // Поиск в KeySpace1
    long ks1_pos = findKs1Position(key1);
    if (ks1_pos == -1) return -1;

    FileKeySpace1 ks1;
    file.seekg(ks1_pos);
    file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));

    long prev_item_offset = -1;
    long current_item_offset = ks1.first_item_offset;
    bool found = false;
    FileItem item_to_delete;

    // Поиск элемента в цепочке
    while (current_item_offset != -1 && !found) {
        file.seekg(current_item_offset);
        file.read(reinterpret_cast<char*>(&item_to_delete), sizeof(FileItem));

        if (item_to_delete.key2 == key2) {
            found = true;
            
            // Удаление из KeySpace1
            if (prev_item_offset == -1) {
                // Первый элемент в цепочке
                ks1.first_item_offset = item_to_delete.next_offset;
                updateKs1At(ks1_pos, ks1);
                
                if (ks1.first_item_offset == -1) {
                    // Цепочка пуста - очищаем KeySpace1
                    memset(ks1.key, 0, MAX_STRING_LENGTH);
                    memset(ks1.par, 0, MAX_STRING_LENGTH);
                    updateKs1At(ks1_pos, ks1);
                    csize1--;
                }
            } else {
                // Связываем предыдущий с следующим
                FileItem prev_item;
                file.seekg(prev_item_offset);
                file.read(reinterpret_cast<char*>(&prev_item), sizeof(FileItem));
                prev_item.next_offset = item_to_delete.next_offset;
                writeItemAt(prev_item_offset, prev_item);
            }
        } else {
            prev_item_offset = current_item_offset;
            current_item_offset = item_to_delete.next_offset;
        }
    }

    if (!found) return -1;

    // Удаление из KeySpace2
    if (item_to_delete.ind2 != -1) {
        long ks2_pos = sizeof(FileHeader) + msize1 * sizeof(FileKeySpace1) + 
                       item_to_delete.ind2 * sizeof(FileKeySpace2);
        
        FileKeySpace2 ks2;
        file.seekg(ks2_pos);
        file.read(reinterpret_cast<char*>(&ks2), sizeof(FileKeySpace2));

        long prev_node_offset = -1;
        long current_node_offset = ks2.first_node_offset;
        bool node_found = false;

        while (current_node_offset != -1 && !node_found) {
            FileNode2 current_node;
            file.seekg(current_node_offset);
            file.read(reinterpret_cast<char*>(&current_node), sizeof(FileNode2));

            if (current_node.info_offset == item_to_delete.info_offset) {
                node_found = true;
                
                if (prev_node_offset == -1) {
                    // Первый узел в цепочке
                    ks2.first_node_offset = current_node.next_offset;
                    updateKs2At(ks2_pos, ks2);
                    
                    if (ks2.first_node_offset == -1) {
                        // Цепочка пуста - очищаем KeySpace2
                        ks2.busy = false;
                        ks2.key = 0;
                        updateKs2At(ks2_pos, ks2);
                        csize2--;
                    }
                } else {
                    // Связываем предыдущий с следующим
                    FileNode2 prev_node;
                    file.seekg(prev_node_offset);
                    file.read(reinterpret_cast<char*>(&prev_node), sizeof(FileNode2));
                    prev_node.next_offset = current_node.next_offset;
                    writeNodeAt(prev_node_offset, prev_node);
                }
            }
            
            prev_node_offset = current_node_offset;
            current_node_offset = current_node.next_offset;
        }
    }

    updateHeader();
    return 0;
}

// Поиск по ключу KeySpace1
std::vector<std::pair<std::string, std::string>> FileBackedTable::searchByKey1(const std::string& key) const {
    std::vector<std::pair<std::string, std::string>> result;
    
    long ks1_pos = findKs1Position(key);
    if (ks1_pos == -1) return result;

    FileKeySpace1 ks1;
    file.seekg(ks1_pos);
    file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));

    long item_offset = ks1.first_item_offset;
    while (item_offset != -1) {
        FileItem item;
        file.seekg(item_offset);
        file.read(reinterpret_cast<char*>(&item), sizeof(FileItem));

        std::string str1, str2;
        readInfo(item.info_offset, str1, str2);
        result.emplace_back(str1, str2);

        item_offset = item.next_offset;
    }

    return result;
}

// Поиск по ключу KeySpace2
std::vector<std::pair<std::string, std::string>> FileBackedTable::searchByKey2(unsigned int key) const {
    std::vector<std::pair<std::string, std::string>> result;
    
    long ks2_pos = findKs2Position(key);
    if (ks2_pos == -1) return result;

    FileKeySpace2 ks2;
    file.seekg(ks2_pos);
    file.read(reinterpret_cast<char*>(&ks2), sizeof(FileKeySpace2));

    long node_offset = ks2.first_node_offset;
    while (node_offset != -1) {
        FileNode2 node;
        file.seekg(node_offset);
        file.read(reinterpret_cast<char*>(&node), sizeof(FileNode2));

        std::string str1, str2;
        readInfo(node.info_offset, str1, str2);
        result.emplace_back(str1, str2);

        node_offset = node.next_offset;
    }

    return result;
}

// Поиск по родительскому ключу
std::vector<std::pair<std::string, std::string>> FileBackedTable::searchByParentKey(const std::string& parent) const {
    std::vector<std::pair<std::string, std::string>> result;
    
    for (long i = 0; i < msize1; i++) {
        FileKeySpace1 ks1;
        file.seekg(sizeof(FileHeader) + i * sizeof(FileKeySpace1));
        file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));

        if (strcmp(ks1.par, parent.c_str()) == 0) {
            long item_offset = ks1.first_item_offset;
            while (item_offset != -1) {
                FileItem item;
                file.seekg(item_offset);
                file.read(reinterpret_cast<char*>(&item), sizeof(FileItem));

                std::string str1, str2;
                readInfo(item.info_offset, str1, str2);
                result.emplace_back(str1, str2);

                item_offset = item.next_offset;
            }
        }
    }

    return result;
}

// Удаление по ключу KeySpace1
int FileBackedTable::deleteByKey1(const std::string& key) {
    long ks1_pos = findKs1Position(key);
    if (ks1_pos == -1) return -1;

    FileKeySpace1 ks1;
    file.seekg(ks1_pos);
    file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));

    // Удаляем все связанные элементы из KeySpace2
    long item_offset = ks1.first_item_offset;
    while (item_offset != -1) {
        FileItem item;
        file.seekg(item_offset);
        file.read(reinterpret_cast<char*>(&item), sizeof(FileItem));

        deleteByCompositeKey(key, item.key2);
        item_offset = item.next_offset;
    }

    // Очищаем KeySpace1
    memset(ks1.key, 0, MAX_STRING_LENGTH);
    memset(ks1.par, 0, MAX_STRING_LENGTH);
    ks1.first_item_offset = -1;
    updateKs1At(ks1_pos, ks1);
    csize1--;

    // Обновляем родительские ссылки
    for (long i = 0; i < msize1; i++) {
        FileKeySpace1 current_ks1;
        file.seekg(sizeof(FileHeader) + i * sizeof(FileKeySpace1));
        file.read(reinterpret_cast<char*>(&current_ks1), sizeof(FileKeySpace1));

        if (strcmp(current_ks1.par, key.c_str()) == 0) {
            memset(current_ks1.par, 0, MAX_STRING_LENGTH);
            updateKs1At(sizeof(FileHeader) + i * sizeof(FileKeySpace1), current_ks1);
        }
    }

    updateHeader();
    return 0;
}

// Удаление по ключу KeySpace2
int FileBackedTable::deleteByKey2(unsigned int key) {
    long ks2_pos = findKs2Position(key);
    if (ks2_pos == -1) return -1;

    FileKeySpace2 ks2;
    file.seekg(ks2_pos);
    file.read(reinterpret_cast<char*>(&ks2), sizeof(FileKeySpace2));

    // Удаляем все связанные элементы из KeySpace1
    long node_offset = ks2.first_node_offset;
    while (node_offset != -1) {
        FileNode2 node;
        file.seekg(node_offset);
        file.read(reinterpret_cast<char*>(&node), sizeof(FileNode2));

        // Находим соответствующий элемент в KeySpace1
        for (long i = 0; i < msize1; i++) {
            FileKeySpace1 ks1;
            file.seekg(sizeof(FileHeader) + i * sizeof(FileKeySpace1));
            file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));

            if (ks1.first_item_offset != -1) {
                long item_offset = ks1.first_item_offset;
                while (item_offset != -1) {
                    FileItem item;
                    file.seekg(item_offset);
                    file.read(reinterpret_cast<char*>(&item), sizeof(FileItem));

                    if (item.info_offset == node.info_offset) {
                        deleteByCompositeKey(ks1.key, key);
                        break;
                    }
                    item_offset = item.next_offset;
                }
            }
        }
        node_offset = node.next_offset;
    }

    // Очищаем KeySpace2
    ks2.busy = false;
    ks2.key = 0;
    ks2.first_node_offset = -1;
    updateKs2At(ks2_pos, ks2);
    csize2--;

    updateHeader();
    return 0;
}

// Вывод содержимого таблицы
void FileBackedTable::printTable() const {
    std::cout << "=== KeySpace1 ===" << std::endl;
    for (long i = 0; i < msize1; i++) {
        FileKeySpace1 ks1;
        file.seekg(sizeof(FileHeader) + i * sizeof(FileKeySpace1));
        file.read(reinterpret_cast<char*>(&ks1), sizeof(FileKeySpace1));

        if (strlen(ks1.key) > 0) {
            std::cout << "Key: " << ks1.key << ", Parent: " << ks1.par << std::endl;
            
            long item_offset = ks1.first_item_offset;
            while (item_offset != -1) {
                FileItem item;
                file.seekg(item_offset);
                file.read(reinterpret_cast<char*>(&item), sizeof(FileItem));

                std::string str1, str2;
                readInfo(item.info_offset, str1, str2);
                std::cout << "  Item: KS2 key=" << item.key2 
                          << ", Info: [" << str1 << ", " << str2 << "]" << std::endl;

                item_offset = item.next_offset;
            }
        }
    }

    std::cout << "\n=== KeySpace2 ===" << std::endl;
    for (long i = 0; i < msize2; i++) {
        FileKeySpace2 ks2;
        file.seekg(sizeof(FileHeader) + msize1 * sizeof(FileKeySpace1) + i * sizeof(FileKeySpace2));
        file.read(reinterpret_cast<char*>(&ks2), sizeof(FileKeySpace2));

        if (ks2.busy) {
            std::cout << "Key: " << ks2.key << std::endl;
            
            long node_offset = ks2.first_node_offset;
            while (node_offset != -1) {
                FileNode2 node;
                file.seekg(node_offset);
                file.read(reinterpret_cast<char*>(&node), sizeof(FileNode2));

                std::string str1, str2;
                readInfo(node.info_offset, str1, str2);
                std::cout << "  Version " << node.release 
                          << ": [" << str1 << ", " << str2 << "]" << std::endl;

                node_offset = node.next_offset;
            }
        }
    }
}