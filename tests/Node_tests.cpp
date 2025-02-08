#include <gtest/gtest.h>
#include "../src/Node.h"
#include <vector>

using std::vector;
using std::unique_ptr;

// Test 1: Node Creation
TEST(LeafNodeTest, NodeCreation) {
    LeafNode node(10);
    std::vector<double> sample = {0, 0, 0, 0, 0};
    EXPECT_EQ(node.predict(sample), 10);
}



// Test 2: Assign Left and Right Child Nodes
TEST(DecisionNodeTest, PredictTest1) {
    auto left = std::make_unique<LeafNode>(3);
    auto right = std::make_unique<LeafNode>(8);

    auto root = std::make_unique<DecisionNode>(0, 3.0, std::move(left), std::move(right));

    vector<double> sample1 = {0, 1.0, 6.2, 0, 0};
    vector<double> sample2 = {5.5, 0, 0, 3.3, -1.4};

    EXPECT_EQ(root->predict(sample1), 3);
    EXPECT_EQ(root->predict(sample2), 8);
}

// Test 3: More Complex Decision Tree Structure
TEST(DecisionNodeTest, PredictTest2) {
    auto left_1 = std::make_unique<LeafNode>(1);
    auto right_1 = std::make_unique<LeafNode>(2);
    auto left_2 = std::make_unique<LeafNode>(3);
    auto right_2 = std::make_unique<LeafNode>(4);

    auto parent_1 = std::make_unique<DecisionNode>(1, 2.0, std::move(left_1), std::move(right_1));
    auto parent_2 = std::make_unique<DecisionNode>(1, 6.0, std::move(left_2), std::move(right_2));
    auto root = std::make_unique<DecisionNode>(0, 3.0, std::move(parent_1), std::move(parent_2));


    vector<double> sample1 = {0, 1.0, 6.2, 0, 0};
    vector<double> sample2 = {0, 3.0, 6.2, 0, 0};
    vector<double> sample3 = {5.5, 5.0, 0, 3.3, -1.4};
    vector<double> sample4 = {5.5, 7.0, 0, 3.3, -1.4};

    EXPECT_EQ(root->predict(sample1), 1);
    EXPECT_EQ(root->predict(sample2), 2);
    EXPECT_EQ(root->predict(sample3), 3);
    EXPECT_EQ(root->predict(sample4), 4);
}


// Test 3: More Complex Decision Tree Structure
TEST(DecisionNodeTest, PrintTest1) {
    auto left_1 = std::make_unique<LeafNode>(1);
    auto right_1 = std::make_unique<LeafNode>(2);
    auto left_2 = std::make_unique<LeafNode>(3);
    auto right_2 = std::make_unique<LeafNode>(4);

    auto parent_1 = std::make_unique<DecisionNode>(1, 2.0, std::move(left_1), std::move(right_1));
    auto parent_2 = std::make_unique<DecisionNode>(1, 6.0, std::move(left_2), std::move(right_2));
    auto root = std::make_unique<DecisionNode>(0, 3.0, std::move(parent_1), std::move(parent_2));

    printf("Decision Tree:\n%s", root->print().c_str());
    EXPECT_EQ(root->print(), "[Feature 0 <= 3.000000]\n\xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 Left: [Feature 1 <= 2.000000]\n\xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 Left: (1.000000)\n\xE2\x94\x94\xE2\x94\x80\xE2\x94\x80 Right: (2.000000)\n\xE2\x94\x94\xE2\x94\x80\xE2\x94\x80 Right: [Feature 1 <= 6.000000]\n\xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 Left: (3.000000)\n\xE2\x94\x94\xE2\x94\x80\xE2\x94\x80 Right: (4.000000)");
}


// Test 3: More Complex Decision Tree Structure
TEST(DecisionNodeTest, AccessorMutatorTest1) {

    std::vector<double> sample = {0, 1.0, 6.2, 0, 0};

    auto left_1 = std::make_unique<LeafNode>(1);
    left_1->set_value(10);
    EXPECT_EQ(left_1->predict(sample), 10);
    
    auto right_1 = std::make_unique<LeafNode>(2);
    auto left_2 = std::make_unique<LeafNode>(3);
    auto right_2 = std::make_unique<LeafNode>(4);

    auto parent_1 = std::make_unique<DecisionNode>(1, 2.0, std::move(left_1), std::move(right_1));
    auto parent_2 = std::make_unique<DecisionNode>(1, 6.0, std::move(left_2), std::move(right_2));
    
    auto root = std::make_unique<DecisionNode>(0, 3.0, std::move(parent_1), std::move(parent_2));

    EXPECT_EQ(root->predict(sample), 10);
    root->set_feature_index(2);
    EXPECT_EQ(root->get_feature_index(), 2);
    EXPECT_EQ(root->predict(sample), 3);
}





int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}