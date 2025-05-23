#include "bintree.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>

// Вставка нового элемента
bool BSTree::insert(const std::string& key, int num1, int num2, const std::string& text, bool allowDuplicates) {
    Info* newInfo = new Info(num1, num2, text);
    try {
        //root = insertHelper(root, key, newInfo, allowDuplicates);
        TreeNode* current = root;
        TreeNode* prev = current;
        if (!current)
            root = new TreeNode(key, newInfo);
        while (current){
            prev = current;
            if (key < current->key){
                current = current->left;
                if(!current)
                   prev -> left = new TreeNode(key, newInfo);
            }
            else if (key > current->key){
                current = current->right;
                if (!current) 
                    prev->right = new TreeNode(key, newInfo);
            }
            else {
                // Ключ уже существует
                if (!allowDuplicates) {
                    throw std::runtime_error("Duplicate key not allowed");
                }
                current->infoList.push_back(newInfo);
            }
        }
        
        return true;
    } catch (const std::runtime_error& e) {
        delete newInfo;
        return false;
    }
}

// Рекурсивная функция вставки ПОДПРАВИТЬ НА ЦИКЛ
TreeNode* BSTree::insertHelper(TreeNode* node, const std::string& key, Info* info, bool allowDuplicates) {
    if (!node) {
        return new TreeNode(key, info);
    }
    
    if (key < node->key) {
        node->left = insertHelper(node->left, key, info, allowDuplicates);
    } else if (key > node->key) {
        node->right = insertHelper(node->right, key, info, allowDuplicates);
    } else {
        // Ключ уже существует
        if (!allowDuplicates) {
            throw std::runtime_error("Duplicate key not allowed");
        }
        node->infoList.push_back(info);
    }
    return node;
}

// Удаление элемента по ключу и индексу
bool BSTree::remove(const std::string& key, int index) {
    bool found = false;
    //root = removeHelper(root, key, index, found);
    if (!root || index < 0) return found;

    TreeNode* current = root;
    TreeNode* prev = current;
    bool prevSide = false;

    while (current) {
        prev = current;
        if (key < current -> key){
            current = current->left;
            if (!current) break;
            prevSide = false;
        }
        else if (key > current-> key){
            current = current->right;
            if (!current) break;
            prevSide = true;
        }
        else{
            found = true;
            // Удаляем информацию по индексу
            delete current->infoList[index];
            current->infoList.erase(current->infoList.begin() + index);
            
            // Если больше информации нет, удаляем узел
            if (current->infoList.empty()) {
                if (!current->left) {
                    //TreeNode* rightChild = current->right;
                    if(prevSide) prev->right = current->right;
                    else prev->left = current->right;
                    delete current;
                } else if (!current->right) {
                    if(prevSide) prev->right = current->left;
                    else prev->left = current->left;
                    delete current;
                } else {
                    // У узла есть оба поддерева
                    TreeNode* minRight = current->right;
                    TreeNode* parent = current;
                    bool isLeft = false;
                    
                    while (minRight->left) {
                        parent = minRight;
                        minRight = minRight->left;
                        isLeft = true;
                    }
                    
                    // Копируем данные
                    current->key = minRight->key;
                    current->infoList = minRight->infoList;
                    
                    // Удаляем minRight
                    if (isLeft) {
                        parent->left = minRight->right;
                    } else {
                        parent->right = minRight->right;
                    }
                    
                    minRight->infoList.clear(); // Защита от двойного удаления
                    delete minRight;
                }
            }
        }
        
    }

    return found;
}

// Рекурсивная функция удаления
TreeNode* BSTree::removeHelper(TreeNode* node, const std::string& key, int index, bool& found) {
    if (!node) return nullptr;
    
    if (key < node->key) {
        node->left = removeHelper(node->left, key, index, found);
    } else if (key > node->key) {
        node->right = removeHelper(node->right, key, index, found);
    } else {
        // Найден узел с нужным ключом
        if (index < 0 || index >= node->infoList.size()) {
            return node; // Неверный индекс
        }
        
        found = true;
        // Удаляем информацию по индексу
        delete node->infoList[index];
        node->infoList.erase(node->infoList.begin() + index);
        
        // Если больше информации нет, удаляем узел
        if (node->infoList.empty()) {
            if (!node->left) {
                TreeNode* rightChild = node->right;
                delete node;
                return rightChild;
            } else if (!node->right) {
                TreeNode* leftChild = node->left;
                delete node;
                return leftChild;
            } else {
                // У узла есть оба поддерева
                TreeNode* minRight = node->right;
                TreeNode* parent = node;
                bool isLeft = false;
                
                while (minRight->left) {
                    parent = minRight;
                    minRight = minRight->left;
                    isLeft = true;
                }
                
                // Копируем данные
                node->key = minRight->key;
                node->infoList = minRight->infoList;
                
                // Удаляем minRight
                if (isLeft) {
                    parent->left = minRight->right;
                } else {
                    parent->right = minRight->right;
                }
                
                minRight->infoList.clear(); // Защита от двойного удаления
                delete minRight;
            }
        }
    }
    return node;
}

// Поиск информации по ключу и индексу
Info* BSTree::find(const std::string& key, int index) const {
    TreeNode* current = root;
    while (current) {
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            if (index >= 0 && index < current->infoList.size()) {
                return current->infoList[index];
            }
            return nullptr;
        }
    }
    return nullptr;
}

