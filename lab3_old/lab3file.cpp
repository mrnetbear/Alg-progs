#include "filetable.hpp"
#include <iostream>
#include <cstring>
#include <limits>

void showMenu() {
    std::cout << "\nFile-Backed Table Menu:\n";
    std::cout << "1. Add element\n";
    std::cout << "2. Search by composite key\n";
    std::cout << "3. Delete by composite key\n";
    std::cout << "4. Search by KeySpace1 key\n";
    std::cout << "5. Search by KeySpace2 key\n";
    std::cout << "6. Delete by KeySpace1 key\n";
    std::cout << "7. Delete by KeySpace2 key\n";
    std::cout << "8. Search by parent key (KeySpace1)\n";
    std::cout << "9. Search all versions by KeySpace2 key\n";
    std::cout << "10. Search specific version by KeySpace2 key\n";
    std::cout << "11. Delete all versions by KeySpace2 key\n";
    std::cout << "12. Delete specific version by KeySpace2 key\n";
    std::cout << "13. Print table\n";
    std::cout << "14. Save table to file (manual)\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    std::string filename;
    std::cout << "Enter filename for table storage: ";
    std::getline(std::cin, filename);

    int msize1;
    std::cout << "Enter maximum size for KeySpace1: ";
    std::cin >> msize1;
    clearInputBuffer();

    // Инициализация таблицы с файловой поддержкой
    FileBackedTable table(filename, msize1);
    std::cout << "Table initialized with file: " << filename << "\n";

    int choice;
    do {
        showMenu();
        std::cin >> choice;
        clearInputBuffer();

        try {
            switch (choice) {
                case 1: { // Добавление элемента
                    std::string key1, parent, str1, str2;
                    unsigned int key2;

                    std::cout << "Enter KeySpace1 key: ";
                    std::getline(std::cin, key1);
                    std::cout << "Enter parent key (empty if none): ";
                    std::getline(std::cin, parent);
                    std::cout << "Enter KeySpace2 key (number): ";
                    std::cin >> key2;
                    clearInputBuffer();
                    std::cout << "Enter first string: ";
                    std::getline(std::cin, str1);
                    std::cout << "Enter second string: ";
                    std::getline(std::cin, str2);

                    InfoType* info = new InfoType(str1, str2);
                    if (table.addElement(key1, parent, key2, info) == 0) {
                        std::cout << "Element added successfully\n";
                    } else {
                        delete info; // Освобождаем память в случае ошибки
                    }
                    break;
                }

                case 2: { // Поиск по составному ключу
                    std::string key1;
                    unsigned int key2;

                    std::cout << "Enter KeySpace1 key: ";
                    std::getline(std::cin, key1);
                    std::cout << "Enter KeySpace2 key: ";
                    std::cin >> key2;
                    clearInputBuffer();

                    Item* item = table.searchByCompositeKey(key1, key2);
                    if (item) {
                        std::cout << "Found: " << item->getInfo()->printInfo() << "\n";
                    } else {
                        std::cout << "Element not found\n";
                    }
                    break;
                }

                case 3: { // Удаление по составному ключу
                    std::string key1;
                    unsigned int key2;

                    std::cout << "Enter KeySpace1 key: ";
                    std::getline(std::cin, key1);
                    std::cout << "Enter KeySpace2 key: ";
                    std::cin >> key2;
                    clearInputBuffer();

                    if (table.deleteByCompositeKey(key1, key2)) {
                        std::cout << "Element deleted successfully\n";
                    } else {
                        std::cout << "Element not found\n";
                    }
                    break;
                }

                case 4: { // Поиск по ключу KeySpace1
                    std::string key1;
                    std::cout << "Enter KeySpace1 key: ";
                    std::getline(std::cin, key1);

                    auto items = table.searchByKey1(key1);
                    if (!items.empty()) {
                        std::cout << "Found " << items.size() << " elements:\n";
                        for (const auto& item : items) {
                            std::cout << "- " << item->getInfo()->printInfo() << "\n";
                        }
                    } else {
                        std::cout << "No elements found\n";
                    }
                    break;
                }

                case 5: { // Поиск по ключу KeySpace2
                    unsigned int key2;
                    std::cout << "Enter KeySpace2 key: ";
                    std::cin >> key2;
                    clearInputBuffer();

                    auto items = table.searchByKey2(key2);
                    if (!items.empty()) {
                        std::cout << "Found " << items.size() << " elements:\n";
                        for (const auto& item : items) {
                            std::cout << "- " << item->getInfo()->printInfo() << "\n";
                        }
                    } else {
                        std::cout << "No elements found\n";
                    }
                    break;
                }

                case 6: { // Удаление по ключу KeySpace1
                    std::string key1;
                    std::cout << "Enter KeySpace1 key to delete: ";
                    std::getline(std::cin, key1);

                    if (table.deleteByKey1(key1) == 0) {
                        std::cout << "All elements with key '" << key1 << "' deleted\n";
                    } else {
                        std::cout << "Key not found\n";
                    }
                    break;
                }

                case 7: { // Удаление по ключу KeySpace2
                    unsigned int key2;
                    std::cout << "Enter KeySpace2 key to delete: ";
                    std::cin >> key2;
                    clearInputBuffer();

                    if (table.deleteByKey2(key2) == 0) {
                        std::cout << "All elements with key " << key2 << " deleted\n";
                    } else {
                        std::cout << "Key not found\n";
                    }
                    break;
                }

                case 8: { // Поиск по родительскому ключу
                    std::string parent;
                    std::cout << "Enter parent key: ";
                    std::getline(std::cin, parent);

                    auto items = table.searchByParentKey(parent);
                    if (!items.empty()) {
                        std::cout << "Found " << items.size() << " child elements:\n";
                        for (const auto& item : items) {
                            std::cout << "- " << item->getInfo()->printInfo() << "\n";
                        }
                    } else {
                        std::cout << "No child elements found\n";
                    }
                    break;
                }

                case 9: { // Поиск всех версий по ключу KeySpace2
                    unsigned int key2;
                    std::cout << "Enter KeySpace2 key: ";
                    std::cin >> key2;
                    clearInputBuffer();

                    auto items = table.searchAllVersionsByKey2(key2);
                    if (!items.empty()) {
                        std::cout << "Found " << items.size() << " versions:\n";
                        for (const auto& item : items) {
                            std::cout << "- Version " << item->getRelease() << ": " 
                                     << item->getInfo()->printInfo() << "\n";
                        }
                    } else {
                        std::cout << "No versions found\n";
                    }
                    break;
                }

                case 10: { // Поиск конкретной версии
                    unsigned int key2;
                    int release;
                    std::cout << "Enter KeySpace2 key: ";
                    std::cin >> key2;
                    std::cout << "Enter version number: ";
                    std::cin >> release;
                    clearInputBuffer();

                    Item* item = table.searchVersionByKey2(key2, release);
                    if (item) {
                        std::cout << "Found version " << release << ": " 
                                 << item->getInfo()->printInfo() << "\n";
                    } else {
                        std::cout << "Version not found\n";
                    }
                    break;
                }

                case 11: { // Удаление всех версий
                    unsigned int key2;
                    std::cout << "Enter KeySpace2 key: ";
                    std::cin >> key2;
                    clearInputBuffer();

                    if (table.deleteAllVersionsByKey2(key2) == 0) {
                        std::cout << "All versions deleted\n";
                    } else {
                        std::cout << "Key not found\n";
                    }
                    break;
                }

                case 12: { // Удаление конкретной версии
                    unsigned int key2;
                    int release;
                    std::cout << "Enter KeySpace2 key: ";
                    std::cin >> key2;
                    std::cout << "Enter version number: ";
                    std::cin >> release;
                    clearInputBuffer();

                    if (table.deleteVersionByKey2(key2, release) == 0) {
                        std::cout << "Version " << release << " deleted\n";
                    } else {
                        std::cout << "Version not found\n";
                    }
                    break;
                }

                case 13: { // Печать таблицы
                    table.printTable();
                    break;
                }

                case 14: { // Ручное сохранение
                    table.saveToFile();
                    std::cout << "Table saved to file\n";
                    break;
                }

                case 0: { // Выход
                    std::cout << "Exiting...\n";
                    break;
                }

                default: {
                    std::cout << "Invalid choice, try again\n";
                    break;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    } while (choice != 0);

    return 0;
}