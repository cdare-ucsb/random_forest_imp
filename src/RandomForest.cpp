#include <vector>
#include <memory>
#include <future>
#include <string>
#include <map>

#include "DecisionTree.h"
#include "DataFrame.h"
#include "RandomForest.h"

using std::vector;




RandomForest::RandomForest(int num_trees, int max_depth, int min_samples_split)
        : num_trees(num_trees), max_depth(max_depth), min_samples_split(min_samples_split) {
            num_features = -1;  // -1 indicates that the number of features should be the square root of the total number of features
        }

RandomForest::RandomForest(int num_trees, int max_depth, int min_samples_split, size_t random_state)
        : num_trees(num_trees), max_depth(max_depth), min_samples_split(min_samples_split), random_state(random_state) {
            num_features = -1;  // -1 indicates that the number of features should be the square root of the total number of features
        }

RandomForest::RandomForest(int num_trees, int max_depth, int min_samples_split, int num_features)
        : num_trees(num_trees), max_depth(max_depth), min_samples_split(min_samples_split), num_features(num_features) {}

RandomForest::RandomForest(int num_trees, int max_depth, int min_samples_split, int num_features, size_t random_state)
        : num_trees(num_trees), max_depth(max_depth), min_samples_split(min_samples_split), num_features(num_features), random_state(random_state) {}

void RandomForest::fit(std::unique_ptr<DataFrame> data, const std::string& label_column) {
    std::vector<std::future<std::unique_ptr<DecisionTree>>> futures;
    full_feature_names = data->columns;

    for (int i = 0; i < num_trees; ++i) {
        futures.push_back(std::async(std::launch::async, [this, &data, label_column, i]() {
            if (num_features == -1) {
                num_features = static_cast<int>(std::sqrt(data->get_num_columns()));
            }
            std::unique_ptr<DataFrame> bootstrap_sample = data->bootstrap_sample(num_features, label_column, random_state + i);

            // Save the feature names used in the bootstrap sample
            std::vector<std::string> selected_features = bootstrap_sample->columns;

            auto tree = std::make_unique<DecisionTree>(max_depth, min_samples_split);
            tree->fit(std::move(bootstrap_sample), label_column);

            // Associate the tree with its selected features
            tree_feature_map[tree.get()] = selected_features;

            return tree;
        }));
    }

    for (auto& future : futures) {
        trees.push_back(std::move(future.get()));  // Add the tree to the forest in the same order
    }
}





double RandomForest::predict(const std::vector<double>& sample) const {
    std::vector<double> predictions;

    // ensure sample size is same as feature size - 1 (i.e. removing label column)
    if (sample.size() != full_feature_names.size() - 1) {
        throw std::runtime_error("Sample size does not match the number of non-label features");
    }

    for (const auto& tree : trees) {
        std::vector<double> filtered_sample;

        // Get the feature set for this tree
        std::vector<std::string> feature_subset = tree_feature_map.at(tree.get());

        // Map the full sample to the feature subset
        for (const auto& feature : feature_subset) {
            size_t index = original_feature_index(feature);  // Use original_feature_index to find the index of each feature
            filtered_sample.push_back(sample[index]);
        }

        predictions.push_back(tree->predict(filtered_sample));
    }

    return majorityVote(predictions);
}


std::string RandomForest::print() {
    if (trees.empty()) {
        return "Empty Random Forest";
    }

    string output_string = "Random forest of length " + std::to_string(num_trees) + " with trees:\n";
    for (const auto& tree : trees) {
        std::vector<std::string> feature_subset = tree_feature_map.at(tree.get());
        output_string += tree->print(feature_subset) + "\n";
    }
    return output_string;
}



size_t RandomForest::original_feature_index(const std::string& feature_name) const {
    // Iterate over the full_feature_names vector to find the matching feature
    for (size_t i = 0; i < full_feature_names.size(); ++i) {
        if (full_feature_names[i] == feature_name) {
            return i;  // Return the index of the matching feature
        }
    }

    // If the feature name is not found, throw an error
    throw std::runtime_error("Feature name not found in original dataset: " + feature_name);
}



double RandomForest::majorityVote(const std::vector<double>& predictions) const {
    std::map<double, int> frequency_map;
    for (double pred : predictions) {
        frequency_map[pred]++;
    }

    return std::max_element(frequency_map.begin(), frequency_map.end(),
                            [](const auto& a, const auto& b) { return a.second < b.second; })->first;
}
