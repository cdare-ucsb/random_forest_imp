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
         */
        virtual int get_height() = 0;
};

/**
 * @class LeafNode
 * @brief A class which represents the leaf node of a decision tree
 */
class LeafNode : public Node {
    protected:
        double value; 
        unique_ptr<Node> left;
        unique_ptr<Node> right;
    public:
        // Constructor
        LeafNode(double val);
        // Destructor
        ~LeafNode();

        // Override methods
        double predict(const vector<double>& sample) const override;
        std::string print() override;

        // Setters
        void set_value(double val);
        int get_num_nodes() override;
        int get_height() override;
        
};

class DecisionNode : public Node{
    protected:
        int feature_index;
        double threshold;

    public:
        unique_ptr<Node> left;
        unique_ptr<Node> right;
        // Constructor
        DecisionNode(int feature_index, double threshold, unique_ptr<Node> left_child, unique_ptr<Node> right_child);
        // Destructor
        ~DecisionNode();

        // Override methods
        double predict(const vector<double>& sample) const override;
        std::string print() override;

        // Setters
        void set_feature_index(int idx);
        void set_threshold(double thr);

        // Getters
        int get_feature_index() const;
        double get_threshold() const;
        int get_num_nodes() override;
        int get_height() override;
};

#endif // NODE_H