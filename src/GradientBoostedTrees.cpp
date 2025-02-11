#include <vector>
#include <memory>
#include <iostream>
#include <cmath>  // for pow()
#include "DecisionTree.h"
#include "DataFrame.h"
#include "GradientBoostedTrees.h"

using Cell = std::variant<int, double, std::string>;
using std::vector;
using std::string;



GradientBoostedTrees::GradientBoostedTrees(int num_trees, double learning_rate, int max_depth, int min_samples_split)
    : num_trees(num_trees), learning_rate(learning_rate), max_depth(max_depth), min_samples_split(min_samples_split) {}

GradientBoostedTrees::~GradientBoostedTrees() {}

void GradientBoostedTrees::fit(std::shared_ptr<DataFrame> data, const std::string& label_column) {
    int n_samples = data->get_num_rows();

    // Step 1: Initialize base prediction (mean of target values)
    base_predictions.resize(n_samples);
    double initial_prediction = data->get_column(label_column).mean();
    std::fill(base_predictions.begin(), base_predictions.end(), initial_prediction);

    for (int i = 0; i < num_trees; ++i) {
        // Step 2: Compute residuals
        std::vector<Cell> residuals(n_samples);
        for (int j = 0; j < n_samples; ++j) {
            double true_value = DataFrame::double_cast(data->retrieve(j, label_column));
            residuals[j] = true_value - base_predictions[j];
        }

        // Step 3: Train a decision tree to predict residuals
        std::shared_ptr<DataFrame> residual_data = data->copy();

        // printf("Residuals:\n");
        // printf("%s\n", residual_data->print().c_str());
        // printf("\n\n");

        Series residual_series(residuals); // Cast to Series object
        residual_data->set_column(label_column, residual_series);

        auto tree = std::make_unique<DecisionTree>(max_depth, min_samples_split);  // Smaller trees for boosting
        tree->fit(residual_data, label_column);

        // Step 4: Update predictions with a fraction of the tree's predictions (controlled by learning_rate)

        residual_data->drop_column(label_column);  // Drop the residuals column
        for (int j = 0; j < n_samples; ++j) {
            vector<Cell> sample = residual_data->get_row(j);
            // convert to vector<double>
            vector<double> sample_doubles;
            for (const auto& cell : sample) {
                sample_doubles.push_back(DataFrame::double_cast(cell));
            }

            base_predictions[j] += learning_rate * tree->predict(sample_doubles);
        }

        trees.push_back(std::move(tree));
    }
}

double GradientBoostedTrees::predict(const std::vector<double>& sample) const {
    if (trees.empty()) {
        throw std::runtime_error("Model has not been trained yet.");
    }

    double prediction = base_predictions[0];  // Start with the initial prediction
    for (const auto& tree : trees) {
        prediction += learning_rate * tree->predict(sample);
    }
    return prediction;
}


