#include "table.hpp"
#include <iostream>
#include <cstring>


void showMenu() {
    std::cout << "Menu:" << std::endl;
    std::cout << "1. Add element" << std::endl;
    std::cout << "2. Search by composite key" << std::endl;
    std::cout << "3. Delete by composite key" << std::endl;
    std::cout << "4. Search by KeySpace1 key" << std::endl;
    std::cout << "5. Search by KeySpace2 key" << std::endl;
    std::cout << "6. Delete by KeySpace1 key" << std::endl;
    std::cout << "7. Delete by KeySpace2 key" << std::endl;
    std::cout << "8. Search by parent key (KeySpace1)" << std::endl;
    std::cout << "9. Search all versions by KeySpace2 key" << std::endl;
    std::cout << "10. Search specific version by KeySpace2 key" << std::endl;
    std::cout << "11. Delete all versions by KeySpace2 key" << std::endl;
    std::cout << "12. Delete specific version by KeySpace2 key" << std::endl;
    std::cout << "13. Print table" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    Table table(10);
    int choice;
    
    do {
        showMenu();
        std::cin >> choice;
        std::cin.ignore(); // Очистка буфера
        
        switch (choice) {
            case 1: {
                char key1[MAX_STRING_LENGTH], parent[MAX_STRING_LENGTH];
                unsigned int key2;
                char str1[MAX_STRING_LENGTH], str2[MAX_STRING_LENGTH];
                
                std::cout << "Enter KeySpace1 key (string): ";
                std::cin.getline(key1, MAX_STRING_LENGTH);
                std::cout << "Enter parent key (string, empty for none): ";
                std::cin.getline(parent, MAX_STRING_LENGTH);
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cin.ignore();
                std::cout << "Enter first string: ";
                std::cin.getline(str1, MAX_STRING_LENGTH);
                std::cout << "Enter second string: ";
                std::cin.getline(str2, MAX_STRING_LENGTH);
                
                InfoType* info = new InfoType(str1, str2);
                if (table.addElement(key1, parent, key2, info) == 0) {
                    std::cout << "Element added successfully" << std::endl;
                }
                break;
            }
            
            case 2: {
                char key1[MAX_STRING_LENGTH];
                unsigned int key2;
                
                std::cout << "Enter KeySpace1 key (string): ";
                std::cin.getline(key1, MAX_STRING_LENGTH);
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cin.ignore();
                
                Item* item = table.searchByCompositeKey(key1, key2);
                if (item != nullptr) {
                    std::cout << "Found: [" << item->info->str1 << ", " << item->info->str2 << "]" << std::endl;
                } else {
                    std::cout << "Element not found" << std::endl;
                }
                break;
            }
            
            case 3: {
                char key1[MAX_STRING_LENGTH];
                unsigned int key2;
                
                std::cout << "Enter KeySpace1 key (string): ";
                std::cin.getline(key1, MAX_STRING_LENGTH);
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cin.ignore();
                
                if (table.deleteByCompositeKey(key1, key2) == 0) {
                    std::cout << "Element deleted successfully" << std::endl;
                }
                break;
            }
            
            case 4: {
                char key1[MAX_STRING_LENGTH];
                
                std::cout << "Enter KeySpace1 key (string): ";
                std::cin.getline(key1, MAX_STRING_LENGTH);
                
                std::vector<Item*> items = table.searchByKey1(key1);
                if (!items.empty()) {
                    std::cout << "Found elements:" << std::endl;
                    for (Item* item : items) {
                        std::cout << "  [" << item->info->str1 << ", " << item->info->str2 << "]" << std::endl;
                    }
                } else {
                    std::cout << "No elements found with this key" << std::endl;
                }
                break;
            }
            
            case 5: {
                unsigned int key2;
                
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cin.ignore();
                
                std::vector<Item*> items = table.searchByKey2(key2);
                if (!items.empty()) {
                    std::cout << "Found elements:" << std::endl;
                    for (Item* item : items) {
                        std::cout << "  [" << item->info->str1 << ", " << item->info->str2 << "]" << std::endl;
                    }
                } else {
                    std::cout << "No elements found with this key" << std::endl;
                }
                break;
            }
            
            case 6: {
                char key1[MAX_STRING_LENGTH];
                
                std::cout << "Enter KeySpace1 key (string): ";
                std::cin.getline(key1, MAX_STRING_LENGTH);
                
                if (table.deleteByKey1(key1) == 0) {
                    std::cout << "Elements deleted successfully" << std::endl;
                }
                break;
            }
            
            case 7: {
                unsigned int key2;
                
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cin.ignore();
                
                if (table.deleteByKey2(key2) == 0) {
                    std::cout << "Elements deleted successfully" << std::endl;
                }
                break;
            }
            
            case 8: {
                char parent[MAX_STRING_LENGTH];
                
                std::cout << "Enter parent key (string): ";
                std::cin.getline(parent, MAX_STRING_LENGTH);
                
                std::vector<Item*> items = table.searchByParentKey(parent);
                if (!items.empty()) {
                    std::cout << "Found elements:" << std::endl;
                    for (Item* item : items) {
                        std::cout << "  [" << item->info->str1 << ", " << item->info->str2 << "]" << std::endl;
                    }
                } else {
                    std::cout << "No elements found with this parent key" << std::endl;
                }
                break;
            }
            
            case 9: {
                unsigned int key2;
                
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cin.ignore();
                
                std::vector<Item*> items = table.searchAllVersionsByKey2(key2);
                if (!items.empty()) {
                    std::cout << "Found versions:" << std::endl;
                    int ks2Index = table.findKeyPosition(key2);
                    if (ks2Index != -1) {
                        Node2* node = table.getKs2()[ks2Index].node;
                        while (node != nullptr) {
                            std::cout << "  Release " << node->release << ": [" 
                                     << node->info->str1 << ", " << node->info->str2 << "]" << std::endl;
                            node = node->next;
                        }
                    }
                } else {
                    std::cout << "No versions found with this key" << std::endl;
                }
                break;
            }
            
            case 10: {
                unsigned int key2;
                int release;
                
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cout << "Enter release number: ";
                std::cin >> release;
                std::cin.ignore();
                
                Item* item = table.searchVersionByKey2(key2, release);
                if (item != nullptr) {
                    std::cout << "Found: [" << item->info->str1 << ", " << item->info->str2 << "]" << std::endl;
                } else {
                    std::cout << "Version not found" << std::endl;
                }
                break;
            }
            
            case 11: {
                unsigned int key2;
                
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cin.ignore();
                
                if (table.deleteAllVersionsByKey2(key2) == 0) {
                    std::cout << "All versions deleted successfully" << std::endl;
                }
                break;
            }
            
            case 12: {
                unsigned int key2;
                int release;
                
                std::cout << "Enter KeySpace2 key (unsigned int): ";
                std::cin >> key2;
                std::cout << "Enter release number: ";
                std::cin >> release;
                std::cin.ignore();
                
                if (table.deleteVersionByKey2(key2, release) == 0) {
                    std::cout << "Version deleted successfully" << std::endl;
                }
                break;
            }
            
            case 13: {
                table.printTable();
                break;
            }
            
            case 0: {
                std::cout << "Exiting..." << std::endl;
                break;
            }
            
            default: {
                std::cout << "Invalid choice, try again" << std::endl;
                break;
            }
        }
    } while (choice != 0);
    
    return 0;
}