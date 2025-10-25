#ifndef FILETABLE_H
#define FILETABLE_H

#include <fstream>
#include <string>
#include <vector>
#include <cstring>

const unsigned int MAX_STRING_LENGTH = 10;
//const unsigned int HASH_TABLE_SIZE = 2048;

class FileBackedTable {
private:
    std::string filename;
    mutable std::fstream file;

    struct FileHeader {
        int msize1;
        int msize2;
        int csize1;
        int csize2;
    };

    struct FileKeySpace1 {
        char key[MAX_STRING_LENGTH];
        char par[MAX_STRING_LENGTH];
        long first_item_offset;
    };

    struct FileKeySpace2 {
        bool busy;
        unsigned int key;
        long first_node_offset;
    };

    struct FileItem {
        long info_offset;
        int release;
        long next_offset;
        char key1[MAX_STRING_LENGTH];
        unsigned int key2;
        int ind1;
        int ind2;
    };

    struct FileNode2 {
        long info_offset;
        int release;
        long next_offset;
    };

    struct FileInfo {
        char str1[MAX_STRING_LENGTH];
        char str2[MAX_STRING_LENGTH];
    };

    // Метаданные
    int msize1;
    int msize2;
    int csize1;
    int csize2;

    // Вспомогательные методы
    long writeInfo(const std::string& str1, const std::string& str2);
    void readInfo(long offset, std::string& str1, std::string& str2) const;
    long allocateSpace(size_t size);
    void updateHeader();
    long findEmptyKs1Position() const;
    long findKs1Position(const std::string& key) const;
    long findKs2InsertPosition(unsigned int key) const;
    long findKs2Position(unsigned int key) const;
    void updateKs1At(long offset, const FileKeySpace1& ks1);
    void updateKs2At(long offset, const FileKeySpace2& ks2);
    void writeItemAt(long offset, const FileItem& item);
    void writeNodeAt(long offset, const FileNode2& node);

public:
    FileBackedTable(const std::string& filename, int msize1, int msize2); //= HASH_TABLE_SIZE);
    ~FileBackedTable();

    // Основные операции
    int addElement(const std::string& key1, const std::string& par, unsigned int key2, 
                  const std::string& infoStr1, const std::string& infoStr2);
    bool searchByCompositeKey(const std::string& key1, unsigned int key2, 
                            std::string& outStr1, std::string& outStr2) const;
    int deleteByCompositeKey(const std::string& key1, unsigned int key2);

    // Поиск
    std::vector<std::pair<std::string, std::string>> searchByKey1(const std::string& key) const;
    std::vector<std::pair<std::string, std::string>> searchByKey2(unsigned int key) const;
    std::vector<std::pair<std::string, std::string>> searchByParentKey(const std::string& parent) const;

    // Удаление
    int deleteByKey1(const std::string& key);
    int deleteByKey2(unsigned int key);

    // Вывод
    void printTable() const;
};

#endif // FILETABLE_H