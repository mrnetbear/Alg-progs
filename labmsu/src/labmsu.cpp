#include <iostream>
#include "connectedSets.hpp"


void printSets(const ConnectedSetsList& setsList) {
    std::cout << "Total sets: " << setsList.count() << std::endl;
    int setNum = 1;
    for (auto it = setsList.begin(); it != setsList.end(); ++it) {
        const ConnectedSet& set = *it;
        std::cout << "Set #" << setNum++ << " (size: " << set.size() << "): ";
        for (auto point : set.getPoints()) {
            std::cout << "(" << point.x << "," << point.y << ") ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    ConnectedSetsList setsList;
    
    // Добавляем несколько точек
    setsList.addPoint(Point(0, 0));
    std::cout << "After adding (0,0):" << std::endl;
    printSets(setsList);
    
    setsList.addPoint(Point(1, 0));
    std::cout << "After adding (1,0):" << std::endl;
    printSets(setsList);
    
    setsList.addPoint(Point(3, 3));
    std::cout << "After adding (3,3):" << std::endl;
    printSets(setsList);
    
    setsList.addPoint(Point(2, 0));
    std::cout << "After adding (2,0):" << std::endl;
    printSets(setsList);
    
    setsList.addPoint(Point(3, 2));
    std::cout << "After adding (3,2):" << std::endl;
    printSets(setsList);
    
    setsList.addPoint(Point(3, 4));
    std::cout << "After adding (3,4):" << std::endl;
    printSets(setsList);
    
    setsList.addPoint(Point(4, 3));
    std::cout << "After adding (4,3):" << std::endl;
    printSets(setsList);

    unsigned int numPoints;
    std::cout << "Enter ammount of points: ";
    std::cin >> numPoints;
    while (numPoints){
        for (size_t i = 0; i < numPoints; ++i){
            int x, y;
            std::cout << "Enter points coordinates (x, y): ";
            std::cin >> x >> y;
            setsList.addPoint(Point(x, y));
        }
        printSets(setsList);
        std::cout << std::endl;
        std::cout << "Enter ammount of points: ";
        std::cin >> numPoints;
    }
    return 0;
}