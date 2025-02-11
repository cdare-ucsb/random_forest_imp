#include <gtest/gtest.h>
#include "../src/DataFrame.h"
#include "../src/DecisionTree.h"
#include "../src/Node.h"
#include <vector>

using std::vector;


/**
 * @brief Unit Test for the DecisionTree class
 * 
 * @test test the DecisionTree num_nodes method
 */
TEST(DecisionTreeTest, DecisionTreeNumNodes) {

    DecisionTree dt(3,1);

    // Check if the data is correctly stored
    EXPECT_EQ(dt.get_num_nodes(), 0);


    vector<vector<double>> data1 = {
        {2.5, 1.5, 0},
        {1.0, 3.0, 1},
        {3.5, 2.0, 0},
        {4.0, 3.5, 1},
        {5.0, 2.5, 1}
    };
    vector<string> columns1 = {"A", "B", "C"};
    unique_ptr<DataFrame> df1 = std::make_unique<DataFrame>(data1, columns1);

    DecisionTree dt1(3,1);
    // Train Decision Tree
    dt1.fit(std::move(df1), "C");
    EXPECT_EQ(dt1.get_num_nodes(), 9);
    
    vector<vector<double>> data2 = {
        {2.5, 1.5, 3.4, -2.1, 0},
        {1.0, 3.0, 1.1, 9.2, 1},
        {3.5, 2.0, 13.4, -2.5, 0},
        {4.0, 3.5, 2.3, 11.1, 1},
        {5.0, 2.5, 1.9, 10.5, 1},
        {6.4, 2.0, 15.5, -3, 0},
        {7.2, 3.5, 2.3, 11.1, 1},
        {8.1, 2.5, 1.9, 10.5, 1},
        {3.5, 2.0, 15.5, -3, 0},
        {3.1, 3.5, 2.3, 11.1, 1},
        {2.5, 2.5, 1.9, 10.5, 1},
        {1.1, 2.0, 15.5, -3, 0},
        {9.5, 3.5, 2.3, 11.1, 1},
        {2.5, 2.5, 1.9, 10.5, 1},
        {1.0, 2.0, 15.5, -3, 0},
        {3.5, 3.5, 2.3, 11.1, 1},
        {1.0, 2.5, 1.9, 10.5, 1},
        {1.0, 2.0, 15.5, -3, 0},
        {4.2, 3.5, 2.3, 11.1, 1},
        {4.0, 2.5, 1.9, 10.5, 1},
        {3.5, 2.0, 15.5, -3, 0},
        {2.5, 3.5, 2.3, 11.1, 1},
        {8.1, 2.5, 1.9, 10.5, 1},
        {7.2, 2.0, 15.5, -3, 0},
        {2.5, 3.5, 2.3, 11.1, 1},
        {6.4, 2.5, 1.9, 10.5, 1},
        {11.1, 2.0, 15.5, -3, 0},
        {28.1, 3.4, 14.2, -4, 0}
    };


    vector<string> columns2 = {"A", "B", "C", "D", "E"};
    unique_ptr<DataFrame> df2 = std::make_unique<DataFrame>(data2, columns2);

    DecisionTree dt2(3,1);
    // Train Decision Tree
    dt2.fit(std::move(df2), "E");
    EXPECT_EQ(dt2.get_num_nodes(), 15);
}

/**
 * @brief Unit Test for the DecisionTree class
 * 
 * @test test the DecisionTree predict method
 */
