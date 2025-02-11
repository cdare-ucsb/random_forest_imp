#include <gtest/gtest.h>
#include "../src/DataFrame.h"
#include "../src/DecisionTree.h"
#include "../src/Node.h"
#include "../src/GradientBoostedTrees.h"
#include <vector>




TEST(GradientBoostedTreesTest, FitTest) {
    int num_trees = 3;
    double learning_rate = 0.1;
    int max_depth = 3;
    int min_samples_split = 1;

    GradientBoostedTrees gb(num_trees, learning_rate, max_depth, min_samples_split);
    unique_ptr<DataFrame> df = DataFrame::read_csv("../../samples/seattle-weather.csv")->head(20);

    // Clean data
    df->drop_column("date");
    df->one_hot_encode("weather");
    vector<string> columns = df->columns;

    gb.fit(std::move(df), "weather");

    EXPECT_EQ(gb.predict({0.0, 12.8, 5.0, 4.7}), 1);
    EXPECT_EQ(gb.predict({1.0, 9.8, -1.0, 6.7}), 1);
    EXPECT_EQ(gb.predict({4.0, 1.8, -3.0, 2.7}), 3);

    EXPECT_THROW(gb.predict({0.0, 12.8, 5.0, 4.7, 0.0}), std::runtime_error);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}