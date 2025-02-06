#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "DataFrame.h"
#include "Node.h"
#include "DecisionTree.h"

using std::string;
using std::vector;
using std::unique_ptr;


// Helper function for printing the tree recursively
void DecisionTree::print_helper(const Node* node, const vector<string>& col_names,
                                const string& prefix, bool isLeft, std::ostringstream& oss) {
    if (!node) {
        return;
    }

    // Attempt to typecast the node to a DecisionNode; if successful, it is a decision node
    // NOTE: this is why we use a raw pointer as an argument instead of a unique_ptr
    const DecisionNode* decisionNode = dynamic_cast<const DecisionNode*>(node);
    if (decisionNode) {
        // Print decision nodes
        oss << prefix;
        oss << (isLeft ? "├── " : "└── ");
        oss << "[ " << col_names[decisionNode->get_feature_index()] << " <= " << decisionNode->get_threshold() << " ]\n";
        
        // Recur for left and right children
        print_helper(decisionNode->left.get(), col_names, prefix + (isLeft ? "│   " : "    "), true, oss);
        print_helper(decisionNode->right.get(), col_names, prefix + (isLeft ? "│   " : "    "), false, oss);
    }

    // Attempt to typecast the node to a LeafNode; if successful, it is a leaf node
    // NOTE: this is why we use a raw pointer as an argument instead of a unique_ptr
    const LeafNode* leafNode = dynamic_cast<const LeafNode*>(node);
    if (leafNode) {
        // Print leaf nodes
        oss << prefix;
        oss << (isLeft ? "├── " : "└── ");
        oss << "( " << leafNode->predict({}) << " )\n"; // Print label
    }
}

// Helper function for fitting the decision tree recursively. 
// This is the main implementation of the ID3 algorithm.
unique_ptr<Node> DecisionTree::fit_helper(unique_ptr<DataFrame> df, string label_column, int max_depth, int minSamplesSplit) {
    // Base cases for recursion
    if (df->get_num_rows() < minSamplesSplit || max_depth == 0) {
        // Compute the most common label in the dataset
        return std::make_unique<LeafNode>(df->column_mode(label_column));
    }

    // Find the best attribute to split on
    int best_feature = df->selectBestAttribute(label_column);
    if (best_feature == -1) {
        return std::make_unique<LeafNode>(df->column_mode(label_column));
    }

    // Determine threshold for splitting (using median for continuous data)
    double threshold = df->column_median(df->columns[best_feature]);

    // Split data into left and right subsets
    unique_ptr<DataFrame> left_df = df->filter(best_feature, threshold, "<=");
    unique_ptr<DataFrame> right_df = df->filter(best_feature, threshold, ">");

    // If splitting doesn't separate data, return a leaf node
    if (left_df->get_num_rows() == 0 || right_df->get_num_rows() == 0) {
        return std::make_unique<LeafNode>(df->column_mode(label_column));
    }

    // Recursively build left and right subtrees
    unique_ptr<Node> left_child = fit_helper(std::move(left_df), label_column, max_depth - 1, minSamplesSplit);
    unique_ptr<Node> right_child = fit_helper(std::move(right_df), label_column, max_depth - 1, minSamplesSplit);

    // Return the constructed decision node
    return std::make_unique<DecisionNode>(best_feature, threshold, std::move(left_child), std::move(right_child));
}

        

// Constructor
DecisionTree::DecisionTree() : root(nullptr) {}
// Destructor; the default destructor handles destruction of the root node and its children
DecisionTree::~DecisionTree() = default;

// Predict method; simply utilize the functionality from the Node class
double DecisionTree::predict(vector<double> sample) {
    if (!root) {
        throw std::runtime_error("Decision tree is not trained.");
    }

    return root->predict(sample);
}


// Fit method: Entry point for training the decision tree
void DecisionTree::fit(unique_ptr<DataFrame> df, string label_column, int max_depth, int minSamplesSplit) {
    root = fit_helper(std::move(df), label_column, max_depth, minSamplesSplit);
}

// Print method: Entry point for printing the decision tree
string DecisionTree::print(vector<string> col_names) {
    if (!root) {
        return "Empty Decision Tree";
    }

    std::ostringstream oss;
    print_helper(root.get(), col_names, "", true, oss);
    return oss.str();
}

// Get number of nodes; simply utilize the functionality from the Node class
int DecisionTree::get_num_nodes() {
    if (!root) {
        return 0;
    }

    return root->get_num_nodes();
}

// Get height of decision tree; simply utilize the functionality from the Node class
int DecisionTree::get_height() {
    if (!root) {
        return 0;
    }

    return root->get_height();
}