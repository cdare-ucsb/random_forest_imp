#ifndef NODE_H
#define NODE_H

using std::unique_ptr;
using std::vector;

/**
 * @class Node
 * @brief A class that represents a node in a decision tree
 * 
 * This class represents a node in a decision tree. The node can be either a decision node or a leaf node. 
 * A decision node contains a decision rule based on an attribute and a value, and pointers to its left and right
 * child nodes. A leaf node contains a predicted value for the target variable. The node class is an abstract class
 * that is inherited by the DecisionNode and LeafNode classes.
 */
class Node {
    public:
        unique_ptr<Node> left; ///< Pointer to the left child node
        unique_ptr<Node> right; ///< Pointer to the right child node
        /**
         * @brief Constructor for the Node class
         */
        Node();
        /**
         * @brief Destructor for the Node class
         */
        virtual  ~Node();
        /**
         * @brief Predict method
         * @param sample Vector of feature values for a single sample
         * @return Predicted value for the sample
         * 
         * This function takes a vector of feature values for a single sample and returns the predicted value.
         * The function is implemented in the derived classes LeafNode and DecisionNode, and is called by the predict()
         * method of the DecisionTree class.
         * 
         * @see DecisionTree::predict(std::vector<double> sample)
         * @see DecisionNode::predict(std::vector<double> sample)
         * @see LeafNode::predict(std::vector<double> sample)
         */
        virtual double predict(const vector<double>& sample) const = 0;
        /**
         * @brief Print method
         * @return String representation of the node
         * 
         * This function returns a string representation of the node. The function is implemented in the derived classes
         * LeafNode and DecisionNode, and is called by the print() method of the DecisionTree class.
         * 
         * @see DecisionTree::print()
         * @see DecisionNode::print()
         * @see LeafNode::print()
         */
        virtual std::string print() = 0;
        /**
         * @brief Get the number of nodes in the subtree rooted at this node
         * @return Number of nodes in the subtree rooted at this node
         * 
         * This function returns the number of nodes in the subtree rooted at this node. The function recursively
         * calculates the number of nodes in the subtree by calling the get_num_nodes() method of the left and right
         * child nodes. The function is implemented in the derived classes LeafNode and DecisionNode, and is called by the
         * get_num_nodes() method of the DecisionTree class. For the base class Node and the LeafNode class, the
         * function returns 1.
         * 
         * @see DecisionTree::get_num_nodes()
         * @see DecisionNode::get_num_nodes()
         * @see LeafNode::get_num_nodes()
         */
        virtual int get_num_nodes() = 0;

        /**
         * @brief Get the height of the subtree rooted at this node
         * @return Height of the subtree rooted at this node
         * 
         * This function returns the height of the subtree rooted at this node. The function recursively calculates
         * the height of the subtree by calling the get_height() method of the left and right child nodes. The function
         * is implemented in the derived classes LeafNode and DecisionNode, and is called by the get_height() method of the
         * DecisionTree class. For the base class Node and the LeafNode class, the function returns 0.
         * 
         * @see DecisionTree::get_height()
         * @see DecisionNode::get_height()
         * @see LeafNode::get_height()
         */
        virtual int get_height() = 0;
};

/**
 * @class LeafNode
 * @brief A class that represents a leaf node in a decision tree
 * 
 * This class represents a leaf node in a decision tree. A leaf node contains a predicted value for the target variable.
 * The LeafNode class inherits from the Node class and implements the pure virtual methods defined in the Node class.
 * 
 * @see Node
 */
class LeafNode : public Node {
    protected:
        double value; ///< Value that the leaf node should predict in the decision tree
    public:
        /**
         * @brief Constructor for LeafNode
         * 
         * This constructor initializes a LeafNode with a given value.
         * 
         * @param val The value that the leaf node should predict
         */
        LeafNode(double val);
        /**
         * @brief Destructor for LeafNode
         */
        ~LeafNode();

        /**
         * @brief Predict the value for a given sample
         * 
         * This method predicts the value for the target variable based on the given sample. 
         * Since the node is a leaf node, it simply the value that the leaf node should predict (i.e.
         * the value variable).
         * 
         * @param sample A vector of feature values representing the sample
         * @return The predicted value for the target variable
         * 
         * @see DecisionTree::predict(std::vector<double> sample)
         * @see Node::predict(std::vector<double> sample)
         * @see DecisionNode::predict(std::vector<double> sample)
         */
        double predict(const vector<double>& sample) const override;
        /**
         * @brief Print the leaf node
         * @return A string representation of the leaf node
         * 
         * This method returns a string representation of the leaf node.
         * The string representation includes the value that the leaf node should predict.
         * The format of the string is "(value)". For example, if the value is 1.0, the string
         * representation would be "(1.0)".
         * 
         * @see DecisionTree::print()
         * @see Node::print()
         * @see DecisionNode::print()
         */
        std::string print() override;

        /**
         * @brief Set the value of the leaf node
         * @param val The value that the leaf node should predict
         * 
         * This method sets the value that the leaf node should predict.
         */
        void set_value(double val);

