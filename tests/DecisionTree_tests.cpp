#include <gtest/gtest.h>
#include "../src/DataFrame.h"
#include "../src/DecisionTree.h"
#include "../src/Node.h"
#include <vector>

using std::vector;


// Test 1: DecisionTree creation
TEST(DecisionTreeTest, DecisionTreeCreation) {

    DecisionTree dt;

    // Check if the data is correctly stored
    EXPECT_EQ(dt.get_num_nodes(), 0);
}

// Test 2: DecisionTree num_nodes method
TEST(DecisionTreeTest, DecisionTreeNumNodes) {

    DecisionTree dt;

    // Check if the data is correctly stored
    EXPECT_EQ(dt.get_num_nodes(), 0);


    // Create a simple decision tree
    DataFrame df({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {"A", "B", "C"});
    dt.fit(&df, "C", 5, 1);
    std::string tree_str = dt.print({"A", "B", "C"});
    printf("%s\n", tree_str.c_str());
    printf("%s\n", dt.root->print({"A", "B", "C"}).c_str());
    printf("%d\n", dt.root->left == nullptr ? 1 : 0);
    printf("%d\n", dt.root->right == nullptr ? 1 : 0);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}