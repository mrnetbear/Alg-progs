#ifndef CONNECTED_SETS_HPP
#define CONNECTED_SETS_HPP

#include <vector>
#include <utility>
#include <iostream>

struct Point {
    int x, y;
    Point(int x = 0, int y = 0);
    
    bool isNear(const Point& other) const;
    bool operator==(const Point& other) const;
};

// Узел двусвязного списка точек
struct PointNode {
    Point data;
    PointNode* prev;
    PointNode* next;
    
    PointNode(const Point& p, PointNode* prev = nullptr, PointNode* next = nullptr);
};

// Двусвязный список точек
class PointList {
private:
    PointNode* head;
    PointNode* tail;
    size_t size_;
    
public:
    PointList();
    ~PointList();
    
    void push_back(const Point& p);
    bool contains(const Point& p) const;
    bool isNear(const Point& p) const;
    void merge(const PointList& other);
    void clear();
    size_t size() const;
    
    class Iterator {
    private:
        PointNode* current;
    
    public:
        Iterator(PointNode* node = nullptr);
        Point& operator*() const;
        Point* operator->() const;
        Iterator& operator++();
        Iterator operator++(int);
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };
    
    Iterator begin() const;
    Iterator end() const;
};

class ConnectedSet {
private:
    PointList points;
    
public:
    void addPoint(const Point& p);
    bool contains(const Point& p) const;
    bool isNear(const Point& p) const;
    void merge(const ConnectedSet& other);
    const PointList& getPoints() const;
    size_t size() const;
};

// Узел двусвязного списка множеств
struct SetNode {
    ConnectedSet data;
    SetNode* prev;
    SetNode* next;
    
    SetNode(const ConnectedSet& set, SetNode* prev = nullptr, SetNode* next = nullptr);
};

// Двусвязный список множеств
class ConnectedSetsList {
private:
    SetNode* head;
    SetNode* tail;
    size_t size_;
    
    void removeNode(SetNode* node);
    
public:
    ConnectedSetsList();
    ~ConnectedSetsList();
    
    void addPoint(const Point& p);
    void clear();
    size_t count() const;
    
    class Iterator {
    private:
        SetNode* current;
    
    public:
        Iterator(SetNode* node = nullptr);
        ConnectedSet& operator*() const;
        ConnectedSet* operator->() const;
        Iterator& operator++();
        Iterator operator++(int);
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };
    
    Iterator begin() const;
    Iterator end() const;
};

#endif // CONNECTED_SETS_HPP