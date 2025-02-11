#include <gtest/gtest.h>
#include "../src/DataFrame.h"
#include "../src/DecisionTree.h"
#include "../src/Node.h"
#include "../src/RandomForest.h"
#include <vector>


/**
 * @brief Unit Tests for the RandomForest class
 * 
 * @test Test the RandomForest constructor with different parameters
 */
TEST(RandomForestTest, RandomForestPredict) {

    int num_trees = 3;
    int max_depth = 3;
    int min_samples_split = 1;
    int num_features = 2;
    RandomForest rf(num_trees, max_depth, min_samples_split, num_features, 123456);

    
    unique_ptr<DataFrame> df = DataFrame::read_csv("../../samples/seattle-weather.csv")->head(20);


    // Clean data
    df->drop_column("date");
    df->one_hot_encode("weather");
    vector<string> columns = df->columns;


    rf.fit(std::move(df), "weather");

    EXPECT_EQ(rf.predict({0.0, 12.8, 5.0, 4.7}), 1);
    EXPECT_EQ(rf.predict({1.0, 9.8, -1.0, 6.7}), 1);
    EXPECT_EQ(rf.predict({4.0, 1.8, -3.0, 2.7}), 3);

    EXPECT_THROW(rf.predict({0.0, 12.8, 5.0, 4.7, 0.0}), std::runtime_error);
}

/**
 * @brief Unit Tests for the RandomForest class
 * 
 * @test Test the RandomForest predict with a larger number of trees
 */
TEST(RandomForestTest, RandomForestPredict2) {
    
    int num_trees = 6;
    int max_depth = 4;
    int min_samples_split = 1;
    int num_features = 3;
    RandomForest rf(num_trees, max_depth, min_samples_split, num_features, 123456);

    
    unique_ptr<DataFrame> df = DataFrame::read_csv("../../samples/seattle-weather.csv")->head(100);

    // Clean data
    df->drop_column("date");
    df->one_hot_encode("weather");
    vector<string> columns = df->columns;

    rf.fit(std::move(df), "weather");

    EXPECT_EQ(rf.predict({0.0, 12.8, 5.0, 4.7}), 0);
    EXPECT_EQ(rf.predict({1.0, 9.8, -1.0, 6.7}), 1);
    EXPECT_EQ(rf.predict({4.0, 1.8, -3.0, 2.7}), 1);


}

/**
 * @brief Unit Tests for the RandomForest class
 * 
 * @test Test the hyperparamter tuning for the Random Forest; this test generally takes much longer than the other tests combined
 */
TEST(RandomForestTest, RandomForestHypertune) {

    unique_ptr<DataFrame> df = DataFrame::read_csv("../../samples/seattle-weather.csv")->head(100);

    // Clean data
    df->drop_column("date");
    df->one_hot_encode("weather");
    vector<string> columns = df->columns;

    vector<int> num_trees_values = {1, 2, 4, 8};
    vector<int> max_depth_values = {1, 2, 3, 4};
    vector<int> min_samples_split_values = {1, 2, 3, 4};
    vector<int> num_features_values = {1, 2, 3};


    auto [best_num_trees, best_max_depth, best_min_samples_split, best_num_features] = RandomForest::hypertune(std::move(df),
                                                                                    "weather", 3, 123456,
                                                                                    num_trees_values,
                                                                                    max_depth_values,
                                                                                    min_samples_split_values,
                                                                                    num_features_values);

    EXPECT_EQ(best_num_trees, 8);
    EXPECT_EQ(best_max_depth, 3);
    EXPECT_EQ(best_min_samples_split, 1);
    EXPECT_EQ(best_num_features, 2);
}



int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}