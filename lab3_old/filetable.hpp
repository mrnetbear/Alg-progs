#ifndef FILETABLE_H
#define FILETABLE_H

#include "table.hpp"
#include <fstream>
#include <string>
#include <cstring>

class FileBackedTable : public Table {
private:
    std::string filename;
    std::fstream file;

    // Структуры для хранения в файле
    struct FileHeader {
        int msize1;
        int msize2;
        int csize1;
        int csize2;
    };

    struct FileKeySpace1 {
        char key[MAX_STRING_LENGTH];
        char par[MAX_STRING_LENGTH];
        long info_offset;  // Смещение к первому элементу в файле
    };

    struct FileKeySpace2 {
        bool busy;
        UINT key;
        long node_offset;  // Смещение к первому узлу в файле
    };

    struct FileItem {
        long info_offset;
        int release;
        long next_offset;
        char key1[MAX_STRING_LENGTH];
        UINT key2;
        int ind1;
        int ind2;
    };

    struct FileNode2 {
        long info_offset;
        int release;
        long next_offset;
    };

    struct FileInfo {
        std::string str1;
        std::string str2;
    };

    // Вспомогательные методы
    void writeInfo(long offset, const InfoType* info);
    InfoType* readInfo(long offset);
    long writeNewInfo(const InfoType* info);
    void updateItem(Item* item, long offset);
    Item* readItem(long offset);
    Node2* readNode2(long offset);

public:
    FileBackedTable(const std::string& filename, int msize1, int msize2 = HASH_TABLE_SIZE);
    ~FileBackedTable();

    int addElement(const std::string& key1, const std::string& par, UINT key2, InfoType* info) override;
    Item* searchByCompositeKey(const std::string& key1, UINT key2) const override;
    int deleteByCompositeKey(const std::string& key1, UINT key2) override;

    void loadFromFile();
    void saveToFile();
};

#endif // FILETABLE_H