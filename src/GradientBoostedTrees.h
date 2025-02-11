#include <vector>
#include <memory>
#include <string>

#include "DecisionTree.h"
#include "DataFrame.h"
#include "Classifier.h"


/**
 * @class GradientBoostedTrees
 * @brief Class implementation of gradient boosted trees
 * 
 * 
 * This class represents a gradient boosted trees model, which is an ensemble learning method that builds a series of decision trees.acosf
 * The GradientBoostedTrees class provides methods for fitting the model to the data and making predictions. 
 * Unlike the RandomForest class, the GradientBoostedTrees class builds a series of decision trees sequentially, where each tree is trained
 * to correct the errors of the previous tree. The final prediction is the sum of the predictions from each tree in the series.
 */
class GradientBoostedTrees : public Classifier {
private:
    int max_depth; ///< Maximum depth of the trees
    int min_samples_split; ///< Minimum number of samples required to split a node

    int num_trees; ///< Number of trees that should sequentially be built
    double learning_rate; ///< Learning rate for the gradient boosting algorithm
    std::vector<std::unique_ptr<DecisionTree>> trees; ///< Vector of decision trees in the ensemble
    std::vector<double> base_predictions; ///< Vector of base predictions for the gradient boosting algorithm
public:
    /**
     * @brief Constructor for the GradientBoostedTrees class
     * @param num_trees Number of trees to build
     * @param learning_rate Learning rate for the gradient boosting algorithm
     * @param max_depth Maximum depth of the trees
     * @param min_samples_split Minimum number of samples required to split a node
     * 
     * Constructor for the GradientBoostedTrees class. The constructor initializes the GradientBoostedTrees with the specified
     * number of trees, learning rate, maximum depth of the trees, and minimum number of samples required to split a node.
     */
    GradientBoostedTrees(int num_trees, double learning_rate, int max_depth, int min_samples_split);

    /**
     * @brief Destructor for the GradientBoostedTrees class
     * 
     * Destructor for the GradientBoostedTrees class. The destructor is used here to handle the destruction of the decision trees in the ensemble.
     */
    ~GradientBoostedTrees();

    /**
     * @brief Function to fit the GradientBoostedTrees to the data
     * @param data Data to fit the GradientBoostedTrees to
     * @param label_column Name of the column containing the labels
     * 
     * This function fits the GradientBoostedTrees to the data by training the individual decision trees in the ensemble.
     * The function takes a DataFrame containing the data and the name of the column containing the labels.
     * For each decision tree, the base predictions are calculated, and the tree is trained to correct the errors of the previous tree.
     */
    void fit(std::shared_ptr<DataFrame> data, const std::string& label_column) override;

    /**
     * @brief Function to make predictions using the GradientBoostedTrees
     * @param sample Sample to make predictions on
     * @return Prediction from the GradientBoostedTrees
     * 
     * This function makes predictions using the GradientBoostedTrees on the specified sample.
     * The function returns the prediction from the GradientBoostedTrees.
     */
    double predict(const std::vector<double>& sample) const override;
};