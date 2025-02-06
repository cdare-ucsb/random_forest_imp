#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <vector>
#include "Node.h"
#include "DataFrame.h"

using std::string;
using std::vector;
using std::unique_ptr;

class DecisionTree {
    private:
        unique_ptr<Node> root;
                // Helper function for printing the tree recursively
        void print_helper(const Node* node, const vector<string>& col_names,
                                        const string& prefix, bool isLeft, std::ostringstream& oss); 
        unique_ptr<Node> fit_helper(DataFrame* df, string label_column, int max_depth, int minSamplesSplit);

        
    public:
        // Constructor
        DecisionTree();
        // Destructor
        ~DecisionTree();


        // Predict method
        double predict(std::vector<double> sample);
        
        // Fit method: Entry point for training the decision tree
        void fit(DataFrame* df, string label_column, int max_depth, int minSamplesSplit);
        string print(vector<string> col_names);
};

#endif // DECISIONTREE_H