#ifndef LLRB_TREE_H
#define LLRB_TREE_H

#include <iostream>
#include <vector>
#include <string>

typedef unsigned int Key;

// Структура для информационного поля
struct Info {
    float num1;
    float num2;
    std::string str;

    Info(float n1 = 0, float n2 = 0, const std::string& s = "") : num1(n1), num2(n2), str(s) {}
};

// Узел LLRB дерева
enum Color { RED, BLACK };

class LLRBNode {
public:
    Key key;
    Info* info;
    LLRBNode* left;
    LLRBNode* right;
    Color color;
    int size;

    LLRBNode(Key k, Info* i, Color c = RED, LLRBNode* l = nullptr, LLRBNode* r = nullptr)
        : key(k), info(i), left(l), right(r), color(c), size(1) {}

    ~LLRBNode() {
        delete info;
    }
};

// Класс LLRB дерева
class LLRBTree {
private:
    LLRBNode* root;

    // Вспомогательные функции
    bool isRed(LLRBNode* node) const;
    int size(LLRBNode* node) const;
    LLRBNode* rotateLeft(LLRBNode* h);
    LLRBNode* rotateRight(LLRBNode* h);
    void flipColors(LLRBNode* h);
    LLRBNode* moveRedLeft(LLRBNode* h);
    LLRBNode* moveRedRight(LLRBNode* h);
    LLRBNode* balance(LLRBNode* h);
    LLRBNode* min(LLRBNode* h);
    LLRBNode* deleteMin(LLRBNode* h);
    LLRBNode* insert(LLRBNode* h, Key key, Info* info, Info*& oldInfo);
    LLRBNode* remove(LLRBNode* h, Key key, Info*& deletedInfo);
    LLRBNode* get(LLRBNode* h, Key key) const;
    void rangeQuery(LLRBNode* h, Key a, Key b, std::vector<LLRBNode*>& result, bool reverse) const;
    void printTree(LLRBNode* h, int level = 0) const;
    void clear(LLRBNode* h);
    LLRBNode* findMostDifferent(LLRBNode* h, Key key, LLRBNode* currentBest) const;
    void inOrderTraversal(LLRBNode* h, std::vector<LLRBNode*>& nodes) const;

public:
    LLRBTree();
    ~LLRBTree();

    // Основные операции
    Info* insert(Key key, Info* info);
    Info* remove(Key key);
    Info* get(Key key) const;
    std::vector<Info*> rangeQuery(Key a, Key b, bool reverse = false) const;
    Info* findMostDifferent(Key key) const;
    void printTree() const;
    void clear();
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
    void generateDotFile(const std::string& filename) const;
    void visualize(const std::string& dotFilename, const std::string& outputImageFilename);
};

#endif // LLRB_TREE_H