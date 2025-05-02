#include "bintree.hpp"
#include <iostream>
#include <chrono>
#include <cctype>

// Функция для вывода меню
void printMenu(bool state) {
    std::cout << "allowDuplicates = " << state << std::endl;
    std::cout << "\nMenu:\n";
    std::cout << "1. Insert element\n";
    std::cout << "2. Remove element\n";
    std::cout << "3. Find element\n";
    std::cout << "4. Print in reverse order in range\n";
    std::cout << "5. Find smallest key greater than given\n";
    std::cout << "6. Print tree structure\n";
    std::cout << "7. Load from file\n";
    std::cout << "8. Clear tree\n";
    std::cout << "9. Exit\n";
    std::cout << "10. Visualization\n";
    if (!state)
        std::cout << "0. AllowDuplicates\n";
    else
    std::cout << "0. DisableDuplicates\n";
    std::cout << "Enter your choice: ";
}

// Функция для ввода строки
std::string inputString(const std::string& prompt) {
    std::string result;
    std::cout << prompt;
    std::getline(std::cin, result);
    return result;
}

// Функция для ввода числа
int inputInt(const std::string& prompt) {
    int result;
    std::cout << prompt;
    while (!(std::cin >> result)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an integer: ";
    }
    std::cin.ignore(); // Игнорируем оставшийся символ новой строки
    return result;
}

// Основная функция
int main() {
    BSTree tree;
    int choice;
    bool allowDuplicates = false;
    
    do {
        printMenu(allowDuplicates);
        choice = inputInt("");
        
        switch (choice) {
            case 1: { // Вставка элемента
                std::string key = inputString("Enter key: ");
                int num1 = inputInt("Enter first number: ");
                int num2 = inputInt("Enter second number: ");
                std::string text = inputString("Enter text: ");
                
                auto start = std::chrono::high_resolution_clock::now();
                bool success = tree.insert(key, num1, num2, text, allowDuplicates);
                auto end = std::chrono::high_resolution_clock::now();
                
                if (success) {
                    std::cout << "Element inserted successfully. Time: " 
                              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                              << " microseconds\n";
                } else {
                    std::cout << "Error: Duplicate key not allowed\n";
                }
                break;
            }
            case 2: { // Удаление элемента
                std::string key = inputString("Enter key to remove: ");
                int index = inputInt("Enter index (0 for first): ");
                
                auto start = std::chrono::high_resolution_clock::now();
                bool success = tree.remove(key, index);
                auto end = std::chrono::high_resolution_clock::now();
                
                if (success) {
                    std::cout << "Element removed successfully. Time: " 
                              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                              << " microseconds\n";
                } else {
                    std::cout << "Error: Key not found or invalid index\n";
                }
                break;
            }
            case 3: { // Поиск элемента
                std::string key = inputString("Enter key to find: ");
                int index = inputInt("Enter index (0 for first): ");
                
                auto start = std::chrono::high_resolution_clock::now();
                Info* info = tree.find(key, index);
                auto end = std::chrono::high_resolution_clock::now();
                
                if (info) {
                    std::cout << "Found: " << info->number1 << ", " << info->number2 
                              << ", \"" << info->text << "\". Time: "
                              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                              << " microseconds\n";
                } else {
                    std::cout << "Key not found or invalid index\n";
                }
                break;
            }
            case 4: { // Вывод в обратном порядке в диапазоне
                std::string startKey = inputString("Enter start key: ");
                std::string endKey = inputString("Enter end key: ");
                
                auto start = std::chrono::high_resolution_clock::now();
                std::cout << "Elements in reverse order from " << startKey << " to " << endKey << ":\n";
                tree.printReverseRange(startKey, endKey);
                auto end = std::chrono::high_resolution_clock::now();
                
                std::cout << "Operation time: " 
                          << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                          << " microseconds\n";
                break;
            }
            case 5: { // Поиск минимального ключа, большего заданного
                std::string key = inputString("Enter key: ");
                int index = inputInt("Enter index (0 for first): ");
                
                auto start = std::chrono::high_resolution_clock::now();
                Info* info = tree.findMinGreater(key, index);
                auto end = std::chrono::high_resolution_clock::now();
                
                if (info) {
                    std::cout << "Found: " << info->number1 << ", " << info->number2 
                              << ", \"" << info->text << "\". Time: "
                              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                              << " microseconds\n";
                } else {
                    std::cout << "No key greater than " << key << " found\n";
                }
                break;
            }
            case 6: { // Вывод структуры дерева
                tree.printTree();
                break;
            }
            case 7: { // Загрузка из файла
                std::string filename = inputString("Enter filename: ");
                allowDuplicates = inputInt("Allow duplicates? (0 - no, 1 - yes): ");
                
                auto start = std::chrono::high_resolution_clock::now();
                bool success = tree.loadFromFile(filename, allowDuplicates);
                auto end = std::chrono::high_resolution_clock::now();
                
                if (success) {
                    std::cout << "Tree loaded from file successfully. Time: " 
                              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
                              << " microseconds\n";
                } else {
                    std::cout << "Error loading file\n";
                }
                break;
            }
            case 8: { // Очистка дерева
                tree.clear();
                std::cout << "Tree cleared\n";
                break;
            }
            case 9: { // Выход
                std::cout << "Exiting...\n";
                break;
            }
            case 10: { // Визуализация дерева
                std::string dotFilename = inputString("Enter DOT filename (e.g., tree.dot): ");
                std::string outputImageFilename = inputString("Enter output image filename (e.g., tree.png): ");
                tree.visualize(dotFilename, outputImageFilename);
                break;
            }
            case 0:{
                if (!allowDuplicates){
                    allowDuplicates = true;
                    std::cout << "Duplicates enabled" << std::endl;
                }else{
                    allowDuplicates = false;
                    std::cout << "Duplicates disabled" << std::endl;
                }
                break;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    } while (choice != 9);
    
    return 0;
}