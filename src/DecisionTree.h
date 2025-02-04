#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "node.h"

class DecisionTree {
    protected:
        Node* root;

    public:
        // Constructor
        DecisionTree(Node* root);
        // Destructor
        ~DecisionTree();

        // Predict method
        double predict(std::vector<double> sample);

        // Print method
        std::string print();

}

#endif // DECISIONTREE_H