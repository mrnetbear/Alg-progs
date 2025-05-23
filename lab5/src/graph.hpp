#ifndef GRAPH_H
#define GRAPH_H

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

class Vertex {
public:
    std::string name;
    double x, y;

    Vertex() : name(""), x(0), y(0) {}
    Vertex(std::string n, double x_coord, double y_coord) : name(n), x(x_coord), y(y_coord) {}

    //double distanceTo(const Vertex& other) const;
};

class Graph {
private:
    std::unordered_map<std::string, Vertex> vertices;
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> adjacencyList;
    std::string filename;
    
    bool vertexExists(const std::string& name) const;
    bool edgeExists(const std::string& from, const std::string& to) const;
    bool isCyclicUtil(const std::string& v, std::unordered_map<std::string, bool>& visited, 
                     std::unordered_map<std::string, bool>& recursionStack);
    void topologicalSortUtil(const std::string& v, std::unordered_map<std::string, bool>& visited, std::stack<std::string>& s);
public:
    Graph(const std::string& file) : filename(file) {
        loadFromFile();
    }

    ~Graph() {
        saveToFile();
    }
    void loadFromFile();
    void saveToFile() const;
    bool addVertex(const std::string& name, double x, double y);
    bool addEdge(const std::string& from, const std::string& to, double weight);
    bool removeVertex(const std::string& name);
    bool removeEdge(const std::string& from, const std::string& to);
    void printGraph() const;
    std::vector<std::string> bfs(const std::string& start, const std::string& target);
    std::pair<std::vector<std::string>, double> bellmanFord(const std::string& start, const std::string& target);
    bool isAcyclic();
    std::vector<std::string> topologicalSort();
    void generateRandomGraph(int vertexCount, int edgeCount);
    void visualizeGraph(const std::string& outputFilename = "graph.png") const;
};
#endif //GRAPH_H