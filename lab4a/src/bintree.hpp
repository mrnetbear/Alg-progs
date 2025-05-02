#ifndef BINTREE_H
#define BINTREE_H

#ifdef HAS_GRAPHVIZ
void generateDotFile(const std::string& filename) const;
void visualize(const std::string& dotFilename, const std::string& outputImageFilename);
#endif

#include <iostream>
#include <string>
#include <vector>

// Структура для хранения информации в узле
struct Info {
    int number1;
    int number2;
    std::string text;
    
    Info(int n1, int n2, const std::string& t) : number1(n1), number2(n2), text(t) {}
};

// Узел дерева
struct TreeNode {
    std::string key;
    std::vector<Info*> infoList; // Список информационных полей для одного ключа
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(const std::string& k, Info* info) : key(k), left(nullptr), right(nullptr) {
        infoList.push_back(info);
    }
    
    ~TreeNode() {
        for (auto info : infoList) {
            delete info;
        }
    }
};

// Класс бинарного дерева поиска
class BSTree {
private:
    TreeNode* root;
    
    // Вспомогательные рекурсивные функции
    TreeNode* insertHelper(TreeNode* node, const std::string& key, Info* info, bool allowDuplicates);
    TreeNode* removeHelper(TreeNode* node, const std::string& key, int index, bool& found);
    void printReverseHelper(TreeNode* node, const std::string& start, const std::string& end) const;
    void printTreeHelper(TreeNode* node, int space = 0) const;
    void clearHelper(TreeNode* node);
    TreeNode* findMinGreaterHelper(TreeNode* node, const std::string& key, TreeNode* best) const;
    
public:
    BSTree() : root(nullptr) {}
    ~BSTree() { clear(); }
    
    // Основные операции
    bool insert(const std::string& key, int num1, int num2, const std::string& text, bool allowDuplicates = false);
    bool remove(const std::string& key, int index = 0);
    Info* find(const std::string& key, int index = 0) const;
    void printReverseRange(const std::string& start, const std::string& end) const;
    Info* findMinGreater(const std::string& key, int index = 0) const;
    
    // Вспомогательные функции
    void clear();
    void printTree() const;
    bool loadFromFile(const std::string& filename, bool allowDuplicates = false);

    //Визуализация
    void generateDotFile(const std::string& filename) const;
    void visualize(const std::string& dotFilename, const std::string& outputImageFilename);
};


#endif //BINTREE_H