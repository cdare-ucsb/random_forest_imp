#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <vector>
#include "Node.h"
#include "DataFrame.h"

using std::string;
using std::vector;
using std::unique_ptr;



/**
 * @class DecisionTree
 * @brief A class that represents a decision tree model using the ID3 algorithm for classification
 * 
 * This class represents a decision tree model that can be used for classification tasks.
 * The tree is built using the ID3 algorithm, which is a top-down, greedy algorithm that selects
 * the best attribute to split the data at each node based on the information gain. The tree is
 * represented as a collection of nodes, where each node contains a decision rule based on an attribute
 * and a value, and a pointer to its child nodes. The tree can be trained using the fit method, which
 * takes a DataFrame object containing the training data and a label column, and builds the tree using
 * the ID3 algorithm. The tree can be printed using the print method, which returns a string representation
 * of the tree. The tree can be used to make predictions using the predict method, which takes a sample
 * vector of feature values and returns the predicted class label.
 */
class DecisionTree {
    private:
        unique_ptr<Node> root; ///< Pointer to the root node of the decision tree
        
        /**
         * @brief Helper method for the print function
         * @param node Pointer to the node in the decision tree that one wants to begin printing from.
         *           This is typically the root node. Note that this is a recursive function that will
         *           traverse the entire tree starting from the given node. Additionally, a raw pointer
         *           is used here to avoid type conflicts between DecisionNode and TreeNode.
         * @param col_names Vector of column names from the DataFrame that was used to train the decision tree.
         * @param prefix String that is used to format the output of the tree.
         * @param isLeft Boolean value that indicates whether the current node is a left child or not.
         * @param oss Output string stream that is used to build the string representation of the tree.
         * 
         * This is a recursive helper function that traverses the decision tree starting from the given node. 
         * The function prints the decision rules at each node and the predicted class label at each leaf node.
         * The function uses a raw pointer as an argument to avoid type conflicts between DecisionNode and TreeNode.
         * 
         * @see print(const vector<string>& col_names)
         */
        void print_helper(const Node* node, const vector<string>& col_names,
                                        const string& prefix, bool isLeft, std::ostringstream& oss); 

        /**
         * @brief Helper method for the fit function. Main implementation of ID3 algorithm.
         * @param df Pointer to the DataFrame object containing the training data
         * @param label_column Name of the column in the DataFrame that contains the class labels
         * @param max_depth Maximum depth of the decision tree
         * @param minSamplesSplit Minimum number of samples required to split a node
         * @return Pointer to the root node of the decision tree
         * 
         * This is a recursive helper function that builds the decision tree by selecting the best attribute. 
         * For the most part, the ID3 algorithm is implemented in this function and not in the fit() function.
         * 
         * @see fit(unique_ptr<DataFrame> df, string label_column, int max_depth, int minSamplesSplit)
         */
        unique_ptr<Node> fit_helper(unique_ptr<DataFrame> df, string label_column, int max_depth, int minSamplesSplit);

        
    public:
        /**
         * @brief Constructor for the DecisionTree class
         * 
         * The default constructor is used here to initialize the root node to nullptr.
         */
        DecisionTree();
        /**
         * @brief Destructor for the DecisionTree class
         * 
         * The default destructor is used here to handle the destruction of the root node and its children.
         */
        ~DecisionTree();


        /**
         * @brief Predict method
         * @param sample Vector of feature values for a single sample
         * @return Predicted class label for the sample
         * @throws runtime_error if the decision tree is not trained
         * 
         * This function takes a vector of feature values for a single sample and returns the predicted.
         * class label. The function traverses the decision tree starting from the root node and follows the
         * decision rules at each node to determine the predicted class label for the sample. The actual recusion
         * is implemented in the Node class, and this function simply calls the predict() method of the root node.
         * 
         * @see Node::predict(const vector<double>& sample)
         * 
         * @code
         * std::vector<std::vector<double>> data1 = {{2.5, 1.5, 0}, {1.0, 3.0, 1}, {3.5, 2.0, 0}, {4.0, 3.5, 1}, {5.0, 2.5, 1}};
         * std::vector<std::string> columns1 = {"A", "B", "C"};
         * unique_ptr<DataFrame> df1 = std::make_unique<DataFrame>(data1, columns1);
         * DecisionTree dt1;
         * dt1.fit(std::move(df1), "C", 3, 1);
         * std::cout << dt1.predict({2.5, 1.5}) << std::endl;
         * @endcode
         */
        double predict(std::vector<double> sample);

