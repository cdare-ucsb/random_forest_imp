#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <queue>
#include <iomanip>
#include <sstream>

#include "DataFrame.h"
#include "Node.h"
#include "DecisionTree.h"

using std::string;
using std::vector;


/* -------------------------------------------
*             CONSTRUCTOR/DESTRUCTOR
----------------------------------------------*/
DecisionTree::DecisionTree(): root(nullptr){}


DecisionTree::~DecisionTree() {
    root = nullptr;
}

/* -------------------------------------------
*             GETTERS
----------------------------------------------*/

int DecisionTree::get_num_nodes() {
    if (root == nullptr) {
        return 0;
    }
    return root->get_num_nodes();
}


/* -------------------------------------------
*             FIT METHOD
----------------------------------------------*/


void DecisionTree::fit(DataFrame* df, string label_column, int max_depth, int minSamplesSplit) {
    this->root = fit_helper(df, label_column, max_depth, minSamplesSplit);

    printf("\n\nFit_helper finished:\n root node is %s\n", root->print(df->columns).c_str());
    printf("Left child is %s\n", root->left->print(df->columns).c_str());
    printf("Right child is %s\n", root->right->print(df->columns).c_str());
}

Node* DecisionTree::fit_helper(DataFrame* df, string label_column, int max_depth, int minSamplesSplit) {
    printf("Call to fit_helper; input dataframe:\n%s\n", df->print().c_str());
    printf("Label column: %s\n", label_column.c_str());
    printf("Max depth: %d\n", max_depth);
    printf("Min samples split: %d\n", minSamplesSplit);
    
    // Extract the target column
    vector<double> labels = df->get_column(label_column);

    // Base case: if the max depth is reached or if the number of samples is less than the minimum required
    if (max_depth == 0 || df->get_num_rows() < minSamplesSplit) {
        if (max_depth == 0) printf("Max depth reached\n");
        if (df->get_num_rows() < minSamplesSplit) printf("Min samples split reached\n");
        return new LeafNode(df->column_median(label_column));
    }

    // Edge case: if all the labels are the same
    bool same_labels = true;
    for (size_t i = 1; i < labels.size(); i++) {
        if (labels[i] != labels[0]) {
            same_labels = false;
            break;
        }
    }
    if (same_labels) {
        if (same_labels) printf("All labels are the same\n");
        return new LeafNode(labels[0]);
    }

    // Find the best attribute to split on
    int bestAttribute = df->selectBestAttribute(label_column);
    if (bestAttribute == -1) {
        return new LeafNode(df->column_median(label_column));
    }

    printf("Split choice: [%s <= %f]\n", df->columns[bestAttribute].c_str(), df->column_median(df->columns[bestAttribute]));


    // Split the dataset based on the best attribute
    DataFrame* left_df = df->filter(bestAttribute, df->column_median(df->columns[bestAttribute]), "<=");
    DataFrame* right_df = df->filter(bestAttribute, df->column_median(df->columns[bestAttribute]), ">");
    printf("Left dataframe after splitting:\n%s\n", left_df->print().c_str());
    printf("Right dataframe after splitting:\n%s\n", right_df->print().c_str());


    // Recursively build the tree
    Node* left_child = fit_helper(left_df, label_column, max_depth - 1, minSamplesSplit);
    Node* right_child = fit_helper(right_df, label_column, max_depth - 1, minSamplesSplit);

    // Create a decision node
    DecisionNode* decision_node = new DecisionNode(bestAttribute, df->column_median(df->columns[bestAttribute]),
                                          left_child, right_child);
    printf("Decision node created: %s\n", decision_node->print(df->columns).c_str());
    if (decision_node->left == nullptr) printf("Left child is null\n");
    else printf("Left child is %s\n", decision_node->left->print(df->columns).c_str());
    if (decision_node->right == nullptr) printf("Right child is null\n");
    else printf("Right child is %s\n", decision_node->right->print(df->columns).c_str());

    // Clean up
    delete left_df;
    delete right_df;

    return decision_node;
}

/* -------------------------------------------
*             PRINT METHOD
----------------------------------------------*/
// string DecisionTree::print(vector<string> col_names) {

//     if (root == nullptr)
//         return "";

//     std::ostringstream output;
//     int height = root->get_height();
//     int maxWidth = (1 << height) - 1;  // Maximum width of last level (2^h - 1)

//     std::queue<std::unique_ptr<Node>> q;
//     q.push(root);

//     int level = 0;
//     while (!q.empty()) {
//         int count = q.size();
//         int spaces = (maxWidth / (1 << level)) - 1;  // Leading spaces
//         int betweenSpaces = (spaces * 2) + 1;        // Spaces between nodes

//         output << std::string(spaces, ' ');  // Leading spaces

//         std::queue<std::unique_ptr<Node>> tempQueue;  // Next level queue
//         while (count--) {
//             std::unique_ptr<Node> node = q.front();
//             q.pop();

//             if (node) {
//                 output << std::setw(2) << node->print(col_names);  // Print node value
//                 tempQueue.push(node->left);
//                 tempQueue.push(node->right);
//             } else {
//                 output << "  ";  // Empty space for NULL nodes
//                 tempQueue.push(nullptr);
//                 tempQueue.push(nullptr);
//             }

//             if (count)
//                 output << std::string(betweenSpaces, ' ');  // Spacing between nodes
//         }
//         output << "\n";  // New line for next level

//         // Check if all nodes in next level are null (stop early)
//         bool allNull = true;
//         size_t tempQueueSize = tempQueue.size();
//         for (size_t i = 0; i < tempQueueSize; i++) {
//             std::unique_ptr<Node> node = tempQueue.front();
//             tempQueue.pop();
//             if (node) allNull = false;
//             tempQueue.push(node);
//         }
//         if (allNull) break;

//         q = tempQueue;
//         level++;
//     }

//     return output.str();  // Return the formatted tree as a string
// }


int main(int argc, char* argv[]) {
    // Create a simple decision tree
    DataFrame df({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {"A", "B", "C"});
    DecisionTree dt;
    dt.fit(&df, "C", 5, 1);
    // std::string tree_str = dt.print({"A", "B", "C"});
    // printf("%s\n", tree_str.c_str());
    printf("%s\n", dt.root->print({"A", "B", "C"}).c_str());
    printf("%d\n", dt.root->left == nullptr ? 1 : 0);
    printf("%d\n", dt.root->right == nullptr ? 1 : 0);
    return 0;
}