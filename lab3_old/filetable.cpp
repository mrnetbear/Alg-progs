#include "filetable.hpp"

#include <iostream>
#include <vector>
#include <string>

FileBackedTable::FileBackedTable(const std::string& filename, int msize1, int msize2)
    : Table(msize1, msize2), filename(filename) {
    file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    
    if (!file) {
        // Файл не существует, создаем новый
        file.open(filename, std::ios::binary | std::ios::out);
        file.close();
        file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
        
        // Инициализируем пустую таблицу
        FileHeader header;
        header.msize1 = msize1;
        header.msize2 = msize2;
        header.csize1 = 0;
        header.csize2 = 0;
        
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&header), sizeof(FileHeader));
        
        // Инициализируем KeySpace1
        FileKeySpace1 emptyKs1;
        memset(emptyKs1.key, 0, MAX_STRING_LENGTH);
        memset(emptyKs1.par, 0, MAX_STRING_LENGTH);
        emptyKs1.info_offset = -1;
        
        for (int i = 0; i < msize1; i++) {
            file.write(reinterpret_cast<char*>(&emptyKs1), sizeof(FileKeySpace1));
        }
        
        // Инициализируем KeySpace2
        FileKeySpace2 emptyKs2;
        emptyKs2.busy = false;
        emptyKs2.key = 0;
        emptyKs2.node_offset = -1;
        
        for (int i = 0; i < msize2; i++) {
            file.write(reinterpret_cast<char*>(&emptyKs2), sizeof(FileKeySpace2));
        }
        
        file.flush();
    } else {
        loadFromFile();
    }
}

FileBackedTable::~FileBackedTable() {
    saveToFile();
    file.close();
}

void FileBackedTable::loadFromFile() {
    file.seekg(0);
    
    // Читаем заголовок
    FileHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(FileHeader));
    
    msize1 = header.msize1;
    msize2 = header.msize2;
    csize1 = header.csize1;
    csize2 = header.csize2;
    
    // Читаем KeySpace1
    for (int i = 0; i < msize1; i++) {
        FileKeySpace1 fileKs1;
        file.read(reinterpret_cast<char*>(&fileKs1), sizeof(FileKeySpace1));
        
        if (strlen(fileKs1.key) > 0) {
            ks1[i].setValues(fileKs1.key, fileKs1.par, nullptr);
            
            // Загружаем цепочку элементов
            Item* prev = nullptr;
            long current_offset = fileKs1.info_offset;
            
            while (current_offset != -1) {
                FileItem fileItem;
                file.seekg(current_offset);
                file.read(reinterpret_cast<char*>(&fileItem), sizeof(FileItem));
                
                InfoType* info = readInfo(fileItem.info_offset);
                Item* item = new Item(info, fileItem.key1, fileItem.key2, fileItem.ind1, fileItem.ind2);
                item->setRelease(fileItem.release);
                
                if (prev) {
                    prev->setNext(item);
                } else {
                    ks1[i].setInfo(item);
                }
                
                prev = item;
                current_offset = fileItem.next_offset;
            }
        }
    }
    
    // Читаем KeySpace2
    for (int i = 0; i < msize2; i++) {
        FileKeySpace2 fileKs2;
        file.read(reinterpret_cast<char*>(&fileKs2), sizeof(FileKeySpace2));
        
        if (fileKs2.busy) {
            ks2[i].setParameters(true, fileKs2.key, nullptr);
            
            // Загружаем цепочку версий
            Node2* prev = nullptr;
            long current_offset = fileKs2.node_offset;
            
            while (current_offset != -1) {
                FileNode2 fileNode;
                file.seekg(current_offset);
                file.read(reinterpret_cast<char*>(&fileNode), sizeof(FileNode2));
                
                InfoType* info = readInfo(fileNode.info_offset);
                Node2* node = new Node2(info);
                node->setRelease(fileNode.release);
                
                if (prev) {
                    prev->setNext(node);
                } else {
                    ks2[i].setParameters(true, fileKs2.key, node);
                }
                
                prev = node;
                current_offset = fileNode.next_offset;
            }
        }
    }
}