TEST(DecisionTreeTest, DecisionTreePredict) {

    DecisionTree dt(3,1);
    EXPECT_THROW(dt.predict({2.5, 1.5}), std::runtime_error);


    vector<vector<double>> data1 = {
        {2.5, 1.5, 0},
        {1.0, 3.0, 1},
        {3.5, 2.0, 0},
        {4.0, 3.5, 1},
        {5.0, 2.5, 1}
    };
    vector<string> columns1 = {"A", "B", "C"};
    unique_ptr<DataFrame> df1 = std::make_unique<DataFrame>(data1, columns1);

    DecisionTree dt1(3,1);
    // Train Decision Tree
    dt1.fit(std::move(df1), "C");
    EXPECT_EQ(dt1.predict({2.5, 1.5}), 0);
    EXPECT_EQ(dt1.predict({1.0, 3.0}), 1);
    EXPECT_EQ(dt1.predict({3.5, 2.0}), 0);


    vector<vector<double>> data2 = {
        {2.5, 1.5, 3.4, -2.1, 0},
        {1.0, 3.0, 1.1, 9.2, 1},
        {3.5, 2.0, 13.4, -2.5, 0},
        {4.0, 3.5, 2.3, 11.1, 1},
        {5.0, 2.5, 1.9, 10.5, 1},
        {6.4, 2.0, 15.5, -3, 0},
        {7.2, 3.5, 2.3, 11.1, 1},
        {8.1, 2.5, 1.9, 10.5, 1},
        {3.5, 2.0, 15.5, -3, 0},
        {3.1, 3.5, 2.3, 11.1, 1},
        {2.5, 2.5, 1.9, 10.5, 1},
        {1.1, 2.0, 15.5, -3, 0},
        {9.5, 3.5, 2.3, 11.1, 1},
        {2.5, 2.5, 1.9, 10.5, 1},
        {1.0, 2.0, 15.5, -3, 0},
        {3.5, 3.5, 2.3, 11.1, 1},
        {1.0, 2.5, 1.9, 10.5, 1},
        {1.0, 2.0, 15.5, -3, 0},
        {4.2, 3.5, 2.3, 11.1, 1},
        {4.0, 2.5, 1.9, 10.5, 1},
        {3.5, 2.0, 15.5, -3, 0},
        {2.5, 3.5, 2.3, 11.1, 1},
        {8.1, 2.5, 1.9, 10.5, 1},
        {7.2, 2.0, 15.5, -3, 0},
        {2.5, 3.5, 2.3, 11.1, 1},
        {6.4, 2.5, 1.9, 10.5, 1},
        {11.1, 2.0, 15.5, -3, 0},
        {28.1, 3.4, 14.2, -4, 0}
    };


    vector<string> columns2 = {"A", "B", "C", "D", "E"};
    unique_ptr<DataFrame> df2 = std::make_unique<DataFrame>(data2, columns2);

    DecisionTree dt2(3,1);
    // Train Decision Tree
    dt2.fit(std::move(df2), "E");
    EXPECT_EQ(dt2.predict({2.5, 1.5, 3.4, -2.1}), 0);
    EXPECT_EQ(dt2.predict({1.0, 3.0, 1.1, 9.2}), 1);
    EXPECT_EQ(dt2.predict({3.5, 2.0, 13.4, -2.5}), 0);
    EXPECT_EQ(dt2.predict({4.0, 3.5, 2.3, 11.1}), 1);
    EXPECT_EQ(dt2.predict({8.0, 3.5, 2.3, 11.1}), 1);
}

/**
 * @brief Unit Test for the DecisionTree class
 * 
 * @test test the DecisionTree print method
 */
TEST(DecisionTreeTest, DecisionTreePrint) {

    DecisionTree dt(3,1);

    // Check if the data is correctly stored
   
    vector<vector<double>> data1 = {
        {2.5, 1.5, 0},
        {1.0, 3.0, 1},
        {3.5, 2.0, 0},
        {4.0, 3.5, 1},
        {5.0, 2.5, 1}
    };
    vector<string> columns = {"A", "B", "C"};
    unique_ptr<DataFrame> df = std::make_unique<DataFrame>(data1, columns);

    EXPECT_EQ(dt.print(columns), "Empty Decision Tree");
    // Train Decision Tree
    dt.fit(std::move(df), "C");
    EXPECT_EQ(dt.print(columns), "\xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 [ A <= 3.5 ]\n\xE2\x94\x82   \xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 [ A <= 2.5 ]\n\xE2\x94\x82   \xE2\x94\x82   \xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 [ A <= 1.75 ]\n\xE2\x94\x82   \xE2\x94\x82   \xE2\x94\x82   \xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 ( 1 )\n\xE2\x94\x82   \xE2\x94\x82   \xE2\x94\x82   \xE2\x94\x94\xE2\x94\x80\xE2\x94\x80 ( 0 )\n\xE2\x94\x82   \xE2\x94\x82   \xE2\x94\x94\xE2\x94\x80\xE2\x94\x80 ( 0 )\n\xE2\x94\x82   \xE2\x94\x94\xE2\x94\x80\xE2\x94\x80 [ A <= 4.5 ]\n\xE2\x94\x82       \xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 ( 1 )\n\xE2\x94\x82       \xE2\x94\x94\xE2\x94\x80\xE2\x94\x80 ( 1 )\n");
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}