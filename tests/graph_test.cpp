#include <gtest/gtest.h>
#include "graph.h"

TEST(GraphTest, AddNode) {
    Graph<int> g(2);
    g.setNodeData(0, 1);
    g.setNodeData(1, 2);
    EXPECT_EQ(g.getNodeData(0), 1);
    EXPECT_EQ(g.getNodeData(1), 2);
}

TEST(GraphTest, ConnectNodes) {
    Graph<int> g(2);
    g.setNodeData(0, 1);
    g.setNodeData(1, 2);
    g.connectNodes(1, 2);
    EXPECT_EQ(g.getNodeNeighbors(0).size(), 1);
    EXPECT_EQ(g.getNodeNeighbors(0)[0], 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