        /**
         * @brief Get the number of nodes in the decision tree
         * @return Number of nodes in the decision tree
         * 
         * This function returns the number of nodes in the decision tree. The function simply
         * calls the get_num_nodes() method of the root node, which recursively calculates the
         * number of nodes in the tree.
         * 
         * @see Node::get_num_nodes()
         */
        int get_num_nodes();

        /**
         * @brief Get the height of the decision tree
         * @return the height, as an integer, of the fitted decision tree. 
         * 
         * This function returns the height of the decision tree starting from the root node.
         * In the case that tree is empty or consists of a single leaf node (i.e. the root is
         * a leaf node) the function returns 0. The function simply calls the get_height() method
         * of the root, which recursively calculates the height of its children.
         * 
         * @see Node::get_height()
         */
        int get_height();

        
        /**
         * @brief The fit method trains the decision tree using the ID3 algorithm
         * @param df unique_ptr to the DataFrame object containing the training data
         * @param label_column Name of the column in the DataFrame that contains the class labels
         * @param max_depth Maximum depth of the decision tree
         * @param minSamplesSplit Minimum number of samples required to split a node
         * 
         * This is the entry point for training the decision tree. The function takes a unique_ptr to
         * a DataFrame and the name of the column containing the class labels, as well as the maximum depth
         * of the tree and the minimum number of samples required to split a node. The function then calls
         * the fit_helper() method to build the decision tree using the ID3 algorithm.
         * 
         * @see fit_helper(unique_ptr<DataFrame> df, string label_column, int max_depth, int minSamplesSplit)
         * 
         * @code
         * std::vector<std::vector<double>> data1 = {{2.5, 1.5, 0}, {1.0, 3.0, 1}, {3.5, 2.0, 0}, {4.0, 3.5, 1}, {5.0, 2.5, 1}};
         * std::vector<std::string> columns1 = {"A", "B", "C"};
         * unique_ptr<DataFrame> df1 = std::make_unique<DataFrame>(data1, columns1);
         * DecisionTree dt1;
         * dt1.fit(std::move(df1), "C", 3, 1);
         * @endcode
         */
        void fit(unique_ptr<DataFrame> df, string label_column, int max_depth, int minSamplesSplit);

        /**
         * @brief Print method for the decision tree
         * @param col_names Vector of column names from the DataFrame that was used to train the decision tree
         * @return String representation of the decision tree
         * 
         * This function returns a string representation of the decision tree. The function calls the
         * print_helper() method to traverse the tree starting from the root node and build the string
         * representation of the tree.
         * 
         * @see print_helper(const Node* node, const vector<string>& col_names, const string& prefix, bool isLeft, std::ostringstream& oss)
         * @see Node::print()
         * 
         * @code
         * std::vector<std::vector<double>> data1 = {{2.5, 1.5, 0}, {1.0, 3.0, 1}, {3.5, 2.0, 0}, {4.0, 3.5, 1}, {5.0, 2.5, 1}};
         * std::vector<std::string> columns1 = {"A", "B", "C"};
         * unique_ptr<DataFrame> df1 = std::make_unique<DataFrame>(data1, columns1);
         * DecisionTree dt1;
         * dt1.fit(std::move(df1), "C", 3, 1);
         * std::cout << dt1.print(columns1) << std::endl;
         * @endcode
         * 
         */
        string print(vector<string> col_names);
};

#endif // DECISIONTREE_H