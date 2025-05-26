#ifndef CONNECTED_SETS_H
#define CONNECTED_SETS_H

#include <vector>
#include <iostream>
#include <utility>

struct Point{
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool isNear(const Point& other) const;
    bool operator==(const Point& other) const;
};

struct PointNode{
    Point data;
    PointNode* prev;
    PointNode* next;

    PointNode(const Point& p, PointNode* prev = nullptr, PointNode* next = nullptr)
    : data(p), prev(prev), next(next) {}
};

class PointList {
    PointNode* head;
    PointNode* tail;
    size_t size_;

public:
    PointList() : head(nullptr), tail(nullptr), size_(0) {}
    
    ~PointList() {
        clear();
    }

    void push_back(const Point& p);
    bool contains(const Point& p) const;
    bool isNear(const Point& p) const;
    void merge(const PointList& other);
    void clear();
    size_t size() const;
};

class Iterator {
    PointNode* current;

public: 
    Iterator(PointNode* node) : current(node) {}
    Point& operator*() { return current->data; }
    Iterator& operator++() { current = current->next; return *this; }
    bool operator!=(const Iterator& other) { return current != other.current; }

    Iterator begin() { return Iterator(current); }
    Iterator end() { return Iterator(nullptr); }
};

#endif //CONNECTED_SETS_H