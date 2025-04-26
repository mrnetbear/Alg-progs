#include "table.hpp"
#include <iostream>
#include <algorithm>

InfoType::InfoType(const std::string s1, const std::string s2) {
    str1 = s1;
    str2 = s2;
}

std::string InfoType::printInfo(){
    std::string result;
    return str1 + ", " + str2;
}

// Реализация Item
Item::Item(InfoType* info, const std::string& key1, UINT key2, int ind1, int ind2) 
    : info(info), key1(key1), key2(key2), ind1(ind1), ind2(ind2), next(nullptr), release(1) {}

void Item::setInd1(int ind){
    ind1 = ind;
}

void Item::setInd2(int ind){
    ind2 = ind;
}

void Item::setNext(Item* newNext){
    next = newNext;
}

void Item::setRelease(int newRelease){
    release = newRelease;
}

std::string Item::getKey1(){
    return key1;
}

UINT Item::getKey2(){
    return key2;
};

Item* Item::getNext(){
    return next;
}

InfoType* Item::getInfo(){
    return info;
}

int Item::getRelease(){
    return release;
}

int Item::getInd1(){
    return ind1;
}

int Item::getInd2(){
    return ind2;
}


// Реализация KeySpace1
KeySpace1::KeySpace1() : key(""), par(""), info(nullptr) {}

void KeySpace1::setValues(std::string newKey, std::string newPar, Item* newInfo){
    key = newKey; par = newPar; info = newInfo;
}

void KeySpace1::setInfo(Item* newInfo){
    info = newInfo;
}

void KeySpace1::setPar(std::string newPar){
    par = newPar;
}

std::string KeySpace1::getKey(){
    return key;
}

std::string KeySpace1::getPar(){
    return par;
}

Item* KeySpace1::getInfo(){
    return info;
} 

//Реализация Node2 
Node2::Node2(InfoType* info, Node2* next) : info(info), next(next) {
    release = 1;
}

Node2* Node2::getNext(){
    return next;
}

InfoType* Node2::getInfo(){
    return info;
}

int Node2::getRelease(){
    return release;
}

void  Node2::setNext(Node2* newNext){
    next = newNext;
}

void Node2::setRelease(int newRelease){
    release = newRelease;
}

// Реализация KeySpace2
KeySpace2::KeySpace2() : busy(false), key(0), node(nullptr) {}

bool KeySpace2::isBusy(){
    return busy;
}

UINT KeySpace2::getKey(){
    return key;
}

Node2* KeySpace2::getNode() const{
    return node;
}

