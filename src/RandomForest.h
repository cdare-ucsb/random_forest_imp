#include <vector>
#include <memory>
#include <future>
#include <string>


#include "DecisionTree.h"
#include "DataFrame.h"
#include "Classifier.h"

using std::vector;


/**
 * @class RandomForest
 * @brief Random Forest class
 * 
 * This class represents a Random Forest model, which is an ensemble learning method that constructs a multitude
 * of decision trees at training time and outputs the class that is the mode of the classes (classification) of the 
 * individual trees. Random decision forests correct for decision trees' habit of overfitting to their training set.
 * The Random Forest class provides methods for fitting the model to the data and making predictions. 
 */

class RandomForest : public Classifier {

    private:
        std::vector<std::shared_ptr<DecisionTree>> trees; ///< Vector of decision trees in the forest
        size_t num_trees;   ///< Number of trees in the forest
        size_t num_features; ///< Number of features to consider when looking for the best split
        size_t max_depth; ///< Maximum depth of the trees
        size_t min_samples_split; ///< Minimum number of samples required to split a node
        size_t random_state; ///< Random seed for the random number generator
        
        std::vector<std::string> full_feature_names; ///< Original feature names; used when mapping the features back to the original dataset
        std::map<std::shared_ptr<DecisionTree>, std::vector<std::string>> tree_feature_map; ///< Map of decision trees to the features they were trained on
        mutable std::mutex map_mutex; ///< Mutex to protect the tree_feature_map

        /**
         * @brief Function to get the index of a column in the DataFrame
         * @return the index of the specified column
         * 
         * This function returns the index of the specified column in the original DataFrame that 
         * the RandomForest was trained on.
         */
        size_t original_feature_index(const std::string& feature_name) const;

        /**
         * @brief Function to perform a majority vote on the predictions
         * @return the majority vote prediction
         * 
         * This function performs a majority vote on the predictions from the individual trees in the forest.
         * The function returns the majority vote prediction.
         */
        double majorityVote(const std::vector<double>& predictions) const;


    public:

        /**
         * @brief Constructor for RandomForest
         * @param num_trees Number of trees in the forest
         * @param max_depth Maximum depth of the trees
         * @param min_samples_split Minimum number of samples required to split a node
         * @param num_features Number of features to consider when looking for the best split
         * 
         * Constructor for the RandomForest class. The constructor initializes the RandomForest with the specified
         * number of trees, maximum depth of the trees, minimum number of samples required to split a node, and number
         * of features to consider when looking for the best split.
         */
        RandomForest(int num_trees, int max_depth, int min_samples_split, int num_features);

        /**
         * @brief Constructor for RandomForest with random seed
         * @param num_trees Number of trees in the forest
         * @param max_depth Maximum depth of the trees
         * @param min_samples_split Minimum number of samples required to split a node
         * @param num_features Number of features to consider when looking for the best split
         * @param random_state Random seed for the random number generator
         * 
         * Constructor for the RandomForest class. The constructor initializes the RandomForest with the specified
         * number of trees, maximum depth of the trees, minimum number of samples required to split a node, number
         * of features to consider when looking for the best split, and random seed for the random number generator.
         */
        RandomForest(int num_trees, int max_depth, int min_samples_split, int num_features, size_t random_state);


        /**
         * @brief Function to fit the RandomForest to the data
         * @param data Data to fit the RandomForest to
         * @param label_column Name of the column containing the labels
         * 
         * This function fits the RandomForest to the data by training the individual decision trees in the forest.
         * The function takes a DataFrame containing the data and the name of the column containing the labels.
         * For each decision tree, bootstrap samples are created from the data, and the tree is trained on the samples.
         */
        void fit(std::shared_ptr<DataFrame> data, const std::string& label_column) override;

        /**
         * @brief Function to make predictions using the RandomForest
         * @param sample Sample to make predictions on
         * @return Prediction from the RandomForest
         * 
         * This function makes predictions using the RandomForest on the specified sample.
         * The function returns the prediction from the RandomForest.
         */
        double predict(const std::vector<double>& sample) const override;


        /**
         * @brief Function to print the RandomForest
         * @return String representation of the RandomForest
         * 
         * This function returns a string representation of the RandomForest by printing the
         * individual decision trees in the forest.
         */
        std::string print();

        /**
         * @brief Function to perform hyperparameter tuning for the RandomForest
         * @param data Data to perform hyperparameter tuning on
         * @param label_column Name of the column containing the labels
         * @param num_folds Number of folds for cross-validation
         * @param seed Random seed for the random number generator
         * @param num_trees_values Vector of values for the number of trees
         * @param max_depth_values Vector of values for the maximum depth
         * @param min_samples_split_values Vector of values for the minimum samples split
         * @param num_features_values Vector of values for the number of features
         * @return Tuple of hyperparameters with the best accuracy
         * 
         * This function performs hyperparameter tuning for the RandomForest by training the model on different
         * hyperparameter values and evaluating the accuracy using cross-validation. The function returns a tuple
         * of hyperparameters with the best accuracy.
         * 
         * The function takes a DataFrame containing the data, the name of the column containing the labels, the number
         * of folds for cross-validation, a random seed for the random number generator, and vectors of values for the
         * number of trees, maximum depth, minimum samples split, and number of features.
         * 
         * The function returns a tuple of hyperparameters with the best accuracy. The hyperparameters are the number of trees,
         * maximum depth, minimum samples split, and number of features.
         * 
         * The function uses cross-validation to evaluate the accuracy of the model with different hyperparameter values.
         * It trains the model on a training set consisting of all folds except the current fold and evaluates the accuracy
         * on the current fold. The function returns the average accuracy over all folds for each set of hyperparameters.
         * 
         * @code
         * 
         * // Create a DataFrame from a CSV file
         * std::uniqe_ptr<DataFrame> data = DataFrame::read_csv("data.csv");
         * 
         * 
         * // Perform hyperparameter tuning
         * std::vector<int> num_trees_values = {1, 2, 4, 8};
         * std::vector<int> max_depth_values = {1, 2, 3, 4};
         * std::vector<int> min_samples_split_values = {1, 2, 3, 4};
         * std::vector<int> num_features_values = {1, 2, 3};
         * size_t num_folds = 3;
         * size_t random_state = 123456;
         * 
         * auto [best_num_trees, best_max_depth, best_min_samples_split, best_num_features] = RandomForest::hypertune(std::move(data),
         *                                                                                    "label", num_folds, random_state,
         *                                                                                   num_trees_values, max_depth_values,
         *                                                                                   min_samples_split_values, num_features_values);
         * 
         * @endcode
         */
         
        static std::tuple<int,int,int,int> hypertune(std::shared_ptr<DataFrame> data, const std::string& label_column, size_t num_folds, size_t seed,
                             const std::vector<int>& num_trees_values,
                             const std::vector<int>& max_depth_values,
                            const std::vector<int>& min_samples_split_values,
                             const std::vector<int>& num_features_values,
                             bool verbose = false);

        double score(std::shared_ptr<DataFrame> data, const std::string& label_column);

};