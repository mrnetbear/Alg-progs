#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>

typedef unsigned int UINT;

const UINT HASH_TABLE_SIZE = 2048;
const UINT MAX_STRING_LENGTH = 1024;

class InfoType {
private:
    std::string str1;
    std::string str2;
    
public:
    InfoType(){};
    InfoType(const std::string s1, const std::string s2);
    std::string getStr1() const {return str1;};
    std::string getStr2() const {return str2;};
    ~InfoType(){};
    std::string printInfo();
};

class Item {
private:
    InfoType* info;
    int release;
    Item* next;
    
    std::string key1;
    UINT key2;
    int ind1;
    int ind2;
    
public:
    Item(InfoType* info, const std::string& key1, 
            UINT key2, int ind1, int ind2);
    Item(){};
    ~Item(){};
    void setInd1(int ind);
    void setInd2(int ind);
    void setNext(Item* newNext);
    void setRelease(int newRelease);
    std::string getKey1();
    UINT getKey2();
    Item* getNext();
    InfoType* getInfo();
    int getRelease();
    int getInd1();
    int getInd2();
};

class KeySpace1 {
private:
    std::string key;
    std::string par;
    Item* info;

public:
    KeySpace1();
    ~KeySpace1(){};
    void setValues(std::string newKey, std::string newPar, 
        Item* newInfo);
    void setInfo(Item* newInfo);
    void setPar(std::string newPar);
    std::string getKey();
    std::string getPar();
    Item* getInfo();
};

class Node2 {
private:
    int release;
    InfoType* info;
    Node2* next;
    
public:
    Node2(InfoType* info, Node2* next = nullptr);
    Node2(){};
    ~Node2(){};
    Node2* getNext();
    InfoType* getInfo();
    int getRelease();
    void setNext(Node2* newNext);
    void setRelease(int newRelease);
};

class KeySpace2 {
private:
    bool busy;
    UINT key;
    Node2* node;
    
public:
    KeySpace2();
    ~KeySpace2(){};
    bool isBusy();
    UINT getKey();
    Node2* getNode() const;
    void setParameters(bool newBusy, UINT newKey, Node2* newNode);
};

class Table {
protected:
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
    virtual int addElement(const std::string& key1, const std::string& par, UINT key2, InfoType* info);
    virtual Item* searchByCompositeKey(const std::string& key1, UINT key2) const;
    virtual int deleteByCompositeKey(const std::string& key1, UINT key2);
    
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