#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <vector>
#include "Node.h"
#include "DataFrame.h"

class DecisionTree {
    protected:
        

        Node* fit_helper(DataFrame* df, std::string label_column, int max_depth, int minSamplesSplit);
        string print_helper(Node* node, int height);
    public:
        // Constructor
        DecisionTree();
        // Destructor
        ~DecisionTree();
        Node* root;

        // Predict method
        double predict(std::vector<double> sample);

        void fit(DataFrame* df, std::string label_column, int max_depth, int minSamplesSplit);

        // Print method
        std::string print(vector<string> col_names);

        // Getters
        int get_num_nodes();

};

#endif // DECISIONTREE_H