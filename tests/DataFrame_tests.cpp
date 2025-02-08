#include <gtest/gtest.h>
#include "../src/DataFrame.h"
#include <vector>

using std::vector;
using std::unique_ptr;
using std::string;

/**
 * @brief Unit Test for data frame operations
 * 
 * @test Test creation of data frame, including adding rows and columns
 */
TEST(DataFrameTest, DataFrameCreation) {
    
    DataFrame df;

    df.addColumn("a");
    df.addColumn("b");
    df.addColumn("c");
    df.addColumn("d");

    df.addRow({0,0,0,0});
    df.addRow({1,0,1,0});
    df.addRow({0,2,0,2});
    df.addRow({3,3,3,3});
    df.addRow({4,0,0,4});

    EXPECT_THROW(df.addRow({0,0,0}), std::runtime_error);
    
    // Check if the data is correctly stored
    EXPECT_EQ(DataFrame::int_cast(df.retrieve(1, "a")), 1);
    EXPECT_EQ(DataFrame::int_cast(df.retrieve(3, 3)), 3);
    EXPECT_EQ(df.get_num_rows(), 5);
}


/**
 * @brief Unit Test for data frame operations
 * 
 * @test Test Data Frame retrieval and get_column functionality
 */
TEST(DataFrameTest, DataFrameRetrieval) {
    DataFrame df;

    df.addColumn("a");
    df.addColumn("b");
    df.addColumn("c");
    df.addColumn("d");

    df.addRow({0,0,0,0});
    df.addRow({1,0,1,0});
    df.addRow({0,2,0,2});
    df.addRow({3,3,3,3});
    df.addRow({4,0,0,4});

    Series col = df.get_column("a");
    printf("Column a: %s", col.print().c_str());
    printf("\n");
    EXPECT_EQ(DataFrame::int_cast(col.retrieve(0)), 0);
    EXPECT_THROW(df.retrieve(6, "a"), std::runtime_error);


    EXPECT_THROW(df.get_column("z"), std::invalid_argument);
}


/**
 * @brief Unit Test for data frame operations
 * 
 * @test Test Data Frame attribute selection
 */
TEST(DataFrameTest, DataFrameBestAttributeSelection) {
    DataFrame df;

    df.addColumn("a");
    df.addColumn("b");
    df.addColumn("c");
    df.addColumn("d");

    df.addRow({0,0,0,0});
    df.addRow({1,0,1,0});
    df.addRow({0,2,0,2});
    df.addRow({3,3,3,3});
    df.addRow({4,0,0,4});

    // Check if the best attribute is correctly selected
    EXPECT_EQ(df.selectBestAttribute("d"), "a");
    EXPECT_EQ(df.selectBestAttribute("a"), "d");
    EXPECT_EQ(df.selectBestAttribute("b"), "d");
    EXPECT_EQ(df.selectBestAttribute("c"), "a");


    // Check that invalid arguments throw exceptions
    EXPECT_THROW(df.selectBestAttribute("z"), std::invalid_argument);
    EXPECT_THROW(df.selectBestAttribute("e"), std::invalid_argument);
    EXPECT_THROW(df.selectBestAttribute("f"), std::invalid_argument);
}

/**
 * @brief Unit Test for data frame operations
 * 
 * @test Test data frame print function
 */
TEST(DataFrameTest, DataFramePrint) {
    DataFrame df;

    df.addColumn("a");
    df.addColumn("b");
    df.addColumn("c");
    df.addColumn("d");

    df.addRow({0,0,0,0});
    df.addRow({1,0,1,0});
    df.addRow({0,2,0,2});
    df.addRow({3,3,3,3});
    df.addRow({4,0,0,4});

    EXPECT_EQ(df.print(), "-----------------\n| a | b | c | d |\n-----------------\n| 0 | 0 | 0 | 0 |\n| 1 | 0 | 1 | 0 |\n| 0 | 2 | 0 | 2 |\n| 3 | 3 | 3 | 3 |\n| 4 | 0 | 0 | 4 |\n-----------------\n");
}




/**
 * @brief Unit tests for data frame operations
 * 
 * @test Test the filtering functionality 
 */
