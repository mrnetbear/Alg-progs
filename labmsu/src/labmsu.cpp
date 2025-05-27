#include <iostream>
#include "connectedSets.hpp"

using namespace std;

void printSets(const ConnectedSetsList& setsList) {
    cout << "Total sets: " << setsList.count() << endl;
    int setNum = 1;
    for (auto it = setsList.begin(); it != setsList.end(); ++it) {
        const ConnectedSet& set = *it;
        cout << "Set #" << setNum++ << " (size: " << set.size() << "): ";
        for (auto point : set.getPoints()) {
            cout << "(" << point.x << "," << point.y << ") ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    ConnectedSetsList setsList;
    
    // Добавляем несколько точек
    setsList.addPoint(Point(0, 0));
    cout << "After adding (0,0):" << endl;
    printSets(setsList);
    
    setsList.addPoint(Point(1, 0));
    cout << "After adding (1,0):" << endl;
    printSets(setsList);
    
    setsList.addPoint(Point(3, 3));
    cout << "After adding (3,3):" << endl;
    printSets(setsList);
    
    setsList.addPoint(Point(2, 0));
    cout << "After adding (2,0):" << endl;
    printSets(setsList);
    
    setsList.addPoint(Point(3, 2));
    cout << "After adding (3,2):" << endl;
    printSets(setsList);
    
    setsList.addPoint(Point(3, 4));
    cout << "After adding (3,4):" << endl;
    printSets(setsList);
    
    setsList.addPoint(Point(4, 3));
    cout << "After adding (4,3):" << endl;
    printSets(setsList);
    
    return 0;
}