void FileBackedTable::saveToFile() {
    file.seekp(0);
    
    // Сохраняем заголовок
    FileHeader header;
    header.msize1 = msize1;
    header.msize2 = msize2;
    header.csize1 = csize1;
    header.csize2 = csize2;
    
    file.write(reinterpret_cast<char*>(&header), sizeof(FileHeader));
    
    // Сохраняем KeySpace1
    for (int i = 0; i < msize1; i++) {
        FileKeySpace1 fileKs1;
        memset(fileKs1.key, 0, MAX_STRING_LENGTH);
        memset(fileKs1.par, 0, MAX_STRING_LENGTH);
        fileKs1.info_offset = -1;
        
        if (ks1[i].getKey() != "") {
            strncpy(fileKs1.key, ks1[i].getKey().c_str(), MAX_STRING_LENGTH);
            strncpy(fileKs1.par, ks1[i].getPar().c_str(), MAX_STRING_LENGTH);
            
            // Сохраняем цепочку элементов
            Item* current = ks1[i].getInfo();
            long prev_offset = -1;
            
            while (current) {
                FileItem fileItem;
                strncpy(fileItem.key1, current->getKey1().c_str(), MAX_STRING_LENGTH);
                fileItem.key2 = current->getKey2();
                fileItem.ind1 = current->getInd1();
                fileItem.ind2 = current->getInd2();
                fileItem.release = current->getRelease();
                fileItem.next_offset = -1;
                
                // Сохраняем информацию
                fileItem.info_offset = writeNewInfo(current->getInfo());
                
                // Записываем элемент в файл
                long current_offset = file.tellp();
                file.write(reinterpret_cast<char*>(&fileItem), sizeof(FileItem));
                
                if (prev_offset == -1) {
                    fileKs1.info_offset = current_offset;
                } else {
                    // Обновляем ссылку на следующий элемент у предыдущего
                    file.seekp(prev_offset + offsetof(FileItem, next_offset));
                    file.write(reinterpret_cast<char*>(&current_offset), sizeof(long));
                    file.seekp(current_offset);
                }
                
                prev_offset = current_offset;
                current = current->getNext();
            }
        }
        
        file.write(reinterpret_cast<char*>(&fileKs1), sizeof(FileKeySpace1));
    }
    
    // Сохраняем KeySpace2
    for (int i = 0; i < msize2; i++) {
        FileKeySpace2 fileKs2;
        fileKs2.busy = ks2[i].isBusy();
        fileKs2.key = ks2[i].getKey();
        fileKs2.node_offset = -1;
        
        if (fileKs2.busy) {
            // Сохраняем цепочку версий
            Node2* current = ks2[i].getNode();
            long prev_offset = -1;
            
            while (current) {
                FileNode2 fileNode;
                fileNode.release = current->getRelease();
                fileNode.next_offset = -1;
                
                // Сохраняем информацию
                fileNode.info_offset = writeNewInfo(current->getInfo());
                
                // Записываем узел в файл
                long current_offset = file.tellp();
                file.write(reinterpret_cast<char*>(&fileNode), sizeof(FileNode2));
                
                if (prev_offset == -1) {
                    fileKs2.node_offset = current_offset;
                } else {
                    // Обновляем ссылку на следующий узел у предыдущего
                    file.seekp(prev_offset + offsetof(FileNode2, next_offset));
                    file.write(reinterpret_cast<char*>(&current_offset), sizeof(long));
                    file.seekp(current_offset);
                }
                
                prev_offset = current_offset;
                current = current->getNext();
            }
        }
        
        file.write(reinterpret_cast<char*>(&fileKs2), sizeof(FileKeySpace2));
    }
    
    file.flush();
}

int FileBackedTable::addElement(const std::string& key1, const std::string& par, UINT key2, InfoType* info) {
    int result = Table::addElement(key1, par, key2, info);
    if (result == 0) {
        saveToFile();  // Сохраняем изменения в файл
    }
    return result;
}

Item* FileBackedTable::searchByCompositeKey(const std::string& key1, UINT key2) const {
    // В этом простом примере мы используем реализацию из базового класса
    // В реальной реализации нужно было бы читать данные из файла
    return Table::searchByCompositeKey(key1, key2);
}

int FileBackedTable::deleteByCompositeKey(const std::string& key1, UINT key2) {
    int result = Table::deleteByCompositeKey(key1, key2);
    if (result == 0) {
        saveToFile();  // Сохраняем изменения в файл
    }
    return result;
}

// Вспомогательные методы для работы с файлом
void FileBackedTable::writeInfo(long offset, const InfoType* info) {
    FileInfo fileInfo;
    fileInfo.str1 = info->getStr1();
    fileInfo.str2 = info->getStr2();
    
    file.seekp(offset);
    file.write(reinterpret_cast<char*>(&fileInfo), sizeof(FileInfo));
}

InfoType* FileBackedTable::readInfo(long offset) {
    if (offset == -1) return nullptr;
    
    FileInfo fileInfo;
    file.seekg(offset);
    file.read(reinterpret_cast<char*>(&fileInfo), sizeof(FileInfo));
    
    return new InfoType(fileInfo.str1, fileInfo.str2);
}

long FileBackedTable::writeNewInfo(const InfoType* info) {
    file.seekp(0, std::ios::end);
    long offset = file.tellp();
    writeInfo(offset, info);
    return offset;
}