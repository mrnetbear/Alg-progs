#include "table.hpp"
#include <iostream>
#include <algorithm>

InfoType::InfoType(const char* s1, const char* s2) {
    str1 = new char[strlen(s1) + 1];
    strcpy(str1, s1);
    str2 = new char[strlen(s2) + 1];
    strcpy(str2, s2);
}

InfoType::~InfoType() {
    delete[] str1;
    delete[] str2;
}

// Реализация Item
Item::Item(InfoType* info, const std::string& key1, UINT key2, int ind1, int ind2) 
    : info(info), key1(key1), key2(key2), ind1(ind1), ind2(ind2), next(nullptr), release(1) {}

Item::~Item() {
    delete info;
    delete next;
}

// Реализация KeySpace1
KeySpace1::KeySpace1() : key(""), par(""), info(nullptr) {}

KeySpace1::~KeySpace1() {
    delete info;
}

Node2::Node2(InfoType* info, Node2* next) : info(info), next(next) {
    release = 1;
}

Node2::~Node2() {
    delete info;
    delete next;
}

// Реализация KeySpace2
KeySpace2::KeySpace2() : busy(false), key(0), node(nullptr) {}

KeySpace2::~KeySpace2() {
    delete node;
}

// Реализация Table
Table::Table(int msize1, int msize2) : msize1(msize1), msize2(msize2), csize1(0), csize2(0) {
    ks1 = new KeySpace1[msize1];
    ks2 = new KeySpace2[msize2];
}

Table::~Table() {
    delete[] ks1;
    delete[] ks2;
}

int Table::hash1(unsigned int key) const {
    return key % msize2;
}

int Table::hash2(unsigned int key) const {
    return 1 + (key % (msize2 - 1));
}

int Table::findInsertPosition(unsigned int key) const {
    int h1 = hash1(key);
    int h2 = hash2(key);
    
    for (int i = 0; i < msize2; i++) {
        int index = (h1 + i * h2) % msize2;
        if (!ks2[index].busy) {
            return index;
        }
    }
    
    return -1;
}

int Table::findKeyPosition(unsigned int key) const {
    int h1 = hash1(key);
    int h2 = hash2(key);
    
    for (int i = 0; i < msize2; i++) {
        int index = (h1 + i * h2) % msize2;
        if (ks2[index].busy && ks2[index].key == key) {
            return index;
        }
        if (!ks2[index].busy) {
            return -1;
        }
    }
    
    return -1;
}

int Table::addElement(const std::string& key1, const std::string& par, unsigned int key2, InfoType* info) {
    // Проверка уникальности составного ключа
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].key == key1) {
            Item* current = ks1[i].info;
            while (current != nullptr) {
                if (current->key2 == key2) {
                    std::cout << "Error: Element with this composite key already exists" << std::endl;
                    return -1;
                }
                current = current->next;
            }
        }
    }
    
    // Проверка родительского ключа
    if (par != "") {
        bool parentFound = false;
        for (int i = 0; i < msize1; i++) {
            if (ks1[i].key == par) {
                parentFound = true;
                break;
            }
        }
        if (!parentFound) {
            std::cout << "Error: Parent key not found" << std::endl;
            return -1;
        }
    }
    
    // Создание нового элемента
    Item* newItem = new Item(info, key1, key2, -1, -1);
    
    // Добавление в первое пространство ключей
    int ks1Index = -1;
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].key == "") {
            ks1Index = i;
            break;
        }
    }
    
    if (ks1Index == -1) {
        std::cout << "Error: No space in KeySpace1" << std::endl;
        delete newItem;
        return -1;
    }
    
    ks1[ks1Index].key = key1;
    ks1[ks1Index].par = par;
    ks1[ks1Index].info = newItem;
    csize1++;
    newItem->ind1 = ks1Index;
    
    // Добавление во второе пространство ключей
    int ks2Index = findInsertPosition(key2);
    if (ks2Index == -1) {
        std::cout << "Error: No space in KeySpace2" << std::endl;
        // Откат изменений в первом пространстве
        ks1[ks1Index].key = "";
        ks1[ks1Index].par = "";
        ks1[ks1Index].info = nullptr;
        csize1--;
        delete newItem;
        return -1;
    }
    
    Node2* newNode = new Node2(info);
    newItem->ind2 = ks2Index;
    
    if (ks2[ks2Index].busy) {
        // Добавление новой версии
        Node2* current = ks2[ks2Index].node;
        int release = 1;
        while (current->next != nullptr) {
            current = current->next;
            release++;
        }
        current->next = newNode;
        newNode->release = release + 1;
    } else {
        // Первый элемент с этим ключом
        ks2[ks2Index].busy = true;
        ks2[ks2Index].key = key2;
        ks2[ks2Index].node = newNode;
        csize2++;
    }
    
    return 0;
}

