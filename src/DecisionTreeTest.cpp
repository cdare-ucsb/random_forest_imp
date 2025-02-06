#include <memory>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>  // For std::setw

#include "Node.h"
#include "DataFrame.h"
#include "DecisionTree.h"

using std::vector;
using std::string; 



int main() {
    // Level 2: Leaf Nodes
    auto leaf_1 = std::make_unique<LeafNode>(1.0);
    auto leaf_2 = std::make_unique<LeafNode>(2.0);
    auto leaf_3 = std::make_unique<LeafNode>(3.0);
    auto leaf_4 = std::make_unique<LeafNode>(4.0);

    // Level 1: Decision Nodes (splitting at feature 1)
    auto left_child = std::make_unique<DecisionNode>(1, 2.5, std::move(leaf_1), std::move(leaf_2));
    auto right_child = std::make_unique<DecisionNode>(1, 5.0, std::move(leaf_3), std::move(leaf_4));

    // Level 0 (Root): Decision Node (splitting at feature 0)
    auto root = std::make_unique<DecisionNode>(0, 3.0, std::move(left_child), std::move(right_child));

    // Print the tree structure
    std::cout << "Decision Tree Structure:\n" << root->print() << std::endl;

    // Test predictions
    std::vector<double> sample1 = {2.0, 2.0};  // Goes to left->leaf_1 (1.0)
    std::vector<double> sample2 = {2.0, 3.0};  // Goes to left->leaf_2 (2.0)
    std::vector<double> sample3 = {4.0, 4.0};  // Goes to right->leaf_3 (3.0)
    std::vector<double> sample4 = {4.0, 6.0};  // Goes to right->leaf_4 (4.0)

    std::cout << "Prediction for sample1: " << root->predict(sample1) << std::endl;
    std::cout << "Prediction for sample2: " << root->predict(sample2) << std::endl;
    std::cout << "Prediction for sample3: " << root->predict(sample3) << std::endl;
    std::cout << "Prediction for sample4: " << root->predict(sample4) << std::endl;


    // Create a DataFrame

    vector<vector<double>> data = {
        {2.5, 1.5, 0},
        {1.0, 3.0, 1},
        {3.5, 2.0, 0},
        {4.0, 3.5, 1},
        {5.0, 2.5, 1}
    };
    vector<string> columns = {"feature1", "feature2", "label"};
    DataFrame df(data, columns);

    std::cout << "DataFrame:\n" << df.print() << std::endl;

    // Train Decision Tree
    DecisionTree tree;
    tree.fit(&df, "label", 3, 1);

    // Print the trained tree structure
    std::cout << "Trained Decision Tree:\n" << tree.print(columns) << std::endl;

    // Predict new samples
    vector<double> sample5 = {3.0, 2.0};
    vector<double> sample6 = {4.5, 3.0};

    std::cout << "Prediction for sample1: " << tree.predict(sample5) << std::endl;
    std::cout << "Prediction for sample2: " << tree.predict(sample6) << std::endl;

    return 0;
}
