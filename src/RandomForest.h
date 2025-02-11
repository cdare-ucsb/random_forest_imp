#include <vector>
#include <memory>
#include <future>
#include <string>


#include "DecisionTree.h"
#include "DataFrame.h"

using std::vector;

class RandomForest {

    private:
        std::vector<std::unique_ptr<DecisionTree>> trees;
        std::map<DecisionTree*, std::vector<std::string>> tree_feature_map;


        size_t num_trees;
        size_t num_features;
        size_t max_depth;
        size_t min_samples_split;
        double min_impurity_decrease;
        size_t max_features;
        size_t bootstrap;
        size_t random_state;
        std::vector<std::string> full_feature_names;

        double majorityVote(const std::vector<double>& predictions) const;
        size_t original_feature_index(const std::string& feature_name) const;

    public:

        RandomForest(int num_trees, int max_depth, int min_samples_split);
        RandomForest(int num_trees, int max_depth, int min_samples_split, size_t random_state);
        RandomForest(int num_trees, int max_depth, int min_samples_split, int num_features);
        RandomForest(int num_trees, int max_depth, int min_samples_split, int num_features, size_t random_state);

        void fit(std::unique_ptr<DataFrame> data, const std::string& label_column);

        double predict(const std::vector<double>& sample) const;

        std::string print();

};