#include <gtest/gtest.h>
#include "../src/Node.h"
#include <vector>

// Test 1: Node Creation
TEST(LeafNodeTest, NodeCreation) {
    LeafNode node(10);
    std::vector<double> sample = {0, 0, 0, 0, 0};
    EXPECT_EQ(node.predict(sample), 10);
}



// Test 2: Assign Left and Right Child Nodes
TEST(DecisionNodeTest, PredictTest1) {
    LeafNode* left = new LeafNode(3);
    LeafNode* right = new LeafNode(8);

    DecisionNode* root = new DecisionNode(0, 3.0, left, right);

    std::vector<double> sample1 = {0, 1.0, 6.2, 0, 0};
    std::vector<double> sample2 = {5.5, 0, 0, 3.3, -1.4};

    EXPECT_EQ(root->predict(sample1), 3);
    EXPECT_EQ(root->predict(sample2), 8);

    // Cleanup (avoidls memory leak)
    delete root;
}

// Test 3: More Complex Decision Tree Structure
TEST(DecisionNodeTest, PredictTest2) {
    LeafNode* left_1 = new LeafNode(1);
    LeafNode* right_1 = new LeafNode(2);
    LeafNode* left_2 = new LeafNode(3);
    LeafNode* right_2 = new LeafNode(4);

    DecisionNode* parent_1 = new DecisionNode(1, 2.0, left_1, right_1);
    DecisionNode* parent_2 = new DecisionNode(1, 6.0, left_2, right_2);
    DecisionNode* root = new DecisionNode(0, 3.0, parent_1, parent_2);


    std::vector<double> sample1 = {0, 1.0, 6.2, 0, 0};
    std::vector<double> sample2 = {0, 3.0, 6.2, 0, 0};
    std::vector<double> sample3 = {5.5, 5.0, 0, 3.3, -1.4};
    std::vector<double> sample4 = {5.5, 7.0, 0, 3.3, -1.4};

    EXPECT_EQ(root->predict(sample1), 1);
    EXPECT_EQ(root->predict(sample2), 2);
    EXPECT_EQ(root->predict(sample3), 3);
    EXPECT_EQ(root->predict(sample4), 4);

    // Cleanup (avoidls memory leak)
    delete root;
}


// Test 3: More Complex Decision Tree Structure
TEST(DecisionNodeTest, PrintTest1) {
    LeafNode* left_1 = new LeafNode(1);
    LeafNode* right_1 = new LeafNode(2);
    LeafNode* left_2 = new LeafNode(3);
    LeafNode* right_2 = new LeafNode(4);

    DecisionNode* parent_1 = new DecisionNode(1, 2.0, left_1, right_1);
    DecisionNode* parent_2 = new DecisionNode(1, 6.0, left_2, right_2);
    DecisionNode* root = new DecisionNode(0, 3.0, parent_1, parent_2);


    EXPECT_EQ(left_1->print(), "(1.000000)");
    EXPECT_EQ(parent_1->print(), "[idx:1, thr:2.000000]\nLeft Child: (1.000000)\nRight Child: (2.000000)");

    // Cleanup (avoidls memory leak)
    delete root;
}


// Test 3: More Complex Decision Tree Structure
TEST(DecisionNodeTest, AccessorMutatorTest1) {

    std::vector<double> sample = {0, 1.0, 6.2, 0, 0};

    LeafNode* left_1 = new LeafNode(1);
    LeafNode* right_1 = new LeafNode(2);
    LeafNode* left_2 = new LeafNode(3);
    LeafNode* right_2 = new LeafNode(4);

    DecisionNode* parent_1 = new DecisionNode(1, 2.0, left_1, right_1);
    DecisionNode* parent_2 = new DecisionNode(1, 6.0, left_2, right_2);
    DecisionNode* root = new DecisionNode(0, 3.0, parent_1, parent_2);

    left_1->set_value(10);
    EXPECT_EQ(left_1->predict(sample), 10);
    EXPECT_EQ(root->predict(sample), 10);

    parent_1->set_feature_index(2);
    EXPECT_EQ(root->predict(sample), 2);
    parent_1->set_threshold(7.0);
    EXPECT_EQ(root->predict(sample), 10);


    // Cleanup (avoidls memory leak)
    delete root;
}





int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}