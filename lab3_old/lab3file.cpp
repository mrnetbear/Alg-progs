#include "filetable.hpp"
#include <iostream>
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
    std::cout << "9. Print table\n";
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

                    if (table.addElement(key1, parent, key2, str1, str2) == 0) {
                        std::cout << "Element added successfully\n";
                    } else {
                        std::cout << "Failed to add element (key may already exist)\n";
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

                    std::string str1, str2;
                    if (table.searchByCompositeKey(key1, key2, str1, str2)) {
                        std::cout << "Found: [" << str1 << ", " << str2 << "]\n";
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

                    if (table.deleteByCompositeKey(key1, key2) == 0) {
                        std::cout << "Element deleted successfully\n";
                    } else {
                        std::cout << "Element not found\n";
                    }
                    break;
                }

                case 4: { // Поиск по ключу KeySpace1
                    std::string key;
                    std::cout << "Enter KeySpace1 key: ";
                    std::getline(std::cin, key);

                    auto items = table.searchByKey1(key);
                    if (!items.empty()) {
                        std::cout << "Found " << items.size() << " elements:\n";
                        for (const auto& item : items) {
                            std::cout << "- [" << item.first << ", " << item.second << "]\n";
                        }
                    } else {
                        std::cout << "No elements found\n";
                    }
                    break;
                }

                case 5: { // Поиск по ключу KeySpace2
                    unsigned int key;
                    std::cout << "Enter KeySpace2 key: ";
                    std::cin >> key;
                    clearInputBuffer();

                    auto items = table.searchByKey2(key);
                    if (!items.empty()) {
                        std::cout << "Found " << items.size() << " elements:\n";
                        for (const auto& item : items) {
                            std::cout << "- [" << item.first << ", " << item.second << "]\n";
                        }
                    } else {
                        std::cout << "No elements found\n";
                    }
                    break;
                }

                case 6: { // Удаление по ключу KeySpace1
                    std::string key;
                    std::cout << "Enter KeySpace1 key to delete: ";
                    std::getline(std::cin, key);

                    if (table.deleteByKey1(key) == 0) {
                        std::cout << "All elements with key '" << key << "' deleted\n";
                    } else {
                        std::cout << "Key not found\n";
                    }
                    break;
                }

                case 7: { // Удаление по ключу KeySpace2
                    unsigned int key;
                    std::cout << "Enter KeySpace2 key to delete: ";
                    std::cin >> key;
                    clearInputBuffer();

                    if (table.deleteByKey2(key) == 0) {
                        std::cout << "All elements with key " << key << " deleted\n";
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
                            std::cout << "- [" << item.first << ", " << item.second << "]\n";
                        }
                    } else {
                        std::cout << "No child elements found\n";
                    }
                    break;
                }

                case 9: { // Печать таблицы
                    table.printTable();
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