// Вывод в обратном порядке в диапазоне
void BSTree::printReverseRange(const std::string& start, const std::string& end) const {
    printReverseHelper(root, start, end);
    std::cout << std::endl;
}

// Рекурсивная функция вывода в обратном порядке
void BSTree::printReverseHelper(TreeNode* node, const std::string& start, const std::string& end) const {
    if (!node) return;
    
    // Сначала правое поддерево (большие ключи)
    if (node->key <= end) {
        printReverseHelper(node->right, start, end);
    }
    
    // Затем текущий узел, если в диапазоне
    if (node->key >= start && node->key <= end) {
        for (size_t i = 0; i < node->infoList.size(); ++i) {
            const Info* info = node->infoList[i];
            std::cout << "Key: " << node->key << " [" << i << "]: "
                      << info->number1 << ", " << info->number2 << ", \""
                      << info->text << "\"" << std::endl;
        }
    }
    
    // Затем левое поддерево (меньшие ключи)
    if (node->key >= start) {
        printReverseHelper(node->left, start, end);
    }
}

// Поиск минимального ключа, большего заданного
Info* BSTree::findMinGreater(const std::string& key, int index) const {
    TreeNode* result = findMinGreaterHelper(root, key, nullptr);
    if (result && index >= 0 && index < result->infoList.size()) {
        return result->infoList[index];
    }
    return nullptr;
}

// Рекурсивная функция поиска минимального большего ключа
TreeNode* BSTree::findMinGreaterHelper(TreeNode* node, const std::string& key, TreeNode* best) const {
    if (!node) return best;
    
    if (node->key > key) {
        // Текущий узел подходит, ищем может есть меньший, но все еще больший чем key
        best = node;
        return findMinGreaterHelper(node->left, key, best);
    } else {
        // Текущий узел не подходит, идем вправо
        return findMinGreaterHelper(node->right, key, best);
    }
}

// Очистка дерева
void BSTree::clear() {
    clearHelper(root);
    root = nullptr;
}

// Рекурсивная функция очистки
void BSTree::clearHelper(TreeNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
}

// Вывод дерева в виде дерева (для отладки)
void BSTree::printTree() const {
    printTreeHelper(root);
}

// Рекурсивная функция вывода дерева
void BSTree::printTreeHelper(TreeNode* node, int space) const {
    if (!node) return;
    
    space += 5;
    
    printTreeHelper(node->right, space);
    
    std::cout << std::endl;
    for (int i = 5; i < space; i++) {
        std::cout << " ";
    }
    std::cout << node->key << " (" << node->infoList.size() << ")" << std::endl;
    
    printTreeHelper(node->left, space);
}

// Загрузка из файла
bool BSTree::loadFromFile(const std::string& filename, bool allowDuplicates) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    clear();
    
    std::string line;
    while (std::getline(file, line)) {
        // Первая строка - ключ
        std::string key = line;
        
        // Вторая строка - число 1
        if (!std::getline(file, line)) break;
        int num1 = std::stoi(line);
        
        // Третья строка - число 2
        if (!std::getline(file, line)) break;
        int num2 = std::stoi(line);
        
        // Четвертая строка - текст
        if (!std::getline(file, line)) break;
        std::string text = line;
        
        insert(key, num1, num2, text, allowDuplicates);
    }
    
    file.close();
    return true;
}

//Генерация файла для визуализации
void BSTree::generateDotFile(const std::string& filename) const {
    std::ofstream dotFile(filename);
    if (!dotFile.is_open()) {
        std::cerr << "Error opening DOT file for writing.\n";
        return;
    }

    dotFile << "digraph BSTree {\n";
    dotFile << "  node [shape=record, height=0.1];\n";

    // Рекурсивная функция для обхода дерева
    std::function<void(TreeNode*)> traverse = [&](TreeNode* node) {
        if (!node) return;

        // Уникальный идентификатор узла
        std::string nodeId = "node" + std::to_string(reinterpret_cast<uintptr_t>(node));

        // Запись узла
        dotFile << "  " << nodeId << " [label=\"<left> | <key> " << node->key << " (" << node->infoList.size() << ") | <right> \"];\n";

        // Рекурсивный обход левого поддерева
        if (node->left) {
            std::string leftId = "node" + std::to_string(reinterpret_cast<uintptr_t>(node->left));
            dotFile << "  " << nodeId << ":left -> " << leftId << ":key;\n";
            traverse(node->left);
        }

        // Рекурсивный обход правого поддерева
        if (node->right) {
            std::string rightId = "node" + std::to_string(reinterpret_cast<uintptr_t>(node->right));
            dotFile << "  " << nodeId << ":right -> " << rightId << ":key;\n";
            traverse(node->right);
        }
    };

    traverse(root);
    dotFile << "}\n";
    dotFile.close();
}

//Визуализация дерева
void BSTree::visualize(const std::string& dotFilename, const std::string& outputImageFilename) {
    generateDotFile(dotFilename);

    // Вызов Graphviz для генерации изображения
    std::string command = "dot -Tpng " + dotFilename + " -o " + outputImageFilename;
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Error generating tree visualization. Ensure Graphviz is installed.\n";
    } else {
        std::cout << "Tree visualization saved to " << outputImageFilename << "\n";
    }
}