#include "llrb.hpp"
#include <fstream>
#include <algorithm>
#include <limits>

bool LLRBTree::isRed(LLRBNode* node) const {
    if (!node) return false;
    return node->color == RED;
}

int LLRBTree::size(LLRBNode* node) const {
    if (!node) return 0;
    return node->size;
}

LLRBNode* LLRBTree::rotateLeft(LLRBNode* h) {
    LLRBNode* x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = h->color;
    h->color = RED;
    x->size = h->size;
    h->size = 1 + size(h->left) + size(h->right);
    return x;
}

LLRBNode* LLRBTree::rotateRight(LLRBNode* h) {
    LLRBNode* x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = h->color;
    h->color = RED;
    x->size = h->size;
    h->size = 1 + size(h->left) + size(h->right);
    return x;
}

void LLRBTree::flipColors(LLRBNode* h) {
    h->color = h->color == RED ? BLACK : RED;
    h->left->color = h->left->color == RED ? BLACK : RED;
    h->right->color = h->right->color == RED ? BLACK : RED;
}

LLRBNode* LLRBTree::moveRedLeft(LLRBNode* h) {
    flipColors(h);
    if (isRed(h->right->left)) {
        h->right = rotateRight(h->right);
        h = rotateLeft(h);
        flipColors(h);
    }
    return h;
}

LLRBNode* LLRBTree::moveRedRight(LLRBNode* h) {
    flipColors(h);
    if (isRed(h->left->left)) {
        h = rotateRight(h);
        flipColors(h);
    }
    return h;
}

LLRBNode* LLRBTree::balance(LLRBNode* h) {
    if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);
    if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
    if (isRed(h->left) && isRed(h->right)) flipColors(h);

    h->size = 1 + size(h->left) + size(h->right);
    return h;
}

LLRBNode* LLRBTree::min(LLRBNode* h) {
    if (!h->left) return h;
    return min(h->left);
}

LLRBNode* LLRBTree::deleteMin(LLRBNode* h) {
    if (!h->left) {
        delete h;
        return nullptr;
    }

    if (!isRed(h->left) && !isRed(h->left->left))
        h = moveRedLeft(h);

    h->left = deleteMin(h->left);
    return balance(h);
}

LLRBNode* LLRBTree::insert(LLRBNode* h, Key key, Info* info, Info*& oldInfo) {
    if (!h) {
        return new LLRBNode(key, info);
    }

    if (key == h->key) {
        oldInfo = h->info;
        h->info = info;
        return h;
    }

    if (key < h->key) {
        h->left = insert(h->left, key, info, oldInfo);
    } else {
        h->right = insert(h->right, key, info, oldInfo);
    }

    return balance(h);
}

LLRBNode* LLRBTree::remove(LLRBNode* h, Key key, Info*& deletedInfo) {
    if (key < h->key) {
        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h);
        h->left = remove(h->left, key, deletedInfo);
    } else {
        if (isRed(h->left))
            h = rotateRight(h);

        if (key == h->key && !h->right) {
            deletedInfo = h->info;
            h->info = nullptr;  // Предотвращаем двойное удаление
            delete h;
            return nullptr;
        }

        if (!isRed(h->right) && !isRed(h->right->left))
            h = moveRedRight(h);

        if (key == h->key) {
            LLRBNode* m = min(h->right);
            
            // Переносим данные, а не просто указатели
            deletedInfo = h->info;
            h->key = m->key;
            h->info = new Info(*m->info);  // Создаем копию информации
            
            h->right = deleteMin(h->right);
        } else {
            h->right = remove(h->right, key, deletedInfo);
        }
    }
    return balance(h);
}

LLRBNode* LLRBTree::get(LLRBNode* h, Key key) const {
    if (!h) return nullptr;
    if (key == h->key) return h;
    if (key < h->key) return get(h->left, key);
    return get(h->right, key);
}

void LLRBTree::rangeQuery(LLRBNode* h, Key a, Key b, std::vector<LLRBNode*>& result, bool reverse) const {
    if (!h) return;

    if (reverse) {
        if (h->key < b) rangeQuery(h->right, a, b, result, reverse);
        if (h->key > a && h->key < b) result.push_back(h);
        if (h->key > a) rangeQuery(h->left, a, b, result, reverse);
    } else {
        if (h->key > a) rangeQuery(h->left, a, b, result, reverse);
        if (h->key > a && h->key < b) result.push_back(h);
        if (h->key < b) rangeQuery(h->right, a, b, result, reverse);
    }
}

void LLRBTree::printTree(LLRBNode* h, int level) const {
    if (!h) return;

    printTree(h->right, level + 1);
    std::cout << std::string(level * 4, ' ') << (h->color == RED ? "R " : "B ") << h->key << ": " 
              << h->info->num1 << ", " << h->info->num2 << ", \"" << h->info->str << "\"" << std::endl;
    printTree(h->left, level + 1);
}

void LLRBTree::clear(LLRBNode* h) {
    if (!h) return;
    clear(h->left);
    clear(h->right);
    delete h;
}

