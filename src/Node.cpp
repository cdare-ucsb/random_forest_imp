#include <iostream>
#include <vector>
#include <string>

#include "Node.h"

using std::vector;



// --------------- Node Class ---------------

Node::Node(): left(nullptr), right(nullptr) {}
Node::~Node() = default;
double Node::predict(std::vector<double> sample) {
    return 0.0;
}

std::string Node::print(std::vector<std::string> col_names) {
    return "Empty node";
}

int get_num_nodes() {
    return 1;
}

int get_height() {
    return 1;
}


// --------------- LeafNode Class ---------------

LeafNode::LeafNode(double val) : value(val), left(nullptr), right(nullptr) {}

LeafNode::~LeafNode() {}

double LeafNode::predict(std::vector<double> sample) {
    return value;
}

void LeafNode::set_value(double val) {
    value = val;
}

std::string LeafNode::print(std::vector<std::string> col_names) {
    return "(" + std::to_string(value) + ")";
}

int LeafNode::get_num_nodes() {
    return 1;
}

int LeafNode::get_height() {
    return 1;
}

// --------------- DecisionNode Class ---------------

DecisionNode::DecisionNode(int feature_index, double threshold, Node* left_child, Node* right_child) : feature_index(feature_index), threshold(threshold), left(left_child), right(right_child) {}


double DecisionNode::predict(std::vector<double> sample) {
    if (sample[feature_index] <= threshold) {
        return left->predict(sample);
    } else {
        return right->predict(sample);
    }
}

DecisionNode::~DecisionNode() {
    left = nullptr;
    right = nullptr;
}

std::string DecisionNode::print(std::vector<std::string> col_names) {
    return  "[" + col_names[feature_index] + " <= " + std::to_string(threshold) + "]";
}


// Setters implementation

void DecisionNode::set_feature_index(int idx) {
    feature_index = idx;
}

void DecisionNode::set_threshold(double thr) {
    threshold = thr;
}



// Getters implementation
int DecisionNode::get_feature_index() {
    return feature_index;
}

double DecisionNode::get_threshold() {
    return threshold;
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
        return 1;
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



// --------------- Main Function ---------------

// int main(int argc, char* argv[]) {

//     LeafNode* left_1 = new LeafNode(1);
//     LeafNode* right_1 = new LeafNode(2);
//     LeafNode* left_2 = new LeafNode(3);
//     LeafNode* right_2 = new LeafNode(4);

//     DecisionNode* parent_1 = new DecisionNode(1, 2.0, left_1, right_1);
//     DecisionNode* parent_2 = new DecisionNode(1, 6.0, left_2, right_2);
//     DecisionNode* root = new DecisionNode(0, 3.0, parent_1, parent_2);


//     //printf("Root: %s\n", root->print().c_str());

//     printf("%s\n" , parent_1->print().c_str());
//     std::string my_str_1 = "[idx:1, thr:2.000000]\nLeft Child: (1.000000)\nRight Child: (2.000000)\n";
//     std::string my_str_2 = parent_1->print();

//     std::cout << my_str_1.compare(my_str_2) << std::endl;
//     // Cleanup (avoids memory leak)
//     delete root;
// }