void KeySpace2::setParameters(bool newBusy, UINT newKey, 
    Node2* newNode){
    busy = newBusy; key = newKey; node = newNode;
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

int Table::hash1(UINT key) const {
    return key % msize2;
}

int Table::hash2(UINT key) const {
    return 1 + (key % (msize2 - 1));
}

int Table::findInsertPosition(UINT key) const {
    int h1 = hash1(key);
    int h2 = hash2(key);
    
    for (int i = 0; i < msize2; i++) {
        int index = (h1 + i * h2) % msize2;
        if (!ks2[index].isBusy()) {
            return index;
        }
    }
    
    return -1;
}

int Table::findKeyPosition(UINT key) const {
    int h1 = hash1(key);
    int h2 = hash2(key);
    
    for (int i = 0; i < msize2; i++) {
        int index = (h1 + i * h2) % msize2;
        if (ks2[index].isBusy() && ks2[index].getKey() == key) {
            return index;
        }
        if (!ks2[index].isBusy()) {
            return -1;
        }
    }
    
    return -1;
}

int Table::addElement(const std::string& key1, const std::string& par, unsigned int key2, InfoType* info) {
    // Проверка уникальности составного ключа
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].getKey() == key1) {
            Item* current = ks1[i].getInfo();
            while (current != nullptr) {
                if (current->getKey2() == key2) {
                    std::cout << "Error: Element with this composite key already exists" << std::endl;
                    return -1;
                }
                current = current->getNext();
            }
        }
    }
    
    // Проверка родительского ключа
    if (par != "") {
        bool parentFound = false;
        for (int i = 0; i < msize1; i++) {
            if (ks1[i].getKey() == par) {
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
        if (ks1[i].getKey() == "") {
            ks1Index = i;
            break;
        }
    }
    
    if (ks1Index == -1) {
        std::cout << "Error: No space in KeySpace1" << std::endl;
        delete newItem;
        return -1;
    }
    
    ks1[ks1Index].setValues(key1, par, newItem);
    csize1++;
    newItem->setInd1(ks1Index);
    
    // Добавление во второе пространство ключей
    int ks2Index = findInsertPosition(key2);
    if (ks2Index == -1) {
        std::cout << "Error: No space in KeySpace2" << std::endl;
        // Откат изменений в первом пространстве
        ks1[ks1Index].setValues("", "", nullptr);
        csize1--;
        delete newItem;
        return -1;
    }
    
    Node2* newNode = new Node2(info);
    newItem->setInd2(ks2Index);
    
    if (ks2[ks2Index].isBusy()) {
        // Добавление новой версии
        Node2* current = ks2[ks2Index].getNode();
        int release = 1;
        while (current->getNext() != nullptr) {
            current = current->getNext();
            release++;
        }
        current->setNext(newNode);
        newNode->setRelease(release + 1);
    } else {
        // Первый элемент с этим ключом
        ks2[ks2Index].setParameters(true, key2, newNode);
        csize2++;
    }
    
    return 0;
}

Item* Table::searchByCompositeKey(const std::string& key1, unsigned int key2) const {
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].getKey() == key1) {
            Item* current = ks1[i].getInfo();
            while (current != nullptr) {
                if (current->getKey2() == key2) {
                    return current;
                }
                current = current->getNext();
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
        if (ks1[i].getKey() == key1) {
            if (ks1[i].getInfo() == itemToDelete) {
                ks1[i].setInfo(itemToDelete->getNext());
                if (ks1[i].getInfo() == nullptr) {
                    ks1[i].setValues("", "", nullptr);
                    csize1--;
                }
            } else {
                Item* prev = ks1[i].getInfo();
                while (prev->getNext() != itemToDelete) {
                    prev = prev->getNext();
                }
                prev->setNext(itemToDelete->getNext());
            }
            break;
        }
    }
    
    // Удаление из второго пространства ключей
    int ks2Index = findKeyPosition(key2);
    if (ks2Index != -1) {
        Node2* nodeToDelete = nullptr;
        if (ks2[ks2Index].getNode()->getInfo() == itemToDelete->getInfo()) {
            nodeToDelete = ks2[ks2Index].getNode();
            ks2[ks2Index].setParameters( false, 0, nodeToDelete->getNext());
            if (ks2[ks2Index].getNode() == nullptr) {
                ks2[ks2Index].setParameters(false, 0, nullptr);
                csize2--;
            }
        } else {
            Node2* prev = ks2[ks2Index].getNode();
            while (prev->getNext() != nullptr && prev->getNext()->getInfo() != itemToDelete->getInfo()) {
                prev = prev->getNext();
            }
            if (prev->getNext() != nullptr) {
                nodeToDelete = prev->getNext();
                prev->setNext(nodeToDelete->getNext());
            }
        }
        
        if (nodeToDelete != nullptr) {
            nodeToDelete->setNext(nullptr); // Чтобы не удалить цепочку
            delete nodeToDelete;
        }
    }
    
    itemToDelete->setNext(nullptr); // Чтобы не удалить цепочку
    delete itemToDelete;
    
    return 0;
}

std::vector<Item*> Table::searchByKey1(const std::string& key) const {
    std::vector<Item*> result;
    
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].getKey() == key) {
            Item* current = ks1[i].getInfo();
            while (current != nullptr) {
                result.push_back(current);
                current = current->getNext();
            }
            break;
        }
    }
    
    return result;
}