        /**
         * @brief Get the value of the leaf node
         * @return The value that the leaf node should predict
         * 
         * This method overrides the corresponding method in the Node class;
         * since the node is a leaf node, it should have no children and thus
         * return just itself.
         * 
         * @see DecisionTree::get_num_nodes()
         * @see Node::get_num_nodes()
         * @see DecisionNode::get_num_nodes()
         */
        int get_num_nodes() override;

        /**
         * @brief Get the height of the subtree rooted at this node
         * @return Height of the subtree rooted at this node
         * 
         * This method overrides the corresponding method in the Node class; since the
         * node is a leaf node, it should have no children and thus return 0.
         * 
         * @see DecisionTree::get_height()
         * @see Node::get_height()
         * @see DecisionNode::get_height()
         */
        int get_height() override;
        
};


/**
 * @class DecisionNode
 * @brief A class that represents a decision node in a decision tree
 * 
 * This class represents a decision node in a decision tree. A decision node contains a decision rule based on an attribute
 * and a value, and pointers to its left and right child nodes. The DecisionNode class inherits from the Node class and
 * implements the pure virtual methods defined in the Node class.
 * 
 * @see Node
 */
class DecisionNode : public Node{
    protected:
        int feature_index; ///< Index of the feature used for the decision rule
        double threshold; ///< Threshold value for the decision rule

    public:
        unique_ptr<Node> left; ///< Pointer to the left child node
        unique_ptr<Node> right; ///< Pointer to the right child node
        /**
         * @brief Constructor for DecisionNode
         * 
         * @param feature_index Index of the feature used for the decision rule
         * @param threshold Threshold value for the decision rule
         * @param left_child Pointer to the left child node
         * @param right_child Pointer to the right child node
         * 
         * This constructor initializes a DecisionNode with a given feature index, threshold, and pointers to its left and right child nodes.
         */
        DecisionNode(int feature_index, double threshold, unique_ptr<Node> left_child, unique_ptr<Node> right_child);
        /**
         * @brief Destructor for DecisionNode
         */
        ~DecisionNode();

        /**
         * @brief Predict method
         * @param sample Vector of feature values for a single sample
         * @return Predicted value for the sample
         * 
         * This function takes a vector of feature values for a single sample and returns the predicted value.
         * Since the node is a decision node, it checks the value of the feature at the feature index and compares it
         * to the threshold. If the value is less than the threshold, the function calls the predict() method of the left
         * child node. Otherwise, it calls the predict() method of the right child node.
         * 
         * @see DecisionTree::predict(std::vector<double> sample)
         * @see Node::predict(std::vector<double> sample)
         */
        double predict(const vector<double>& sample) const override;

        /**
         * @brief Print method
         * @return String representation of the node
         * 
         * This function returns a string representation of the node. The function prints the decision rule based on the
         * feature index and threshold, and the left and right child nodes. The function is called by the print() method of
         * the DecisionTree class. The format of the string is "Feature <feature_index> <= <threshold> ? <left_child> : <right_child>".
         * For example, if the feature index is 0, the threshold is 0.5, and the left and right child nodes are leaf nodes with values
         * 1.0 and 0.0, respectively, the string representation would be "Feature 0 <= 0.5 ? (1.0) : (0.0)".
         * 
         * @see DecisionTree::print()
         * @see Node::print()
         */
        std::string print() override;

        /**
         * @brief Sets the feature index
         * @param idx Index of the feature used for the decision rule
         */
        void set_feature_index(int idx);

        /**
         * @brief Sets the threshold
         * @param thr Threshold value for the decision rule
         */
        void set_threshold(double thr);

        /**
         * @brief Get the feature index
         * @return Index of the feature used for the decision rule
         */
        int get_feature_index() const;

        /**
         * @brief Get the threshold
         * @return Threshold value for the decision rule
         */
        double get_threshold() const;

        /**
         * @brief Get the number of nodes in the subtree rooted at this node
         * @return Number of nodes in the subtree rooted at this node
         * 
         * This function returns the number of nodes in the subtree rooted at this node. Since the node is a decision node,
         * the function recursively calculates the number of nodes in the subtree by calling the get_num_nodes() method of the
         * left and right child nodes. The function is called by the get_num_nodes() method of the DecisionTree class.
         * 
         * @see DecisionTree::get_num_nodes()
         * @see DecisionNode::get_num_nodes()
         * @see LeafNode::get_num_nodes()
         */
        int get_num_nodes() override;

        /**
         * @brief Get the height of the subtree rooted at this node
         * @return Height of the subtree rooted at this node
         * 
         * This function returns the height of the subtree rooted at this node. Since the node is a decision node, the function
         * recursively calculates the height of the subtree by calling the get_height() method of the left and right child nodes.
         * The function is called by the get_height() method of the DecisionTree class.
         * 
         * @see DecisionTree::get_height()
         * @see DecisionNode::get_height()
         * @see LeafNode::get_height()
         */
        int get_height() override;
};

#endif // NODE_H