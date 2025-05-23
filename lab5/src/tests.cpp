#include <gtest/gtest.h>
#include <cstdio> // для std::remove()
#include "graph.hpp"

class GraphTest : public ::testing::Test {
protected:
    std::string testFilename = "test_graph.txt";
    void SetUp() override {
        g = new Graph("test_graph.txt");
        g->addVertex("A", 0.0, 0.0);
        g->addVertex("B", 1.0, 1.0);
    }

    void TearDown() override {
        delete g;
        std::remove(testFilename.c_str());
    }

    Graph* g;
};

// Тесты добавления вершин
TEST_F(GraphTest, AddVertex) {
    EXPECT_TRUE(g->addVertex("C", 2.0, 2.0));
    EXPECT_FALSE(g->addVertex("A", 0.0, 0.0)); // Дубликат
}

// Тесты удаления вершин
TEST_F(GraphTest, RemoveVertex) {
    EXPECT_TRUE(g->removeVertex("A"));
    EXPECT_FALSE(g->removeVertex("D")); // Несуществующая
}

// Тесты добавления ребер
TEST_F(GraphTest, AddEdge) {
    EXPECT_TRUE(g->addEdge("A", "B", 1.5));
    EXPECT_FALSE(g->addEdge("X", "Y", 1.0)); // Несуществующие вершины
}

// Тесты удаления ребер
TEST_F(GraphTest, RemoveEdge) {
    g->addEdge("A", "B", 1.5);
    EXPECT_TRUE(g->removeEdge("A", "B"));
    EXPECT_FALSE(g->removeEdge("A", "C"));
}
// Тесты BFS
TEST_F(GraphTest, BFSPathFinding) {
    g->addVertex("C", 2.0, 2.0);
    g->addEdge("A", "B", 1.0);
    g->addEdge("B", "C", 1.0);

    auto path = g->bfs("A", "C");
    ASSERT_EQ(path.size(), 3);
    EXPECT_EQ(path[0], "A");
    EXPECT_EQ(path[1], "B");
    EXPECT_EQ(path[2], "C");
}

// Тесты Bellman-Ford
TEST_F(GraphTest, BellmanFordShortestPath) {
    g->addVertex("C", 2.0, 2.0);
    g->addEdge("A", "B", 1.0);
    g->addEdge("B", "C", 1.0);
    g->addEdge("A", "C", 5.0);

    auto result = g->bellmanFord("A", "C");
    EXPECT_DOUBLE_EQ(result.second, 2.0); // Кратчайший путь A->B->C
}

// Тесты на ацикличность
TEST_F(GraphTest, IsAcyclic) {
    g->addEdge("A", "B", 1.0);
    EXPECT_TRUE(g->isAcyclic());
    g->addEdge("B", "A", 1.0); // Создаем цикл
    EXPECT_FALSE(g->isAcyclic());
}

TEST_F(GraphTest, TopologicalSort) {
    g->addVertex("C", 2.0, 2.0);
    g->addVertex("D", 3.0, 3.0);
    g->addEdge("A", "B", 1.0);
    g->addEdge("B", "C", 1.0);
    g->addEdge("C", "D", 1.0);

    auto sorted = g->topologicalSort();
    ASSERT_EQ(sorted.size(), 4);
    EXPECT_GT(std::find(sorted.begin(), sorted.end(), "D") - 
              std::find(sorted.begin(), sorted.end(), "A"), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}