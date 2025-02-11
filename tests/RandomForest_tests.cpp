#include <gtest/gtest.h>
#include "../src/DataFrame.h"
#include "../src/DecisionTree.h"
#include "../src/Node.h"
#include "../src/RandomForest.h"
#include <vector>



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
