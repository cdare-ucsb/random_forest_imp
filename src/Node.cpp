#include <iostream>
#include <vector>
#include <string>

#include "Node.h"

using std::vector;
using std::unique_ptr;


// --------------- Node Class ---------------

Node::Node(): left(nullptr), right(nullptr) {}
Node::~Node() = default;
double Node::predict(const vector<double>& sample) const {
    return 0.0;
}

std::string Node::print()  {
    return "Empty node";
}

int get_num_nodes() {
    return 1;
}

int get_height() {
    return 0;
}


// --------------- Node Class ---------------


// Constructor
LeafNode::LeafNode(double val) : value(val) {}

// Destructor
LeafNode::~LeafNode() = default;

double LeafNode::predict(const vector<double>& sample) const {
    return value;
}

std::string LeafNode::print()  {
    return "(" + std::to_string(value) + ")";
}

void LeafNode::set_value(double val) {
    value = val;
}

int LeafNode::get_num_nodes() {
    return 1;
}   

int LeafNode::get_height() {
    return 0;
}

// --------------- DecisionNode Class ---------------

DecisionNode::DecisionNode(int feature_index, double threshold, std::unique_ptr<Node> left_child, std::unique_ptr<Node> right_child)
    : feature_index(feature_index), threshold(threshold), left(std::move(left_child)), right(std::move(right_child)) {}

//Deconstructor
DecisionNode::~DecisionNode(){
    left = nullptr;
    right = nullptr;
};

double DecisionNode::predict(const std::vector<double>& sample) const {
    if (sample[feature_index] <= threshold) {
        return left->predict(sample);
    } else {
        return right->predict(sample);
    }
}

double DecisionNode::get_threshold() const {
    return threshold;
}

int DecisionNode::get_feature_index() const {
    return feature_index;
}


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



std::string DecisionNode::print()  {
    return "[Feature " + std::to_string(feature_index) + " <= " + std::to_string(threshold) + "]\n" +
            "├── Left: " + (left ? left->print() : "NULL") + "\n" +
            "└── Right: " + (right ? right->print() : "NULL");
}