Item* Table::searchByCompositeKey(const std::string& key1, unsigned int key2) const {
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].key == key1) {
            Item* current = ks1[i].info;
            while (current != nullptr) {
                if (current->key2 == key2) {
                    return current;
                }
                current = current->next;
            }
        }
    }
    return nullptr;
}

int Table::deleteByCompositeKey(const std::string& key1, unsigned int key2) {
    Item* itemToDelete = searchByCompositeKey(key1, key2);
    if (itemToDelete == nullptr) {
        std::cout << "Error: Element not found" << std::endl;
        return -1;
    }
    
    // Удаление из первого пространства ключей
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].key == key1) {
            if (ks1[i].info == itemToDelete) {
                ks1[i].info = itemToDelete->next;
                if (ks1[i].info == nullptr) {
                    ks1[i].key = "";
                    ks1[i].par = "";
                    csize1--;
                }
            } else {
                Item* prev = ks1[i].info;
                while (prev->next != itemToDelete) {
                    prev = prev->next;
                }
                prev->next = itemToDelete->next;
            }
            break;
        }
    }
    
    // Удаление из второго пространства ключей
    int ks2Index = findKeyPosition(key2);
    if (ks2Index != -1) {
        Node2* nodeToDelete = nullptr;
        if (ks2[ks2Index].node->info == itemToDelete->info) {
            nodeToDelete = ks2[ks2Index].node;
            ks2[ks2Index].node = nodeToDelete->next;
            if (ks2[ks2Index].node == nullptr) {
                ks2[ks2Index].busy = false;
                ks2[ks2Index].key = 0;
                csize2--;
            }
        } else {
            Node2* prev = ks2[ks2Index].node;
            while (prev->next != nullptr && prev->next->info != itemToDelete->info) {
                prev = prev->next;
            }
            if (prev->next != nullptr) {
                nodeToDelete = prev->next;
                prev->next = nodeToDelete->next;
            }
        }
        
        if (nodeToDelete != nullptr) {
            nodeToDelete->next = nullptr; // Чтобы не удалить цепочку
            delete nodeToDelete;
        }
    }
    
    itemToDelete->next = nullptr; // Чтобы не удалить цепочку
    delete itemToDelete;
    
    return 0;
}

std::vector<Item*> Table::searchByKey1(const std::string& key) const {
    std::vector<Item*> result;
    
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].key == key) {
            Item* current = ks1[i].info;
            while (current != nullptr) {
                result.push_back(current);
                current = current->next;
            }
            break;
        }
    }
    
    return result;
}

int Table::deleteByKey1(const std::string& key) {
    int index = -1;
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].key == key) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        std::cout << "Error: Key not found in KeySpace1" << std::endl;
        return -1;
    }
    
    // Удаление всех связанных элементов из второго пространства
    Item* current = ks1[index].info;
    while (current != nullptr) {
        Item* next = current->next;
        deleteByCompositeKey(key, current->key2);
        current = next;
    }
    
    // Обновление родительских ключей
    std::string keyToReplace = ks1[index].key;
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].par == keyToReplace) {
            ks1[i].par = "";
        }
    }
    
    return 0;
}

