#ifndef FILETABLE_H
#define FILETABLE_H
#include "table.hpp"
#include <fstream>
#include <string>

// Структура для хранения информации в файле
struct FileInfo {
    long str1_offset;
    long str2_offset;
    int str1_len;
    int str2_len;
};

// Файловая версия таблицы
class FileTable : public Table {
private:
    std::string filename;
    std::fstream file;
    
    // Методы для работы с файлом
    long writeString(const char* str);
    char* readString(long offset, int length);
    long writeInfo(InfoType* info);
    InfoType* readInfo(long offset);
    
public:
    FileTable(int msize1, int msize2, const std::string& filename);
    ~FileTable();
    
    // Переопределенные методы
    int addElement(const std::string& key1, const std::string& par, unsigned int key2, InfoType* info) override;
    Item* searchByCompositeKey(const std::string& key1, unsigned int key2) const override;
    int deleteByCompositeKey(const std::string& key1, unsigned int key2) override;
    
    // Дополнительные методы для работы с файлом
    void saveToFile();
    void loadFromFile();
};

#endif //FILETABLE_H