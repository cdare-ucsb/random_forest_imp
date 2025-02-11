#include <gtest/gtest.h>
#include "../src/DataFrame.h"
#include <vector>
#include <variant>

using std::vector;
using std::unique_ptr;
using std::string;

using Cell = std::variant<int,double,std::string>;

/**
 * @brief Unit Test for data frame operations
 * 
 * @test Test creation of data frame, including adding rows and columns
 */
TEST(DataFrameTest, DataFrameCreation) {
    
    DataFrame df;

    df.add_column("a");
    df.add_column("b");
    df.add_column("c");
    df.add_column("d");

    df.add_row({0,0,0,0});
    df.add_row({1,0,1,0});
    df.add_row({0,2,0,2});
    df.add_row({3,3,3,3});
    df.add_row({4,0,0,4});

    EXPECT_THROW(df.add_row({0,0,0}), std::runtime_error);
    
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

    df.add_column("a");
    df.add_column("b");
    df.add_column("c");
    df.add_column("d");

    df.add_row({0,0,0,0});
    df.add_row({1,0,1,0});
    df.add_row({0,2,0,2});
    df.add_row({3,3,3,3});
    df.add_row({4,0,0,4});

    Series col = df.get_column("a");
    printf("Column a: %s", col.print().c_str());
    printf("\n");
    EXPECT_EQ(DataFrame::int_cast(col.retrieve(0)), 0);
    EXPECT_THROW(df.retrieve(6, "a"), std::out_of_range);


    EXPECT_THROW(df.get_column("z"), std::invalid_argument);
}


/**
 * @brief Unit Test for data frame operations
 * 
 * @test Test Data Frame attribute selection
 */
TEST(DataFrameTest, DataFrameBestAttributeSelection) {
    DataFrame df;

    df.add_column("a");
    df.add_column("b");
    df.add_column("c");
    df.add_column("d");

    df.add_row({0,0,0,0});
    df.add_row({1,0,1,0});
    df.add_row({0,2,0,2});
    df.add_row({3,3,3,3});
    df.add_row({4,0,0,4});

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

    df.add_column("a");
    df.add_column("b");
    df.add_column("c");
    df.add_column("d");

    df.add_row({0,0,0,0});
    df.add_row({1,0,1,0});
    df.add_row({0,2,0,2});
    df.add_row({3,3,3,3});
    df.add_row({4,0,0,4});

    EXPECT_EQ(df.print(), "-----------------\n| a | b | c | d |\n-----------------\n| 0 | 0 | 0 | 0 |\n| 1 | 0 | 1 | 0 |\n| 0 | 2 | 0 | 2 |\n| 3 | 3 | 3 | 3 |\n| 4 | 0 | 0 | 4 |\n-----------------\n");
}




/**
 * @brief Unit tests for data frame operations
 * 
 * @test Test the filtering functionality 
 */
TEST(DataFrameTest, DataFrameFilter) {
    DataFrame df;

    df.add_column("a");
    df.add_column("b");
    df.add_column("c");
    df.add_column("d");

    df.add_row({0,0,0,0});
    df.add_row({1,0,1,0});
    df.add_row({0,2,0,2});
    df.add_row({3,3,3,3});
    df.add_row({4,0,0,4});

    unique_ptr<DataFrame> filtered_df1 = df.filter("a", 2, "<");
    EXPECT_EQ(filtered_df1->get_num_rows(),3);
    
    DataFrame df2;

    df2.add_column("Temp");
    df2.add_column("Day");
    df2.add_column("IsWeekend");
    df2.add_column("Humidity");

    df2.add_row({32.4, "Sat", "Yes", 10.1});
    df2.add_row({36.2, "Sun", "Yes", 9.2});
    df2.add_row({30.4, "Mon", "No", 9.4});
    df2.add_row({39.5, "Tue", "No", 9.6});
    df2.add_row({42.1, "Wed", "No", 13.1});
    df2.add_row({44.3, "Thu", "No", 12.9});
    df2.add_row({40.9, "Fri", "No", 11.1});
    df2.add_row({41.5, "Sat", "Yes", 8.5});
    df2.add_row({38.1, "Sun", "Yes", 8.1});
    df2.add_row({37.7, "Mon", "No", 10.2});


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

    vector<vector<double>> data = {
        {0,0,0,0},
        {1,0,1,0},
        {0,2,0,2},
        {3,3,3,3},
        {4,0,0,4}
    };
    vector<string> columns = {"a", "b", "c", "d"};

    unique_ptr<DataFrame> df = std::make_unique<DataFrame>(data, columns);


    EXPECT_EQ(DataFrame::int_cast(df->retrieve(0, "a")), 0);
    EXPECT_EQ(DataFrame::int_cast(df->retrieve(3, "a")), 3);
    EXPECT_EQ(DataFrame::int_cast(df->retrieve(4, "a")), 4);

    DataFrame df2;
    df2.add_column("Temp");
    df2.add_column("Day");
    df2.add_column("IsWeekend");
    df2.add_column("Humidity");

    df2.add_row({32.4, "Sat", "Yes", 10.1});
    df2.add_row({36.2, "Sun", "Yes", 9.2});
    df2.add_row({30.4, "Mon", "No", 9.4});
    df2.add_row({39.5, "Tue", "No", 9.6});
    df2.add_row({42.1, "Wed", "No", 13.1});
    df2.add_row({44.3, "Thu", "No", 12.9});
    df2.add_row({40.9, "Fri", "No", 11.1});
    df2.add_row({41.5, "Sat", "Yes", 8.5});
    df2.add_row({38.1, "Sun", "Yes", 8.1});
    df2.add_row({37.7, "Mon", "No", 10.2});

    EXPECT_THROW(DataFrame::int_cast(df2.retrieve(0, "Day")), std::invalid_argument);
}


