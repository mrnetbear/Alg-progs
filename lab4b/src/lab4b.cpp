#include "llrb.hpp"
#include <iostream>
#include <chrono>
#include <limits>


// Функции для ввода данных
void printMenu();
unsigned int inputUnsignedInt(const std::string& prompt);
float inputFloat(const std::string& prompt);
std::string inputString(const std::string& prompt);

int main() {
    LLRBTree tree;
    int choice;
    
    do {
        printMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                unsigned int key = inputUnsignedInt("Enter key (unsigned integer): ");
                float num1 = inputFloat("Enter first float number: ");
                float num2 = inputFloat("Enter second float number: ");
                std::string str = inputString("Enter string: ");
                
                auto start = std::chrono::high_resolution_clock::now();
                Info* oldInfo = tree.insert(key, new Info(num1, num2, str));
                auto end = std::chrono::high_resolution_clock::now();
                
                if (oldInfo) {
                    std::cout << "Key already exists. Old info was replaced.\n";
                    delete oldInfo;
                } else {
                    std::cout << "Element inserted successfully.\n";
                }
                
                std::cout << "Time taken: " 
                     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                     << " microseconds\n";
                break;
            }
            case 2: {
                unsigned int key = inputUnsignedInt("Enter key to remove: ");
                
                auto start = std::chrono::high_resolution_clock::now();
                Info* deletedInfo = tree.remove(key);
                auto end = std::chrono::high_resolution_clock::now();
                
                if (deletedInfo) {
                    std::cout << "Element removed. Info was: " 
                         << deletedInfo->num1 << ", " << deletedInfo->num2 
                         << ", \"" << deletedInfo->str << "\"\n";
                    delete deletedInfo;
                } else {
                    std::cout << "Key not found.\n";
                }
                
                std::cout << "Time taken: " 
                     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                     << " microseconds\n";
                break;
            }
            case 3: {
                unsigned int key = inputUnsignedInt("Enter key to search: ");
                
                auto start = std::chrono::high_resolution_clock::now();
                Info* info = tree.get(key);
                auto end = std::chrono::high_resolution_clock::now();
                
                if (info) {
                    std::cout << "Found element: " 
                         << info->num1 << ", " << info->num2 
                         << ", \"" << info->str << "\"\n";
                } else {
                    std::cout << "Key not found.\n";
                }
                
                std::cout << "Time taken: " 
                     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                     << " microseconds\n";
                break;
            }
            case 4: {
                unsigned int a = inputUnsignedInt("Enter lower bound (a): ");
                unsigned int b = inputUnsignedInt("Enter upper bound (b): ");
                std::string order = inputString("Enter order (direct/reverse): ");
                bool reverse = (order == "reverse");
                
                auto start = std::chrono::high_resolution_clock::now();
                std::vector<Info*> results = tree.rangeQuery(a, b, reverse);
                auto end = std::chrono::high_resolution_clock::now();
                
                std::cout << "Found " << results.size() << " elements in range:\n";
                for (Info* info : results) {
                    std::cout << info->num1 << ", " << info->num2 
                         << ", \"" << info->str << "\"\n";
                }
                
                std::cout << "Time taken: " 
                     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                     << " microseconds\n";
                break;
            }
            case 5: {
                Key key = inputUnsignedInt("Enter key to compare: ");
                
                auto start = std::chrono::high_resolution_clock::now();
                Info* info = tree.findMostDifferent(key);
                auto end = std::chrono::high_resolution_clock::now();
                
                if (info) {
                    std::cout << "Most different element: " 
                         << info->num1 << ", " << info->num2 
                         << ", \"" << info->str << "\"\n";
                } else {
                    std::cout << "Tree is empty.\n";
                }
                
                std::cout << "Time taken: " 
                     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                     << " microseconds\n";
                break;
            }
            case 6: {
                tree.printTree();
                break;
            }
            case 7: {
                std::string filename = inputString("Enter filename to load: ");
                
                auto start = std::chrono::high_resolution_clock::now();
                tree.loadFromFile(filename);
                auto end = std::chrono::high_resolution_clock::now();
                
                std::cout << "Tree loaded from file.\n";
                std::cout << "Time taken: " 
                     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                     << " microseconds\n";
                break;
            }
            case 8: {
                std::string filename = inputString("Enter filename to save: ");
                
                auto start = std::chrono::high_resolution_clock::now();
                tree.saveToFile(filename);
                auto end = std::chrono::high_resolution_clock::now();
                
                std::cout << "Tree saved to file.\n";
                std::cout << "Time taken: " 
                     << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                     << " microseconds\n";
                break;
            }
            case 9: {
                tree.clear();
                std::cout << "Tree cleared.\n";
                break;
            }
            case 10: {
                std::string dotFilename = inputString("Enter DOT filename (e.g., tree.dot): ");
                std::string outputImageFilename = inputString("Enter output image filename (e.g., tree.png): ");
                tree.visualize(dotFilename, outputImageFilename);
                break;
            }
            case 0: {
                std::cout << "Exiting...\n";
                break;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    } while (choice != 0);
    
    return 0;
}

void printMenu() {
    std::cout << "\nMenu:\n"
    << "1. Insert element\n"
    << "2. Remove element\n"
    << "3. Search element\n"
    << "4. Range query\n"
    << "5. Find most different element\n"
    << "6. Print tree structure\n"
    << "7. Load from file\n"
    << "8. Save to file\n"
    << "9. Clear tree\n"
    << "10. Visualization\n"
    << "0. Exit\n"
    << "Enter your choice: ";
}

unsigned int inputUnsignedInt(const std::string& prompt) {
    unsigned int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid unsigned integer.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

float inputFloat(const std::string& prompt) {
    float value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid float number.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

std::string inputString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    getline(std::cin, value);
    return value;
}