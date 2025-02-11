#include <iostream>
#include <vector>
#include <string>

#include "Node.h"

using std::vector;
using std::unique_ptr;
using std::string;


// --------------- Node Class ---------------

Node::Node(): left(nullptr), right(nullptr) {}
Node::~Node() = default;

// Default implementation of predict method
double Node::predict(const vector<double>& sample) const {
    return 0.0;
}

// Default implementation of print method
std::string Node::print()  {
    return "Empty node";
}

// Default implementation of get_num_nodes method
int get_num_nodes() {
    return 1;
}

// Default implementation of get_height method
int get_height() {
    return 0;
}


// --------------- Leaf Node Class ---------------


// Constructor
LeafNode::LeafNode(double val) : value(val) {}

// Destructor
LeafNode::~LeafNode() = default;

// Predict method --- since the node is a leaf node, it simply returns the value that the leaf node should predict
double LeafNode::predict(const vector<double>& sample) const {
    return value;
}

// Print method --- returns a string representation of the leaf node in the format "(value)"
std::string LeafNode::print()  {
    return "(" + std::to_string(value) + ")";
}

// Setters
void LeafNode::set_value(double val) {
    value = val;
}

// Getters
int LeafNode::get_num_nodes() {
    return 1;
}   

int LeafNode::get_height() {
    return 0;
}

// --------------- DecisionNode Class ---------------

// Constructor
DecisionNode::DecisionNode(int feature_index, double threshold, std::unique_ptr<Node> left_child, std::unique_ptr<Node> right_child)
    : feature_index(feature_index), threshold(threshold), left(std::move(left_child)), right(std::move(right_child)) {}

//Deconstructor
DecisionNode::~DecisionNode(){
    left = nullptr;
    right = nullptr;
};

// Predict method; the function checks if the feature value of the sample is less than or equal to the threshold.
// If it is, the function calls the predict() method of the left child node. Otherwise, it calls the predict() method
// of the right child node.
double DecisionNode::predict(const std::vector<double>& sample) const {
    if (sample[feature_index] <= threshold) {
        return left->predict(sample);
    } else {
        return right->predict(sample);
    }
}

// Getters
double DecisionNode::get_threshold() const {
    return threshold;
}

int DecisionNode::get_feature_index() const {
    return feature_index;
}

// Recursively calculate the number of nodes in the subtree rooted at this node
int DecisionNode::get_num_nodes() {
    if (left == nullptr && right == nullptr) {
        return 1;
    }
    else if (left == nullptr) {
        return 1 + right->get_num_nodes();
    }
    else if (right == nullptr) {
        return 1 + left->get_num_nodes();
    }
    else {
        return 1 + left->get_num_nodes() + right->get_num_nodes();
    }
}


// Recursively calculate the height of the subtree rooted at this node
int DecisionNode::get_height() {
    if (left == nullptr && right == nullptr) {
        return 0;
    }
    else if (left == nullptr) {
        return 1 + right->get_height();
    }
    else if (right == nullptr) {
        return 1 + left->get_height();
    }
    else {
        return 1 + std::max(left->get_height(), right->get_height());
    }
}


// Setters
void DecisionNode::set_feature_index(int idx) {
    feature_index = idx;
}
        
void DecisionNode::set_threshold(double thr) {
    threshold = thr;
}


// Print method; the function returns a string representation of the decision node in the format "Feature feature_index <= threshold ? (left) : (right)"
string DecisionNode::print()  {
    return "[Feature " + std::to_string(feature_index) + " <= " + std::to_string(threshold) + "]\n" +
            "├── Left: " + (left ? left->print() : "NULL") + "\n" +
            "└── Right: " + (right ? right->print() : "NULL");
}