LLRBNode* LLRBTree::findMostDifferent(LLRBNode* h, Key key, LLRBNode* currentBest) const {
    if (!h) return currentBest;

    if (!currentBest) {
        currentBest = h;
    } else {
        unsigned long diffCurrent = (Key)abs((int)currentBest->key - (int)key);
        unsigned long diffNew = (Key)abs((int)h->key - (int)key);
        if (diffNew > diffCurrent) {
            currentBest = h;
        }
    }

    LLRBNode* leftBest = findMostDifferent(h->left, key, currentBest);
    LLRBNode* rightBest = findMostDifferent(h->right, key, currentBest);

    unsigned long diffLeft = leftBest ? (Key)abs((int)leftBest->key - (int)key) : 0;
    unsigned long diffRight = rightBest ? (Key)abs((int)rightBest->key - (int)key) : 0;
    unsigned long diffCurrent = (Key)abs((int)currentBest->key - (int)key);

    if (diffLeft > diffCurrent) currentBest = leftBest;
    if (diffRight > diffCurrent) currentBest = rightBest;

    return currentBest;
}

void LLRBTree::inOrderTraversal(LLRBNode* h, std::vector<LLRBNode*>& nodes) const {
    if (!h) return;
    inOrderTraversal(h->left, nodes);
    nodes.push_back(h);
    inOrderTraversal(h->right, nodes);
}

// Реализация публичных методов
LLRBTree::LLRBTree() : root(nullptr) {}

LLRBTree::~LLRBTree() { /*clear();*/ }

Info* LLRBTree::insert(Key key, Info* info) {
    Info* oldInfo = nullptr;
    root = insert(root, key, info, oldInfo);
    root->color = BLACK;
    return oldInfo;
}

Info* LLRBTree::remove(Key key) {
    if (!root) return nullptr;

    if (!isRed(root->left) && !isRed(root->right))
        root->color = RED;

    Info* deletedInfo = nullptr;
    root = remove(root, key, deletedInfo);
    if (root) root->color = BLACK;
    return deletedInfo;
}

Info* LLRBTree::get(Key key) const {
    LLRBNode* node = get(root, key);
    return node ? node->info : nullptr;
}

std::vector<Info*> LLRBTree::rangeQuery(Key a, Key b, bool reverse) const {
    std::vector<LLRBNode*> nodes;
    std::vector<Info*> result;
    
    if (a > b) {
        std::swap(a, b);
        reverse = !reverse;
    }

    rangeQuery(root, a, b, nodes, reverse);
    
    for (auto node : nodes) {
        result.push_back(node->info);
    }
    
    return result;
}

Info* LLRBTree::findMostDifferent(Key key) const {
    LLRBNode* node = findMostDifferent(root, key, nullptr);
    return node ? node->info : nullptr;
}

void LLRBTree::printTree() const {
    std::cout << "Tree structure:" << std::endl;
    printTree(root);
}

void LLRBTree::clear() {
    clear(root);
    root = nullptr;
}

void LLRBTree::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    clear();

    Key key;
    float num1, num2;
    std::string str;

    while (file >> key >> num1 >> num2) {
        file.ignore(); // Пропустить пробел перед строкой
        std::getline(file, str);
        insert(key, new Info(num1, num2, str));
    }

    file.close();
}

void LLRBTree::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::vector<LLRBNode*> nodes;
    inOrderTraversal(root, nodes);

    for (auto node : nodes) {
        file << node->key << " " << node->info->num1 << " " << node->info->num2 << " " << node->info->str << std::endl;
    }

    file.close();
}

void LLRBTree::generateDotFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    out << "digraph LLRBTree {\n";
    out << "  node [shape=circle, fontname=\"Arial\"];\n";
    out << "  edge [arrowhead=normal];\n\n";

    // Очередь для обхода в ширину
    std::queue<LLRBNode*> q;
    if (root) q.push(root);

    while (!q.empty()) {
        LLRBNode* current = q.front();
        q.pop();

        // Определяем цвет узла
        std::string nodeColor = current->color == RED ? "red" : "black";
        std::string fontColor = current->color == RED ? "white" : "white";
        
        // Добавляем узел в граф
        out << "  " << current->key << " [label=\"" << current->key 
            << "\\n" << current->info->num1 << "," << current->info->num2
            << "\", style=filled, fillcolor=" << nodeColor 
            << ", fontcolor=" << fontColor << "];\n";

        // Обрабатываем левого потомка
        if (current->left) {
            out << "  " << current->key << " -> " << current->left->key;
            out << " [label=\"L\"];\n";
            q.push(current->left);
        } else {
            // Добавляем невидимый узел для сохранения баланса в визуализации
            out << "  null" << current->key << "L [shape=point];\n";
            out << "  " << current->key << " -> null" << current->key << "L [color=gray];\n";
        }

        // Обрабатываем правого потомка
        if (current->right) {
            out << "  " << current->key << " -> " << current->right->key;
            out << " [label=\"R\"];\n";
            q.push(current->right);
        } else {
            // Добавляем невидимый узел
            out << "  null" << current->key << "R [shape=point];\n";
            out << "  " << current->key << " -> null" << current->key << "R [color=gray];\n";
        }
    }

    out << "}\n";
    out.close();
    
    std::cout << "Graphviz file generated: " << filename << std::endl;
    //std::cout << "To generate image, run: dot -Tpng " << filename << " -o tree.png\n";
}

void LLRBTree::visualize(const std::string& dotFilename, const std::string& outputImageFilename) {
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