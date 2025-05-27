#include "connectedSets.hpp"
#include <algorithm>

// Реализация Point
Point::Point(int x, int y) : x(x), y(y) {}

bool Point::isNear(const Point& other) const {
    return std::max(std::abs(x - other.x), std::abs(y - other.y)) <= 1;
}

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

// Реализация PointNode
PointNode::PointNode(const Point& p, PointNode* prev, PointNode* next) 
    : data(p), prev(prev), next(next) {}

// Реализация PointList
PointList::PointList() : head(nullptr), tail(nullptr), size_(0) {}

PointList::~PointList() {
    clear();
}

void PointList::push_back(const Point& p) {
    PointNode* newNode = new PointNode(p, tail, nullptr);
    if (tail) {
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
    size_++;
}

bool PointList::contains(const Point& p) const {
    PointNode* current = head;
    while (current) {
        if (current->data == p) return true;
        current = current->next;
    }
    return false;
}

bool PointList::isNear(const Point& p) const {
    PointNode* current = head;
    while (current) {
        if (current->data.isNear(p)) return true;
        current = current->next;
    }
    return false;
}

void PointList::merge(const PointList& other) {
    PointNode* current = other.head;
    while (current) {
        push_back(current->data);
        current = current->next;
    }
}

void PointList::clear() {
    PointNode* current = head;
    while (current) {
        PointNode* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size_ = 0;
}

size_t PointList::size() const { return size_; }

// Реализация PointList::Iterator
PointList::Iterator::Iterator(PointNode* node) : current(node) {}

Point& PointList::Iterator::operator*() const { return current->data; }
Point* PointList::Iterator::operator->() const { return &current->data; }

PointList::Iterator& PointList::Iterator::operator++() { 
    if (current) current = current->next; 
    return *this; 
}

PointList::Iterator PointList::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool PointList::Iterator::operator==(const Iterator& other) const { 
    return current == other.current; 
}

bool PointList::Iterator::operator!=(const Iterator& other) const { 
    return current != other.current; 
}

PointList::Iterator PointList::begin() const { return Iterator(head); }
PointList::Iterator PointList::end() const { return Iterator(nullptr); }

// Реализация ConnectedSet
void ConnectedSet::addPoint(const Point& p) {
    points.push_back(p);
}

bool ConnectedSet::contains(const Point& p) const {
    return points.contains(p);
}

bool ConnectedSet::isNear(const Point& p) const {
    return points.isNear(p);
}

void ConnectedSet::merge(const ConnectedSet& other) {
    points.merge(other.points);
}

const PointList& ConnectedSet::getPoints() const {
    return points;
}

size_t ConnectedSet::size() const {
    return points.size();
}

// Реализация SetNode
SetNode::SetNode(const ConnectedSet& set, SetNode* prev, SetNode* next)
    : data(set), prev(prev), next(next) {}

// Реализация ConnectedSetsList
ConnectedSetsList::ConnectedSetsList() : head(nullptr), tail(nullptr), size_(0) {}

ConnectedSetsList::~ConnectedSetsList() {
    clear();
}

void ConnectedSetsList::addPoint(const Point& p) {
    // Проверка на существование точки
    for (SetNode* node = head; node; node = node->next) {
        if (node->data.contains(p)) {
            return;
        }
    }
    
    // Поиск соседних множеств
    std::vector<SetNode*> toMerge;
    SetNode* prevNode = nullptr;
    for (SetNode* node = head; node; node = node->next) {
        if (node->data.isNear(p)) {
            toMerge.push_back(node);
        }
    }
    
    if (toMerge.empty()) {
        // Создаем новое множество
        SetNode* newNode = new SetNode(ConnectedSet(), tail, nullptr);
        newNode->data.addPoint(p);
        if (tail) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
        size_++;
    }
    else {
        // Добавляем точку к первому множеству
        toMerge[0]->data.addPoint(p);
        
        // Объединяем остальные множества
        for (size_t i = 1; i < toMerge.size(); ++i) {
            toMerge[0]->data.merge(toMerge[i]->data);
            removeNode(toMerge[i]);
        }
    }
}

void ConnectedSetsList::removeNode(SetNode* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }
    
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }
    
    delete node;
    size_--;
}

void ConnectedSetsList::clear() {
    SetNode* current = head;
    while (current) {
        SetNode* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size_ = 0;
}

size_t ConnectedSetsList::count() const { return size_; }

// Реализация ConnectedSetsList::Iterator
ConnectedSetsList::Iterator::Iterator(SetNode* node) : current(node) {}

ConnectedSet& ConnectedSetsList::Iterator::operator*() const { return current->data; }
ConnectedSet* ConnectedSetsList::Iterator::operator->() const { return &current->data; }

ConnectedSetsList::Iterator& ConnectedSetsList::Iterator::operator++() { 
    if (current) current = current->next;
    return *this; 
}

ConnectedSetsList::Iterator ConnectedSetsList::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool ConnectedSetsList::Iterator::operator==(const Iterator& other) const { 
    return current == other.current; 
}

bool ConnectedSetsList::Iterator::operator!=(const Iterator& other) const { 
    return current != other.current; 
}

ConnectedSetsList::Iterator ConnectedSetsList::begin() const { return Iterator(head); }
ConnectedSetsList::Iterator ConnectedSetsList::end() const { return Iterator(nullptr); }