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
#include <cstdlib>


double Vertex::distanceTo(const Vertex& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx*dx + dy*dy);
    }


bool Graph::vertexExists(const std::string& name) const {
        return vertices.find(name) != vertices.end();
}

bool Graph::edgeExists(const std::string& from, const std::string& to) const {
        if (!vertexExists(from) || !vertexExists(to)) return false;
        for (const auto& neighbor : adjacencyList.at(from)) {
            if (neighbor.first == to) return true;
        }
        return false;
}

    void Graph::loadFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Creating new graph file: " << filename << std::endl;
            return;
        }

        vertices.clear();
        adjacencyList.clear();

        std::string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            if (line[0] == 'V') {
                // Vertex format: V name x y
                std::istringstream iss(line.substr(1));
                std::string name;
                double x, y;
                if (iss >> name >> x >> y) {
                    addVertex(name, x, y);
                }
            } else if (line[0] == 'E') {
                // Edge format: E from to weight
                std::istringstream iss(line.substr(1));
                std::string from, to;
                double weight;
                if (iss >> from >> to >> weight) {
                    addEdge(from, to, weight);
                }
            }
        }
        file.close();
    }

    void Graph::saveToFile() const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error saving graph to file: " << filename << std::endl;
            return;
        }

        // Save vertices
        for (const auto& v : vertices) {
            file << "V " << v.first << " " << v.second.x << " " << v.second.y << "\n";
        }

        // Save edges
        for (const auto& adj : adjacencyList) {
            for (const auto& edge : adj.second) {
                file << "E " << adj.first << " " << edge.first << " " << edge.second << "\n";
            }
        }

        file.close();
    }

    bool Graph::addVertex(const std::string& name, double x, double y) {
        if (vertexExists(name)) {
            std::cerr << "Vertex " << name << " already exists.\n";
            return false;
        }
        vertices[name] = Vertex(name, x, y);
        adjacencyList[name] = std::vector<std::pair<std::string, double>>();
        return true;
    }

    bool Graph::addEdge(const std::string& from, const std::string& to, double weight) {
        if (!vertexExists(from) || !vertexExists(to)) {
            std::cerr << "One or both vertices don't exist.\n";
            return false;
        }
        if (weight < 0) {
            std::cerr << "Weight cannot be negative.\n";
            return false;
        }
        if (edgeExists(from, to)) {
            std::cerr << "Edge already exists.\n";
            return false;
        }

        adjacencyList[from].emplace_back(to, weight);
        return true;
    }

    bool Graph::removeVertex(const std::string& name) {
        if (!vertexExists(name)) {
            std::cerr << "Vertex " << name << " doesn't exist.\n";
            return false;
        }

        // Remove all outgoing edges
        adjacencyList.erase(name);

        // Remove all incoming edges
        for (auto& adj : adjacencyList) {
            auto& edges = adj.second;
            edges.erase(remove_if(edges.begin(), edges.end(), 
                [&name](const std::pair<std::string, double>& edge) { return edge.first == name; }), 
                edges.end());
        }

        // Remove the vertex itself
        vertices.erase(name);
        return true;
    }

    bool Graph::removeEdge(const std::string& from, const std::string& to) {
        if (!edgeExists(from, to)) {
            std::cerr << "Edge doesn't exist.\n";
            return false;
        }

        auto& edges = adjacencyList[from];
        edges.erase(remove_if(edges.begin(), edges.end(), 
            [&to](const std::pair<std::string, double>& edge) { return edge.first == to; }), 
            edges.end());
        return true;
    }

    void Graph::printGraph() const {
        std::cout << "Vertices:\n";
        for (const auto& v : vertices) {
            std::cout << v.first << " (" << v.second.x << ", " << v.second.y << ")\n";
        }

        std::cout << "\nEdges:\n";
        for (const auto& adj : adjacencyList) {
            for (const auto& edge : adj.second) {
                std::cout << adj.first << " -> " << edge.first << " (weight: " << edge.second << ")\n";
            }
        }
    }

    // BFS search - O(V + E)
    std::vector<std::string> Graph::bfs(const std::string& start, const std::string& target) {
        std::vector<std::string> path;
        if (!vertexExists(start) || !vertexExists(target)) {
            std::cerr << "One or both vertices don't exist.\n";
            return path;
        }

        std::unordered_map<std::string, bool> visited;
        std::unordered_map<std::string, std::string> parent;
        std::queue<std::string> q;

        for (const auto& v : vertices) {
            visited[v.first] = false;
        }

        q.push(start);
        visited[start] = true;
        parent[start] = "";

        bool found = false;
        while (!q.empty() && !found) {
            std::string current = q.front();
            q.pop();

            for (const auto& neighbor : adjacencyList[current]) {
                std::string next = neighbor.first;
                if (!visited[next]) {
                    visited[next] = true;
                    parent[next] = current;
                    q.push(next);

                    if (next == target) {
                        found = true;
                        break;
                    }
                }
            }
        }

        if (found) {
            // Reconstruct path
            std::string node = target;
            while (node != "") {
                path.push_back(node);
                node = parent[node];
            }
            reverse(path.begin(), path.end());
        }

        return path;
    }

    // Bellman-Ford algorithm - O(V*E)
    std::pair<std::vector<std::string>, double> Graph::bellmanFord(const std::string& start, const std::string& target) {
        std::vector<std::string> path;
        double distance = -1;

        if (!vertexExists(start) || !vertexExists(target)) {
            std::cerr << "One or both vertices don't exist.\n";
            return {path, distance};
        }

        std::unordered_map<std::string, double> dist;
        std::unordered_map<std::string, std::string> prev;

        // Initialize distances
        for (const auto& v : vertices) {
            dist[v.first] = std::numeric_limits<double>::infinity();
        }
        dist[start] = 0;

        // Relax all edges V-1 times
        for (size_t i = 1; i < vertices.size(); ++i) {
            for (const auto& adj : adjacencyList) {
                std::string u = adj.first;
                for (const auto& edge : adj.second) {
                    std::string v = edge.first;
                    double weight = edge.second;
                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        prev[v] = u;
                    }
                }
            }
        }

        // Check for negative-weight cycles
        for (const auto& adj : adjacencyList) {
            std::string u = adj.first;
            for (const auto& edge : adj.second) {
                std::string v = edge.first;
                double weight = edge.second;
                if (dist[u] + weight < dist[v]) {
                    std::cerr << "Graph contains negative weight cycle\n";
                    return {path, -1};
                }
            }
        }

        // If no path exists
        if (dist[target] == std::numeric_limits<double>::infinity()) {
            return {path, -1};
        }

        // Reconstruct path
        std::string node = target;
        while (node != start) {
            path.push_back(node);
            node = prev[node];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());

        return {path, dist[target]};
    }

    // Check if graph is acyclic using DFS - O(V + E)
    bool Graph::isAcyclic() {
        std::unordered_map<std::string, bool> visited;
        std::unordered_map<std::string, bool> recursionStack;

        for (const auto& v : vertices) {
            visited[v.first] = false;
            recursionStack[v.first] = false;
        }

        for (const auto& v : vertices) {
            if (!visited[v.first] && isCyclicUtil(v.first, visited, recursionStack)) {
                return false;
            }
        }

        return true;
    }

    // Topological sort using DFS - O(V + E)
    std::vector<std::string> Graph::topologicalSort() {
        std::vector<std::string> result;
        if (!isAcyclic()) {
            std::cerr << "Graph is not acyclic, cannot perform topological sort.\n";
            return result;
        }

        std::unordered_map<std::string, bool> visited;
        for (const auto& v : vertices) {
            visited[v.first] = false;
        }

        std::stack<std::string> s;

        for (const auto& v : vertices) {
            if (!visited[v.first]) {
                topologicalSortUtil(v.first, visited, s);
            }
        }

        while (!s.empty()) {
            result.push_back(s.top());
            s.pop();
        }

        return result;
    }

    void Graph::generateRandomGraph(int vertexCount, int edgeCount) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> coordDist(0, 100);
        std::uniform_int_distribution<> weightDist(1, 100);

        // Clear existing graph
        vertices.clear();
        adjacencyList.clear();

        // Generate vertices
        for (int i = 0; i < vertexCount; ++i) {
            std::string name = "V" + std::to_string(i);
            addVertex(name, coordDist(gen), coordDist(gen));
        }

        // Generate edges
        std::vector<std::string> vertexNames;
        for (const auto& v : vertices) {
            vertexNames.push_back(v.first);
        }

        int edgesAdded = 0;
        while (edgesAdded < edgeCount) {
            std::uniform_int_distribution<> idxDist(0, vertexCount - 1);
            int fromIdx = idxDist(gen);
            int toIdx = idxDist(gen);

            if (fromIdx != toIdx && !edgeExists(vertexNames[fromIdx], vertexNames[toIdx])) {
                double weight = weightDist(gen);
                addEdge(vertexNames[fromIdx], vertexNames[toIdx], weight);
                edgesAdded++;
            }
        }
    }

    bool Graph::isCyclicUtil(const std::string& v, std::unordered_map<std::string, bool>& visited, 
                     std::unordered_map<std::string, bool>& recursionStack) {
        if (!visited[v]) {
            visited[v] = true;
            recursionStack[v] = true;

            for (const auto& neighbor : adjacencyList[v]) {
                std::string next = neighbor.first;
                if (!visited[next] && isCyclicUtil(next, visited, recursionStack)) {
                    return true;
                } else if (recursionStack[next]) {
                    return true;
                }
            }
        }
        recursionStack[v] = false;
        return false;
    }

    void Graph::topologicalSortUtil(const std::string& v, std::unordered_map<std::string, bool>& visited, std::stack<std::string>& s) {
        visited[v] = true;

        for (const auto& neighbor : adjacencyList[v]) {
            std::string next = neighbor.first;
            if (!visited[next]) {
                topologicalSortUtil(next, visited, s);
            }
        }

        s.push(v);
    }
    void Graph::visualizeGraph(const std::string& outputFilename) const {
    // Создаём DOT-файл
    std::string dotFilename = "graph.dot";
    std::ofstream dotFile(dotFilename);
    
    if (!dotFile.is_open()) {
        std::cerr << "Error creating DOT file for visualization.\n";
        return;
    }
    
    // Записываем заголовок DOT-файла
    dotFile << "digraph G {\n";
    dotFile << "  rankdir=LR;\n";
    dotFile << "  node [shape=circle];\n\n";
    
    // Добавляем вершины с координатами (для лучшей визуализации)
    for (const auto& v : vertices) {
        dotFile << "  \"" << v.first << "\" [pos=\"" << v.second.x << "," << v.second.y << "!\"];\n";
    }
    dotFile << "\n";
    
    // Добавляем рёбра
    for (const auto& adj : adjacencyList) {
        for (const auto& edge : adj.second) {
            dotFile << "  \"" << adj.first << "\" -> \"" << edge.first << "\" [label=\"" << edge.second << "\"];\n";
        }
    }
    
    dotFile << "}\n";
    dotFile.close();
    
    // Вызываем Graphviz для создания изображения
    std::string command = "dot -Tpng " + dotFilename + " -o " + outputFilename;
    int result = system(command.c_str());
    
    if (result != 0) {
        std::cerr << "Error generating graph visualization. Make sure Graphviz is installed.\n";
    } else {
        std::cout << "Graph visualization saved to " << outputFilename << "\n";
    }
}