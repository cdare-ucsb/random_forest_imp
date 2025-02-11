#include <vector>
#include <memory>
#include <iostream>
#include <cmath>  // for pow()
#include "DecisionTree.h"
#include "DataFrame.h"
#include "GradientBoostedTrees.h"




GradientBoostedTrees::GradientBoostedTrees(int num_trees, double learning_rate, int max_depth, int min_samples_split)
    : num_trees(num_trees), learning_rate(learning_rate), max_depth(max_depth), min_samples_split(min_samples_split) {}

void GradientBoostedTrees::fit(std::unique_ptr<DataFrame> data, const std::string& label_column) {
    int n_samples = data->get_num_rows();

    // Step 1: Initialize base prediction (mean of target values)
    base_predictions.resize(n_samples);
    double initial_prediction = data->get_column(label_column).mean();
    std::fill(base_predictions.begin(), base_predictions.end(), initial_prediction);

    for (int i = 0; i < num_trees; ++i) {
        // Step 2: Compute residuals
        std::vector<double> residuals(n_samples);
        for (int j = 0; j < n_samples; ++j) {
            double true_value = DataFrame::double_cast(data->retrieve(j, label_column));
            residuals[j] = true_value - base_predictions[j];
        }

        // Step 3: Train a decision tree to predict residuals
        std::unique_ptr<DataFrame> residual_data = data->copy();
        residual_data->set_column(label_column, residuals);

        auto tree = std::make_unique<DecisionTree>(max_depth, min_samples_split);  // Smaller trees for boosting
        tree->fit(std::move(residual_data), label_column);

        // Step 4: Update predictions with a fraction of the tree's predictions (controlled by learning_rate)
        for (int j = 0; j < n_samples; ++j) {
            base_predictions[j] += learning_rate * tree->predict(data->get_row(j));
        }

        trees.push_back(std::move(tree));
    }
}

double predict(const std::vector<double>& sample) const {
    double prediction = base_predictions[0];  // Start with the initial prediction
    for (const auto& tree : trees) {
        prediction += learning_rate * tree->predict(sample);
    }
    return prediction;
}