TEST(DataFrameTest, DataFrameFilter) {
    DataFrame df;

    df.addColumn("a");
    df.addColumn("b");
    df.addColumn("c");
    df.addColumn("d");

    df.addRow({0,0,0,0});
    df.addRow({1,0,1,0});
    df.addRow({0,2,0,2});
    df.addRow({3,3,3,3});
    df.addRow({4,0,0,4});

    unique_ptr<DataFrame> filtered_df1 = df.filter("a", 2, "<");
    EXPECT_EQ(filtered_df1->get_num_rows(),3);
    
    DataFrame df2;
    df2.addColumn("Temp");
    df2.addColumn("Day");
    df2.addColumn("IsWeekend");
    df2.addColumn("Humidity");

    df2.addRow({32.4, "Sat", "Yes", 10.1});
    df2.addRow({36.2, "Sun", "Yes", 9.2});
    df2.addRow({30.4, "Mon", "No", 9.4});
    df2.addRow({39.5, "Tue", "No", 9.6});
    df2.addRow({42.1, "Wed", "No", 13.1});
    df2.addRow({44.3, "Thu", "No", 12.9});
    df2.addRow({40.9, "Fri", "No", 11.1});
    df2.addRow({41.5, "Sat", "Yes", 8.5});
    df2.addRow({38.1, "Sun", "Yes", 8.1});
    df2.addRow({37.7, "Mon", "No", 10.2});

    unique_ptr<DataFrame> filtered_df2 = df2.filter("IsWeekend", "Yes", "!=");
    EXPECT_EQ(filtered_df2->get_num_rows(),6);

    EXPECT_THROW(df2.filter("IsWeekend", "No", "<"), std::invalid_argument);
}

/**
 * @brief Unit tests for data frame operations
 * 
 * @test Test the dataframe and series conversion
 */

TEST(DataFrameTest, DataFrameSeriesConversion) {
    DataFrame df;

    df.addColumn("a");
    df.addColumn("b");
    df.addColumn("c");
    df.addColumn("d");

    df.addRow({0,0,0,0});
    df.addRow({1,0,1,0});
    df.addRow({0,2,0,2});
    df.addRow({3,3,3,3});
    df.addRow({4,0,0,4});

    Series col = df.get_column("a");

    EXPECT_EQ(DataFrame::int_cast(df.retrieve(0, "a")), 0);
    EXPECT_EQ(DataFrame::int_cast(df.retrieve(3, "a")), 3);
    EXPECT_EQ(DataFrame::int_cast(df.retrieve(4, "a")), 4);

    DataFrame df2;
    df2.addColumn("Temp");
    df2.addColumn("Day");
    df2.addColumn("IsWeekend");
    df2.addColumn("Humidity");

    df2.addRow({32.4, "Sat", "Yes", 10.1});
    df2.addRow({36.2, "Sun", "Yes", 9.2});
    df2.addRow({30.4, "Mon", "No", 9.4});
    df2.addRow({39.5, "Tue", "No", 9.6});
    df2.addRow({42.1, "Wed", "No", 13.1});
    df2.addRow({44.3, "Thu", "No", 12.9});
    df2.addRow({40.9, "Fri", "No", 11.1});
    df2.addRow({41.5, "Sat", "Yes", 8.5});
    df2.addRow({38.1, "Sun", "Yes", 8.1});
    df2.addRow({37.7, "Mon", "No", 10.2});

    EXPECT_THROW(DataFrame::int_cast(df2.retrieve(0, "Day")), std::invalid_argument);
}

/**
 * @brief Unit tests for Series operations
 * 
 * @test Test the addition of series
 */
TEST(SeriesTest, SeriesAddition) {
    DataFrame df;

    df.addColumn("a");
    df.addColumn("b");
    df.addColumn("c");
    df.addColumn("d");

    Series col1 = Series({0,0,0,0});
    Series col2 = Series({1,0,1,0});
    Series col3 = Series({0,2,0,2});
    Series col4 = Series({3,3,3,3});
    Series col5 = Series({4,0,0,4});

    Series sum = col1 + col2 + col3 + col4 + col5;

    EXPECT_EQ(DataFrame::int_cast(sum.retrieve(0)), 8);
    EXPECT_EQ(DataFrame::int_cast(sum.retrieve(1)), 5);
    EXPECT_EQ(DataFrame::int_cast(sum.retrieve(2)), 4);
    EXPECT_EQ(DataFrame::int_cast(sum.retrieve(3)), 9);
}


TEST(SeriesTest, SeriesEquality) {
    Series col1 = Series({1, 2, 3, 4, 5});
    Series col2 = Series({1, 2, 3, 4, 5});
    Series col3 = Series({1, 2, 3, 4, 6});

    EXPECT_TRUE(col1 == col2);
    EXPECT_FALSE(col1 == col3);
}   


TEST(SeriesTest, SeriesMedian) {
    Series col1 = Series({1, 2, 3, 4, 5});
    Series col2 = Series({1, 2, 3, 4, 6});
    Series col3 = Series({1, 2, 3, 4, 7, 8});

    EXPECT_EQ(col1.median(), 3);
    EXPECT_EQ(col2.median(), 3);
    EXPECT_EQ(col3.median(), 3.5);
}

TEST(SeriesTest, SeriesMode) {
    Series col1 = Series({0, 2, 0, 4, 5});
    Series col2 = Series({1, 2, 3, 3, 0, 1});
    Series col3 = Series({"Y", "N", "Y", "N", "Y", "Y"});

    EXPECT_EQ(DataFrame::int_cast(col1.mode()), 0);
    EXPECT_EQ(DataFrame::int_cast(col2.mode()), 3);
    EXPECT_EQ(DataFrame::str_cast(col3.mode()), "Y");
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}