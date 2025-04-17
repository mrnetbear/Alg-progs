#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>

typedef unsigned int UINT;

const UINT HASH_TABLE_SIZE = 2048;
const UINT MAX_STRING_LENGTH = 1024;

struct InfoType {
    char* str1;
    char* str2;
    
    InfoType(const char* s1, const char* s2);
    ~InfoType();
};

struct Item {
    InfoType* info;
    int release;
    Item* next;
    
    std::string key1;
    UINT key2;
    int ind1;
    int ind2;
    
    Item(InfoType* info, const std::string& key1, UINT key2, int ind1, int ind2);
    ~Item();
};

struct KeySpace1 {
    std::string key;
    std::string par;
    Item* info;
    
    KeySpace1();
    ~KeySpace1();
};

struct Node2 {
    int release;
    InfoType* info;
    Node2* next;
    
    Node2(InfoType* info, Node2* next = nullptr);
    ~Node2();
};

struct KeySpace2 {
    bool busy;
    UINT key;
    Node2* node;
    
    KeySpace2();
    ~KeySpace2();
};

class Table {
private:
    KeySpace1* ks1;
    KeySpace2* ks2;
    int msize1;
    int msize2;
    int csize1;
    int csize2;
    
    int hash1(UINT key) const;
    int hash2(UINT key) const;
    int findInsertPosition(UINT key) const;
    

public:
    Table(int msize1, int msize2 = HASH_TABLE_SIZE);
    ~Table();
    
    int findKeyPosition(UINT key) const;
    int addElement(const std::string& key1, const std::string& par, UINT key2, InfoType* info);
    Item* searchByCompositeKey(const std::string& key1, UINT key2) const;
    int deleteByCompositeKey(const std::string& key1, UINT key2);
    
    // Операции с KeySpace1
    std::vector<Item*> searchByKey1(const std::string& key) const;
    int deleteByKey1(const std::string& key);
    std::vector<Item*> searchByParentKey(const std::string& parentKey) const;
    
    // Операции с KeySpace2
    std::vector<Item*> searchByKey2(UINT key) const;
    const KeySpace2* getKs2() const { return ks2; }
    int deleteByKey2(UINT key);
    std::vector<Item*> searchAllVersionsByKey2(UINT key) const;
    Item* searchVersionByKey2(UINT key, int release) const;
    int deleteAllVersionsByKey2(UINT key);
    int deleteVersionByKey2(UINT key, int release);
    
    // Вывод
    void printTable() const;
    
    // Геттеры
    int getSize1() const { return csize1; }
    int getSize2() const { return csize2; }
    int getMaxSize1() const { return msize1; }
    int getMaxSize2() const { return msize2; }
};



#endif //TABLE_H