int Table::deleteByKey1(const std::string& key) {
    int index = -1;
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].getKey() == key) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        std::cout << "Error: Key not found in KeySpace1" << std::endl;
        return -1;
    }
    
    // Удаление всех связанных элементов из второго пространства
    Item* current = ks1[index].getInfo();
    while (current != nullptr) {
        Item* next = current->getNext();
        deleteByCompositeKey(key, current->getKey2());
        current = next;
    }
    
    // Обновление родительских ключей
    std::string keyToReplace = ks1[index].getKey();
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].getPar() == keyToReplace) {
            ks1[i].setPar("");
        }
    }
    ks1[index].setValues("", "", nullptr);
    
    return 0;
}

std::vector<Item*> Table::searchByParentKey(const std::string& parentKey) const {
    std::vector<Item*> result;
    
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].getPar() == parentKey) {
            Item* current = ks1[i].getInfo();
            while (current != nullptr) {
                result.push_back(current);
                current = current->getNext();
            }
        }
    }
    
    return result;
}

std::vector<Item*> Table::searchByKey2(unsigned int key) const {
    std::vector<Item*> result;
    
    int ks2Index = findKeyPosition(key);
    if (ks2Index != -1) {
        Node2* current = ks2[ks2Index].getNode();
        while (current != nullptr) {
            // Поиск соответствующего элемента в первом пространстве
            for (int i = 0; i < msize1; i++) {
                if (ks1[i].getKey() != "") {
                    Item* item = ks1[i].getInfo();
                    while (item != nullptr) {
                        if (item->getInfo() == current->getInfo()) {
                            result.push_back(item);
                            break;
                        }
                        item = item->getNext();
                    }
                }
            }
            current = current->getNext();
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
    Node2* current = ks2[ks2Index].getNode();
    while (current != nullptr) {
        Node2* next = current->getNext();
        
        // Поиск элемента в первом пространстве
        for (int i = 0; i < msize1; i++) {
            if (ks1[i].getKey() != "") {
                Item* item = ks1[i].getInfo();
                while (item != nullptr) {
                    if (item->getInfo() == current->getInfo()) {
                        Item* nextItem = item->getNext();
                        deleteByCompositeKey(ks1[i].getKey(), key);
                        item = nextItem;
                    } else {
                        item = item->getNext();
                    }
                }
            }
        }
        
        current = next;
    }
    
    ks2[ks2Index].setParameters(false, 0, nullptr);

    return 0;
}

std::vector<Item*> Table::searchAllVersionsByKey2(unsigned int key) const {
    return searchByKey2(key);
}

Item* Table::searchVersionByKey2(unsigned int key, int release) const {
    std::vector<Item*> items = searchByKey2(key);
    for (Item* item : items) {
        int ks2Index = item->getInd2();
        Node2* node = ks2[ks2Index].getNode();
        while (node != nullptr) {
            if (node->getRelease() == release && node->getInfo() == item->getInfo()) {
                return item;
            }
            node = node->getNext();
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
    
    return deleteByCompositeKey(itemToDelete->getKey1(), key);
}

void Table::printTable() const {
    std::cout << "KeySpace1:" << std::endl;
    for (int i = 0; i < msize1; i++) {
        if (ks1[i].getKey() != "") {
            std::cout << "  Key: " << ks1[i].getKey() 
                     << ", Parent: " << ks1[i].getPar() 
                     << ", Items: ";
            
            Item* current = ks1[i].getInfo();
            while (current != nullptr) {
                std::cout << "(KS2 key: " << current->getKey2() 
                << ", Info: [" << current->getInfo()->printInfo() << "]) ";
                current = current->getNext();
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << "KeySpace2:" << std::endl;
    for (int i = 0; i < msize2; i++) {
        if (ks2[i].isBusy()) {
            std::cout << "  Key: " << ks2[i].getKey() << ", Versions: ";
            
            Node2* current = ks2[i].getNode();
            while (current != nullptr) {
                std::cout << "(Release: " << current->getRelease() << ", Info: [" 
                         << current->getInfo()->printInfo() << "]) ";
                current = current->getNext();
            }
            std::cout << std::endl;
        }
    }
}
