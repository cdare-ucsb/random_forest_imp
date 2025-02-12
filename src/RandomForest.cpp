#include <vector>
#include <memory>
#include <future>
#include <string>
#include <map>
#include <iostream>

#include "DecisionTree.h"
#include "DataFrame.h"
#include "RandomForest.h"

using std::vector;



// Constructors
RandomForest::RandomForest(int num_trees, int max_depth, int min_samples_split, int num_features)
        : num_trees(num_trees), max_depth(max_depth), min_samples_split(min_samples_split), num_features(num_features) {}

RandomForest::RandomForest(int num_trees, int max_depth, int min_samples_split, int num_features, size_t random_state)
        : num_trees(num_trees), max_depth(max_depth), min_samples_split(min_samples_split), num_features(num_features), random_state(random_state) {}



void RandomForest::fit(std::shared_ptr<DataFrame> data, const std::string& label_column) {
    std::vector<std::future<std::shared_ptr<DecisionTree>>> futures;
    full_feature_names = data->columns;

    for (int i = 0; i < num_trees; ++i) {
        futures.push_back(std::async(std::launch::async, [this, &data, label_column, i]() {
            if (num_features == -1) {
                num_features = static_cast<int>(std::sqrt(data->get_num_columns()));
            }
            std::unique_ptr<DataFrame> bootstrap_sample = data->bootstrap_sample(num_features, label_column, random_state + i);

            // Save the feature names used in the bootstrap sample
            std::vector<std::string> selected_features = bootstrap_sample->columns;
            auto tree = std::make_shared<DecisionTree>(max_depth, min_samples_split);
            tree->fit(std::move(bootstrap_sample), label_column);

            // Associate the tree with its selected features; lock with a mutex to ensure there are no race conditions
            {
                std::lock_guard<std::mutex> lock(map_mutex);
                tree_feature_map[tree] = selected_features;
            }

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

    for (const std::shared_ptr<DecisionTree>& tree : trees) {
        std::vector<double> filtered_sample;

        // Get the feature set for this tree
        std::vector<std::string> feature_subset;
        std::lock_guard<std::mutex> lock(map_mutex);
        auto it = tree_feature_map.find(tree);
        if (it != tree_feature_map.end()) {
            feature_subset = it->second;
        } else {
            throw std::runtime_error("Tree not found in tree_feature_map");
        }


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

    // Print each tree in the forest
    for (const auto& tree : trees) {
        std::vector<std::string> feature_subset = tree_feature_map.at(tree);
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


std::tuple<int,int,int,int> RandomForest::hypertune(std::shared_ptr<DataFrame> data, const std::string& label_column, size_t num_folds, size_t seed,
                             const std::vector<int>& num_trees_values,
                             const std::vector<int>& max_depth_values,
                            const std::vector<int>& min_samples_split_values,
                             const std::vector<int>& num_features_values,
                             bool verbose) {
    std::map<std::tuple<int, int, int, int>, double> accuracy_map;
    vector<unique_ptr<DataFrame>> k_folds = data->split_k_fold(num_folds, seed);

    // Calculate total combinations
    size_t total_combinations = num_trees_values.size() * max_depth_values.size() * min_samples_split_values.size() * num_features_values.size();
    size_t current_iteration = 0;

    if (verbose) {
        std::cout << "Performing hyperparameter tuning with " << total_combinations << " combinations\n";
    }

    for (int num_trees : num_trees_values) {
        for (int max_depth : max_depth_values) {
            for (int min_samples_split : min_samples_split_values) {
                for (int num_features : num_features_values) {
                    double all_folds_accuracy = 0.0;
                    // Perform k-fold cross-validation
                    for (size_t i = 0; i < num_folds; ++i) {
                        double single_fold_accuracy = 0.0;
                        // Create a new DataFrame for training data consisting 
                        // of all folds except the current fold
                        std::shared_ptr<DataFrame> train_data = std::make_shared<DataFrame>();
                        
                        for (const auto& col : data->columns) {
                            train_data->add_column(col);
                        }

                        for (size_t j = 0; j < num_folds; ++j) {
                            if (j == i) {
                                continue; // Skip the current fold
                            }
                            size_t num_rows = k_folds[j]->get_num_rows();
                            for (size_t k = 0; k < num_rows; ++k) {
                                train_data->add_row(k_folds[j]->get_row(k));
                            }
                        }

                        RandomForest rf(num_trees, max_depth, min_samples_split, num_features, seed);
                        rf.fit(train_data, label_column); 


                        Series label_column_data = k_folds[i]->get_column(label_column);
                        k_folds[i]->drop_column(label_column);
                        size_t testing_rows = k_folds[i]->get_num_rows();

                        // Measure how many predictions are correct
                        for (size_t j = 0; j < testing_rows; ++j) {
                            vector<Cell> sample = k_folds[i]->get_row(j);


                            // Convert sample to a vector of doubles
                            vector<double> sample_doubles;
                            for (const auto& cell : sample) {
                                sample_doubles.push_back( DataFrame::double_cast(cell) );
                            }

                            double prediction = rf.predict(sample_doubles);
                            if (prediction == DataFrame::double_cast(label_column_data.retrieve(j))) {
                                single_fold_accuracy += 1.0;
                            }

                        }
                        // Accuracy should be the percentage of correct predictions
                        single_fold_accuracy = single_fold_accuracy / testing_rows;
                        // Add row back to the DataFrame
                        k_folds[i]->add_column(label_column, label_column_data);
                        all_folds_accuracy += single_fold_accuracy;
                    }

                    // Average accuracy across all folds
                    accuracy_map[{num_trees, max_depth, min_samples_split, num_features}] = all_folds_accuracy / num_folds;
                    // Update progress bar if verbose is true
                    if (verbose) {
                        current_iteration++;
                        int progress = static_cast<int>((100.0 * current_iteration) / total_combinations);
                        std::cout << "\rProgress: \033[32m[" << std::string(progress / 2, '=') << std::string(50 - progress / 2, ' ')
                                  << "] " << progress << "% complete\033[0m" << std::flush;
                    }
                }
            }
        }
    }

    std::cout << std::endl; // Move to the next line after the progress bar finishes

    // Find the hyperparameters with the highest accuracy
    auto best_hyperparameters = std::max_element(accuracy_map.begin(), accuracy_map.end(),
                                                 [](const auto& a, const auto& b) { return a.second < b.second; });
    return best_hyperparameters->first;

}


double RandomForest::score(std::shared_ptr<DataFrame> data, const std::string& label_column) {

    if (trees.empty()) {
        throw std::runtime_error("RandomForest has not been fit");
    }

    double correct_predictions = 0.0;
    size_t num_rows = data->get_num_rows();

    Series label_column_data = data->get_column(label_column);
    data->drop_column(label_column);

    for (size_t i = 0; i < num_rows; ++i) {
        vector<Cell> sample = data->get_row(i);

        vector<double> sample_doubles;

        for (const auto& cell : sample) {
            sample_doubles.push_back( DataFrame::double_cast(cell) );
        }

        double prediction = predict(sample_doubles);
        if (prediction == DataFrame::double_cast(label_column_data.retrieve(i))) {
            correct_predictions += 1.0;
        }

    }

    data->add_column(label_column, label_column_data);

    return correct_predictions / num_rows;
}