#include <gtest/gtest.h>
#include "../src/DataFrame.h"
#include <vector>

using std::vector;
using std::unique_ptr;
using std::string;

// Test 1: DataFrame creation
TEST(DataFrameTest, DataFrameCreation) {
    vector<vector<double>> sample = {{0,1,0,1}, {0,1,0,1}, {0,1,0,1}, {0,1,0,1}, {0,1,0,1}};
    DataFrame df(sample, {"a", "b", "c", "d"});

    // Check if the data is correctly stored
    EXPECT_EQ((df.get_data())[0][1], 1);
    EXPECT_EQ((df.get_data())[1][2], 0);
    EXPECT_EQ(df.columns[0], "a");
    EXPECT_EQ(df.get_num_rows(), 5);

}


// Test 2: DataFrame column retrieval
TEST(DataFrameTest, DataFrameColumnRetrieval) {
    vector<vector<double>> sample = {
        {0,0,0,0},
        {1,0,1,0},
        {0,2,0,2},
        {3,3,3,3},
        {4,0,0,4}
    };
    DataFrame df(sample, {"a", "b", "c", "d"});
    vector<double> column_b = df.get_column("b");
    vector<double> expected_b = {0, 0, 2, 3, 0};

    vector<double> column_d = df.get_column("d");
    vector<double> expected_d = {0, 0, 2, 3, 4};

    vector<double> column_a = df.get_column("a");
    vector<double> expected_a = {0, 1, 0, 3, 4};

    // Ensure the column is correctly retrieved
    for (size_t i = 0; i < column_b.size(); i++) {
        EXPECT_EQ(column_b[i], expected_b[i]);
        EXPECT_EQ(column_d[i], expected_d[i]);
        EXPECT_EQ(column_a[i], expected_a[i]);
    }

    EXPECT_THROW(df.get_column("z"), std::invalid_argument);
}


// Test 5: DataFrame best attribute selection
TEST(DataFrameTest, DataFrameBestAttributeSelection) {
    vector<vector<double>> sample = {
        {0,0,0,0},
        {1,0,1,0},
        {0,2,0,2},
        {3,3,3,3},
        {4,0,0,4}
    };
    DataFrame df(sample, {"a", "b", "c", "d"});

    // Check if the best attribute is correctly selected
    EXPECT_EQ(df.selectBestAttribute("d"), 0);
    EXPECT_EQ(df.selectBestAttribute("a"), 3);
    EXPECT_EQ(df.selectBestAttribute("b"), 3);
    EXPECT_EQ(df.selectBestAttribute("c"), 0);




    vector<vector<double>> sample2 = {
          {0, 0, 0, 0},
          {-4, 2, 0.5, 1},
          {0.1, 4.1, 0, 2},
          {-2.9, 6.2, 3, 3},
          {0.1, 8.3,  0, 4}};      // Notice the label is roughly one half the value of the second column
          
    DataFrame df2(sample2, {"Feature1", "Feature2", "Feature3", "Label"});

    // Check if the best attribute is correctly selected
    EXPECT_EQ(df2.selectBestAttribute("Label"), 1);

    // Check that invalid arguments throw exceptions
    EXPECT_THROW(df.selectBestAttribute("z"), std::invalid_argument);
    EXPECT_THROW(df.selectBestAttribute("e"), std::invalid_argument);
    EXPECT_THROW(df.selectBestAttribute("f"), std::invalid_argument);
}

// Test 6: DataFrame print
TEST(DataFrameTest, DataFramePrint) {
    vector<vector<double>> sample = {
        {0,0,0,0},
        {1,0,1,0},
        {0,2,0,2},
        {3,3,3,3},
        {4,0,0,4}
    };
    DataFrame df(sample, {"a", "b", "c", "d"});

    // Check if the print method works
    string expected_output = "+------------+------------+------------+------------+\n| a          | b          | c          | d          |\n+------------+------------+------------+------------+\n| 0          | 0          | 0          | 0          |\n| 1          | 0          | 1          | 0          |\n| 0          | 2          | 0          | 2          |\n| 3          | 3          | 3          | 3          |\n| 4          | 0          | 0          | 4          |\n+------------+------------+------------+------------+\n";
    EXPECT_EQ(df.print(), expected_output);

    DataFrame df2({}, {});
    EXPECT_EQ(df2.print(), "Empty DataFrame");
}


