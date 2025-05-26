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
};

#endif //CONNECTED_SETS_H