std::vector<Item*> Table::searchByParentKey(const std::string& parentKey) const {
    std::vector<Item*> result;
    
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].par == parentKey) {
            Item* current = ks1[i].info;
            while (current != nullptr) {
                result.push_back(current);
                current = current->next;
            }
        }
    }
    
    return result;
}

std::vector<Item*> Table::searchByKey2(unsigned int key) const {
    std::vector<Item*> result;
    
    int ks2Index = findKeyPosition(key);
    if (ks2Index != -1) {
        Node2* current = ks2[ks2Index].node;
        while (current != nullptr) {
            // Поиск соответствующего элемента в первом пространстве
            for (int i = 0; i < msize1; i++) {
                if (ks1[i].key != "") {
                    Item* item = ks1[i].info;
                    while (item != nullptr) {
                        if (item->info == current->info) {
                            result.push_back(item);
                            break;
                        }
                        item = item->next;
                    }
                }
            }
            current = current->next;
        }
    }
    
    return result;
}

int Table::deleteByKey2(unsigned int key) {
    int ks2Index = findKeyPosition(key);
    if (ks2Index == -1) {
        std::cout << "Error: Key not found in KeySpace2" << std::endl;
        return -1;
    }
    
    // Удаление всех связанных элементов из первого пространства
    Node2* current = ks2[ks2Index].node;
    while (current != nullptr) {
        Node2* next = current->next;
        
        // Поиск элемента в первом пространстве
        for (int i = 0; i < msize1; i++) {
            if (ks1[i].key != "") {
                Item* item = ks1[i].info;
                while (item != nullptr) {
                    if (item->info == current->info) {
                        Item* nextItem = item->next;
                        deleteByCompositeKey(ks1[i].key, key);
                        item = nextItem;
                    } else {
                        item = item->next;
                    }
                }
            }
        }
        
        current = next;
    }
    
    return 0;
}

std::vector<Item*> Table::searchAllVersionsByKey2(unsigned int key) const {
    return searchByKey2(key);
}

Item* Table::searchVersionByKey2(unsigned int key, int release) const {
    std::vector<Item*> items = searchByKey2(key);
    for (Item* item : items) {
        int ks2Index = item->ind2;
        Node2* node = ks2[ks2Index].node;
        while (node != nullptr) {
            if (node->release == release && node->info == item->info) {
                return item;
            }
            node = node->next;
        }
    }
    return nullptr;
}

int Table::deleteAllVersionsByKey2(unsigned int key) {
    return deleteByKey2(key);
}

int Table::deleteVersionByKey2(unsigned int key, int release) {
    Item* itemToDelete = searchVersionByKey2(key, release);
    if (itemToDelete == nullptr) {
        std::cout << "Error: Version not found" << std::endl;
        return -1;
    }
    
    return deleteByCompositeKey(itemToDelete->key1, key);
}

void Table::printTable() const {
    std::cout << "KeySpace1:" << std::endl;
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].key != "") {
            std::cout << "  Key: " << ks1[i].key 
                     << ", Parent: " << ks1[i].par 
                     << ", Items: ";
            
            Item* current = ks1[i].info;
            while (current != nullptr) {
                std::cout << "(KS2 key: " << current->key2 << ", Info: [" 
                         << current->info->str1 << ", " << current->info->str2 << "]) ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << "KeySpace2:" << std::endl;
    for (int i = 0; i < msize2; i++) {
        if (ks2[i].busy) {
            std::cout << "  Key: " << ks2[i].key << ", Versions: ";
            
            Node2* current = ks2[i].node;
            while (current != nullptr) {
                std::cout << "(Release: " << current->release << ", Info: [" 
                         << current->info->str1 << ", " << current->info->str2 << "]) ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
}