// Test 7: DataFrame column median
TEST(DataFrameTest, DataFrameColumnMedian) {
    vector<vector<double>> sample = {
        {0,0,0,0},
        {1,0,1,0},
        {0,2,0,2},
        {3,3,3,3},
        {4,0,0,4}
    };
    DataFrame df(sample, {"a", "b", "c", "d"});

    // Check if the median is correctly calculated
    EXPECT_EQ(df.column_median("a"), 1);
    EXPECT_EQ(df.column_median("b"), 0);
    EXPECT_EQ(df.column_median("c"), 0);
    EXPECT_EQ(df.column_median("d"), 2);

    // Check that invalid arguments throw exceptions
    EXPECT_THROW(df.column_median("z"), std::invalid_argument);
    EXPECT_THROW(df.column_median("e"), std::invalid_argument);
    EXPECT_THROW(df.column_median("f"), std::invalid_argument);
}

// Test 8: DataFrame mode
TEST(DataFrameTest, DataFrameMode) {
    vector<vector<double>> sample = {
        {0,0,0,0},
        {1,0,1,0},
        {0,2,0,2},
        {3,3,3,3},
        {4,0,0,4}
    };
    DataFrame df(sample, {"a", "b", "c", "d"});

    // Check if the mode is correctly calculated
    EXPECT_EQ(df.column_mode("a"), 0);
    EXPECT_EQ(df.column_mode("b"), 0);
    EXPECT_EQ(df.column_mode("c"), 0);
    EXPECT_EQ(df.column_mode("d"), 0);

    // Check that invalid arguments throw exceptions
    EXPECT_THROW(df.column_mode("z"), std::invalid_argument);
    EXPECT_THROW(df.column_mode("e"), std::invalid_argument);
    EXPECT_THROW(df.column_mode("f"), std::invalid_argument);
}

// Test 8: DataFrame filter
TEST(DataFrameTest, DataFrameFilter) {
    vector<vector<double>> sample = {
        {0,0,0,0},
        {1,0,1,0},
        {0,2,0,2},
        {3,3,3,3},
        {4,0,0,4}
    };
    DataFrame df(sample, {"a", "b", "c", "d"});

    // Check if the filter method works
    unique_ptr<DataFrame> filtered_df = df.filter(0, 1, "<");
    vector<vector<double>> expected_data = {
        {0,0,0,0},
        {0,2,0,2}
    };
    EXPECT_EQ(filtered_df->get_data(), expected_data);

    unique_ptr<DataFrame> filtered_df2 = df.filter(1, 1, ">");
    vector<vector<double>> expected_data2 = {
        {0,2,0,2},
        {3,3,3,3}
    };
    EXPECT_EQ(filtered_df2->get_data(), expected_data2);

    unique_ptr<DataFrame> filtered_df3 = df.filter(2, 0, "==");
    vector<vector<double>> expected_data3 = {
        {0,0,0,0},
        {0,2,0,2},
        {4,0,0,4}
    };
    EXPECT_EQ(filtered_df3->get_data(), expected_data3);

    unique_ptr<DataFrame> filtered_df4 = df.filter(3, 3, "!=");
    vector<vector<double>> expected_data4 = {
        {0,0,0,0},
        {1,0,1,0},
        {0,2,0,2},
        {4,0,0,4}
    };
    EXPECT_EQ(filtered_df4->get_data(), expected_data4);

    // Check that invalid arguments throw exceptions
    EXPECT_THROW(df.filter(5, 1, "lt"), std::invalid_argument);
    EXPECT_THROW(df.filter(0, 1, "z"), std::invalid_argument);
    EXPECT_THROW(df.filter(3, 3, "lt"), std::invalid_argument);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}