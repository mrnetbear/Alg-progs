#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

int calculateDiameter(const std::vector<std::vector<int>>& edges, int numComputers) {
    // Создаем список смежности
    std::vector<std::vector<int>> adj(numComputers);
    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        adj[u].push_back(v);
        adj[v].push_back(u); //если подразумевается в этой задаче ориентированность, нужно убрать 
    }

    int diameter = 0;

    // Для каждого узла выполняем BFS
    for (int start = 0; start < numComputers; ++start) {
        std::vector<int> distances(numComputers, -1);
        std::queue<int> q;
        q.push(start);
        distances[start] = 0;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbor : adj[current]) {
                if (distances[neighbor] == -1) {
                    distances[neighbor] = distances[current] + 1;
                    q.push(neighbor);
                }
            }
        }

        // Находим максимальное расстояние для текущего стартового узла
        int max_dist = *max_element(distances.begin(), distances.end());
        if (max_dist > diameter) {
            diameter = max_dist;
        }
    }

    return diameter;
}

int main() {
    std::vector<std::vector<int>> connections = {
        {39, 98}, {56, 15}, {98, 52}, {8, 29}, {17, 49}, {36, 55}, {66, 16}, {72, 87}, {65, 98}, {5, 3}, 
        {99, 90}, {42, 55}, {69, 60}, {27, 92}, {76, 31}, {35, 92}, {45, 64}, {12, 47}, {9, 69}, {48, 23}, 
        {46, 62}, {78, 39}, {9, 13}, {93, 73}, {94, 19}, {62, 17}, {4, 40}, {97, 84}, {57, 81}, {12, 31}, 
        {74, 79}, {43, 23}, {77, 64}, {89, 82}, {7, 99}, {22, 28}, {92, 10}, {72, 5}, {81, 50}, {14, 75}, 
        {25, 19}, {61, 30}, {97, 81}, {63, 54}, {91, 4}, {71, 5}, {0, 75}, {24, 42}, {70, 6}, {88, 95}, 
        {36, 41}, {18, 53}, {85, 21}, {47, 82}, {83, 85}, {58, 88}, {96, 99}, {74, 11}, {67, 10}, {28, 51}, 
        {61, 68}, {70, 63}, {7, 80}, {93, 66}, {17, 16}, {79, 86}, {20, 37}, {70, 33}, {44, 15}, {20, 1}, 
        {96, 59}, {88, 11}, {60, 81}, {35, 8}, {87, 56}, {83, 82}, {77, 40}, {37, 34}, {75, 38}, {63, 77}, 
        {56, 11}, {23, 66}, {5, 2}, {33, 25}, {25, 82}, {53, 29}, {8, 36}, {2, 68}, {22, 40}, {98, 26}, 
        {35, 63}, {12, 13}, {1, 14}, {32, 65}, {98, 75}, {79, 69}, {50, 98}, {45, 73}, {62, 59}
    };
    
    const int numComputers = 100;

    int diameter = calculateDiameter(connections, numComputers);
    std::cout << "Diameter of the net is: " << diameter << std::endl;
    
    return 0;
}