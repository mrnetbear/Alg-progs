#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <sstream>
#include <cctype>

void printMenu() {
    std::cout << "\nGraph Operations Menu:\n";
    std::cout << "1. Add vertex\n";
    std::cout << "2. Add edge\n";
    std::cout << "3. Remove vertex\n";
    std::cout << "4. Remove edge\n";
    std::cout << "5. Print graph\n";
    std::cout << "6. Find path (BFS)\n";
    std::cout << "7. Find shortest path (Bellman-Ford)\n";
    std::cout << "8. Check if graph is acyclic\n";
    std::cout << "9. Perform topological sort\n";
    std::cout << "10. Generate random graph\n";
    std::cout << "11. Visualize graph (Graphviz)\n";
    std::cout << "12. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    Graph graph("graph_data.txt");

    int choice;
    do {
        printMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::string name, from, to;
        double x, y, weight;
        int vertexCount, edgeCount;

        switch (choice) {
            case 1: // Add vertex
                std::cout << "Enter vertex name: ";
                std::cin >> name;
                std::cout << "Enter x coordinate: ";
                std::cin >> x;
                std::cout << "Enter y coordinate: ";
                std::cin >> y;
                if (graph.addVertex(name, x, y)) {
                    std::cout << "Vertex added successfully.\n";
                }
                break;

            case 2: // Add edge
                std::cout << "Enter source vertex: ";
                std::cin >> from;
                std::cout << "Enter target vertex: ";
                std::cin >> to;
                std::cout << "Enter edge weight (or -1 to calculate distance): ";
                std::cin >> weight;
                if (weight == -1) {
                    // Calculate weight as distance between vertices
                    // (This would require access to vertex coordinates, which we can't do directly)
                    std::cout << "This feature requires modification to the Graph class.\n";
                    std::cout << "Enter weight manually: ";
                    std::cin >> weight;
                }
                if (graph.addEdge(from, to, weight)) {
                    std::cout << "Edge added successfully.\n";
                }
                break;

            case 3: // Remove vertex
                std::cout << "Enter vertex name to remove: ";
                std::cin >> name;
                if (graph.removeVertex(name)) {
                    std::cout << "Vertex removed successfully.\n";
                }
                break;

            case 4: // Remove edge
                std::cout << "Enter source vertex: ";
                std::cin >> from;
                std::cout << "Enter target vertex: ";
                std::cin >> to;
                if (graph.removeEdge(from, to)) {
                    std::cout << "Edge removed successfully.\n";
                }
                break;

            case 5: // Print graph
                graph.printGraph();
                break;

            case 6: { // BFS path
                std::cout << "Enter start vertex: ";
                std::cin >> from;
                std::cout << "Enter target vertex: ";
                std::cin >> to;
                auto path = graph.bfs(from, to);
                if (path.empty()) {
                    std::cout << "No path exists between " << from << " and " << to << ".\n";
                } else {
                    std::cout << "Path found: ";
                    for (size_t i = 0; i < path.size(); ++i) {
                        if (i != 0) std::cout << " -> ";
                        std::cout << path[i];
                    }
                    std::cout << std::endl;
                }
                break;
            }

            case 7: { // Bellman-Ford shortest path
                std::cout << "Enter start vertex: ";
                std::cin >> from;
                std::cout << "Enter target vertex: ";
                std::cin >> to;
                auto result = graph.bellmanFord(from, to);
                if (result.first.empty()) {
                    std::cout << "No path exists between " << from << " and " << to << ".\n";
                } else {
                    std::cout << "Shortest path (distance: " << result.second << "): ";
                    for (size_t i = 0; i < result.first.size(); ++i) {
                        if (i != 0) std::cout << " -> ";
                        std::cout << result.first[i];
                    }
                    std::cout << std::endl;
                }
                break;
            }

            case 8: // Check acyclic
                if (graph.isAcyclic()) {
                    std::cout << "Graph is acyclic.\n";
                } else {
                    std::cout << "Graph contains cycles.\n";
                }
                break;

            case 9: { // Topological sort
                auto sorted = graph.topologicalSort();
                if (sorted.empty()) {
                    std::cout << "Graph is not acyclic, cannot perform topological sort.\n";
                } else {
                    std::cout << "Topological order: ";
                    for (size_t i = 0; i < sorted.size(); ++i) {
                        if (i != 0) std::cout << " -> ";
                        std::cout << sorted[i];
                    }
                    std::cout << std::endl;
                }
                break;
            }

            case 10: // Generate random graph
                std::cout << "Enter number of vertices: ";
                std::cin >> vertexCount;
                std::cout << "Enter number of edges: ";
                std::cin >> edgeCount;
                if (vertexCount <= 0 || edgeCount < 0 || edgeCount > vertexCount * (vertexCount - 1)) {
                    std::cout << "Invalid parameters.\n";
                } else {
                    graph.generateRandomGraph(vertexCount, edgeCount);
                    std::cout << "Random graph generated with " << vertexCount << " vertices and " << edgeCount << " edges.\n";
                }
                break;

            case 11: { // Visualize graph
                std::string filename;
                std::cout << "Enter output filename (default: graph.png): ";
                std::cin.ignore(); // Очищаем буфер
                std::getline(std::cin, filename);
                if (filename.empty()) {
                    filename = "graph.png";
                }
                graph.visualizeGraph(filename);
                break;
            }
            case 12: // Exit
                std::cout << "Exiting program.\n";
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 12);

    return 0;
}