TEST(DataFrameTest, ReadFromCSVTest) {

    unique_ptr<DataFrame> df = DataFrame::read_csv("../../samples/seattle-weather.csv");

    EXPECT_EQ(df->get_num_rows(), 1461);
    EXPECT_EQ(df->get_num_columns(), 6);

    unique_ptr<DataFrame> df2 = df->head(5);
    printf("%s\n", df2->print().c_str());

    EXPECT_EQ(df2->get_num_rows(), 5);
    EXPECT_EQ(df2->get_num_columns(), 6);
    EXPECT_EQ(DataFrame::str_cast(df2->retrieve(0, "date")), "2012-01-01");
    EXPECT_EQ(DataFrame::double_cast(df2->retrieve(0, "temp_max")), 12.8);
    EXPECT_EQ(DataFrame::double_cast(df2->retrieve(0, "temp_min")), 5.0);  
    EXPECT_EQ(DataFrame::double_cast(df2->retrieve(0, "wind")), 4.7);
    EXPECT_EQ(DataFrame::str_cast(df2->retrieve(1, "weather")), "rain");
}

TEST(DataFrameTest, DataFrameDropColumn) {
    DataFrame df;

    df.add_column("a");
    df.add_column("b");
    df.add_column("c");
    df.add_column("d");

    df.add_row({0,0,0,0});
    df.add_row({1,0,1,0});
    df.add_row({0,2,0,2});
    df.add_row({3,3,3,3});
    df.add_row({4,0,0,4});

    df.drop_column("a");

    EXPECT_THROW(df.get_column("a"), std::invalid_argument);
    EXPECT_EQ(df.get_num_columns(), 3);

}

TEST(DataFrameTest, OneHotEncodeTest) {
    DataFrame df;

    df.add_column("a");
    df.add_column("b");
    df.add_column("c");
    df.add_column("d");

    df.add_row({0,0,0,"Y"});
    df.add_row({1,0,1,"N"});
    df.add_row({0,2,0,"Y"});
    df.add_row({3,3,3,"N"});
    df.add_row({4,0,0,"N"});


    df.one_hot_encode("d");

    Series col = df.get_column("d");

    EXPECT_EQ(DataFrame::int_cast(col.retrieve(0)), 0);
    EXPECT_EQ(DataFrame::int_cast(col.retrieve(1)), 1);
    EXPECT_EQ(DataFrame::int_cast(col.retrieve(2)), 0);
    EXPECT_EQ(DataFrame::int_cast(col.retrieve(3)), 1);
    EXPECT_EQ(DataFrame::int_cast(col.retrieve(4)), 1);
}


/**
 * @brief Unit tests for Series operations
 * 
 * @test Test the addition of series
 */
TEST(SeriesTest, SeriesAddition) {

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

TEST(SeriesTest, SeriesNumericCatagories) {

    Series col1 = Series({"Y", "N", "Y", "N", "Y", "Y"});
    Series col2 = Series({"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", "Mon", "Tue", "Wed"});

    Series cat1 = col1.numeric_classes();
    Series cat2 = col2.numeric_classes();

    EXPECT_EQ(DataFrame::int_cast(cat1.retrieve(0)), 0);
    EXPECT_EQ(DataFrame::int_cast(cat1.retrieve(1)), 1);
    EXPECT_EQ(DataFrame::int_cast(cat1.retrieve(2)), 0);
    EXPECT_EQ(DataFrame::int_cast(cat1.retrieve(3)), 1);
    EXPECT_EQ(DataFrame::int_cast(cat1.retrieve(4)), 0);
    EXPECT_EQ(DataFrame::int_cast(cat1.retrieve(5)), 0);

    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(0)), 0);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(1)), 1);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(2)), 2);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(3)), 3);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(4)), 4);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(5)), 5);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(6)), 6);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(7)), 0);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(8)), 1);
    EXPECT_EQ(DataFrame::int_cast(cat2.retrieve(9)), 2);
}


TEST(DataFrameTest, BootstrapSampleTest) {
    DataFrame df;

    df.add_column("a");
    df.add_column("b");
    df.add_column("c");
    df.add_column("d");

    df.add_row({0,0,0,0});
    df.add_row({1,0,1,0});
    df.add_row({0,2,0,2});
    df.add_row({3,3,3,3});
    df.add_row({4,0,0,4});


    unique_ptr<DataFrame> sample = df.bootstrap_sample(3, "d", 54313);

    


    EXPECT_EQ(sample->get_num_rows(), 5);
    EXPECT_EQ(sample->get_num_columns(), 4);
    EXPECT_EQ(DataFrame::int_cast(sample->retrieve(0, "a")), 4);
    EXPECT_EQ(DataFrame::int_cast(sample->retrieve(2, "c")), 0);
    EXPECT_EQ(DataFrame::int_cast(sample->retrieve(3, "d")), 4);
    EXPECT_EQ(DataFrame::int_cast(sample->retrieve(1, "b")), 3);

    unique_ptr<DataFrame> sample2 = df.bootstrap_sample(2, "d", 123456);


    EXPECT_EQ(sample2->get_num_rows(), 5);
    EXPECT_EQ(sample2->get_num_columns(), 3);
    EXPECT_EQ(DataFrame::int_cast(sample2->retrieve(0, "c")), 0);
    EXPECT_EQ(DataFrame::int_cast(sample2->retrieve(2, "b")), 0);
    EXPECT_EQ(DataFrame::int_cast(sample2->retrieve(3, "d")), 3);
    EXPECT_THROW(DataFrame::int_cast(sample2->retrieve(1, "a")), std::out_of_range);
}



int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}