#include "connectedSets.hpp"
#include <cassert>
#include <iostream>

void testPoint() {
    Point p1(0, 0);
    Point p2(1, 0);
    Point p3(2, 0);
    Point p4(0, 1);
    Point p5(10, 10);
    
    assert(p1.isNear(p2) == true);
    assert(p1.isNear(p3) == false);
    assert(p1.isNear(p4) == true);
    assert(p1.isNear(p5) == false);
    assert(p1 == Point(0, 0));
    assert((p1 == p2) == false);
    
    std::cout << "Point tests passed!\n";
}

void testPointList() {
    PointList list;
    assert(list.size() == 0);
    
    list.push_back(Point(0, 0));
    assert(list.size() == 1);
    assert(list.contains(Point(0, 0)));
    assert(!list.contains(Point(1, 1)));
    
    list.push_back(Point(1, 0));
    assert(list.size() == 2);
    assert(list.isNear(Point(0, 1))); // (0,0) is near (0,1)
    assert(!list.isNear(Point(2, 2)));
    
    PointList list2;
    list2.push_back(Point(2, 0));
    list2.push_back(Point(2, 1));
    list.merge(list2);
    assert(list.size() == 4);
    assert(list.contains(Point(2, 0)));
    
    std::cout << "PointList tests passed!\n";
}

void testConnectedSet() {
    ConnectedSet set;
    set.addPoint(Point(0, 0));
    assert(set.size() == 1);
    
    set.addPoint(Point(1, 0));
    assert(set.size() == 2);
    assert(set.contains(Point(0, 0)));
    assert(set.isNear(Point(0, 1))); // (0,0) is near (0,1)
    
    ConnectedSet set2;
    set2.addPoint(Point(2, 0));
    set2.addPoint(Point(2, 1));
    set.merge(set2);
    assert(set.size() == 4);
    assert(set.contains(Point(2, 1)));
    
    std::cout << "ConnectedSet tests passed!\n";
}

void testConnectedSetsList() {
    ConnectedSetsList sets;
    
    // Добавление первой точки - новое множество
    sets.addPoint(Point(0, 0));
    assert(sets.count() == 1);
    assert(sets.begin()->size() == 1);
    std::cout << "Done! 1" << std::endl;
    
    // Добавление соседней точки - в то же множество
    sets.addPoint(Point(1, 0));
    assert(sets.count() == 1);
    assert(sets.begin()->size() == 2);
    std::cout << "Done! 2" << std::endl;
    
    // Добавление изолированной точки - новое множество
    sets.addPoint(Point(10, 10));
    assert(sets.count() == 2);
    std::cout << "Done! 3" << std::endl;
    
    // Добавление точки, объединяющей два множества
    sets.addPoint(Point(2, 0));
    sets.addPoint(Point(9, 10)); // Пока не связывает
    sets.addPoint(Point(10, 9)); // Теперь свяжет (10,10) и (9,10)
    assert(sets.count() == 2);
    std::cout << "Done! 4" << std::endl;
    
    // Проверка отсутствия дубликатов
    sets.addPoint(Point(0, 0));
    assert(sets.count() == 2);
    
    std::cout << "ConnectedSetsList tests passed!\n";
}

void testEdgeCases() {
    ConnectedSetsList sets;
    
    // Добавление одной точки
    sets.addPoint(Point(0, 0));
    
    // Добавление той же точки
    sets.addPoint(Point(0, 0));
    assert(sets.count() == 1);
    assert(sets.begin()->size() == 1);
    
    // Очистка
    sets.clear();
    assert(sets.count() == 0);
    
    // Добавление после очистки
    sets.addPoint(Point(1, 1));
    assert(sets.count() == 1);
    
    std::cout << "Edge cases tests passed!\n";
}

int main() {
    testPoint();
    testPointList();
    testConnectedSet();
    testConnectedSetsList();
    testEdgeCases();
    
    std::cout << "All tests passed successfully!\n";
    return 0;
}