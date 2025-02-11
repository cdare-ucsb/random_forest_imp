#include <vector>
#include <memory>
#include <string>

#include "DecisionTree.h"
#include "DataFrame.h"
#include "Classifier.h"

class GradientBoostedTrees : public Classifier {
private:
    int max_depth;
    int min_samples_split;

    int num_trees;
    double learning_rate;
    std::vector<std::unique_ptr<DecisionTree>> trees;
    std::vector<double> base_predictions;
public:
    GradientBoostedTrees(int num_trees, double learning_rate, int max_depth, int min_samples_split);

    void GradientBoostedTrees::fit(std::unique_ptr<DataFrame> data, const std::string& label_column);
};