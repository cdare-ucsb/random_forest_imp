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

std::string Node::print() {
    return "Empty node";
}




// --------------- LeafNode Class ---------------

LeafNode::LeafNode(double val) : value(val) {}
LeafNode::~LeafNode() {}

double LeafNode::predict(std::vector<double> sample) {
    return value;
}

void LeafNode::set_value(double val) {
    value = val;
}

std::string LeafNode::print() {
    return "(" + std::to_string(value) + ")";
}



// --------------- DecisionNode Class ---------------

DecisionNode::DecisionNode(int feature_index, double threshold, Node* left_child, Node* right_child) : feature_index(feature_index), threshold(threshold) {
    this->left = left_child;
    this->right = right_child;
}

double DecisionNode::predict(std::vector<double> sample) {
    if (sample[feature_index] <= threshold) {
        return left->predict(sample);
    } else {
        return right->predict(sample);
    }
}

DecisionNode::~DecisionNode() {
    delete left;
    delete right;
}

std::string DecisionNode::print() {
    std::string ret_str =  "[idx:" + std::to_string(feature_index) + ", thr:" + std::to_string(threshold) + "]\n";
    ret_str += "Left Child: " + left->print() + "\n";
    ret_str += "Right Child: " + right->print();
    return ret_str;
}

// Setters implementation

void DecisionNode::set_feature_index(int idx) {
    feature_index = idx;
}

void DecisionNode::set_threshold(double thr) {
    threshold = thr;
}

void DecisionNode::set_left(Node* left_child) {
    left = left_child;
}

void DecisionNode::set_right(Node* right_child) {
    right = right_child;
}


// Getters implementation
int DecisionNode::get_feature_index() {
    return feature_index;
}

double DecisionNode::get_threshold() {
    return threshold;
}

Node* DecisionNode::get_left() {
    return left;
}

Node* DecisionNode::get_right() {
    return right;
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