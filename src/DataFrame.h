#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <variant>
#include <unordered_map>

using std::vector;
using std::string;
using std::unique_ptr;

using Cell = std::variant<int,double,string>;



/* -------------------------------------------------- */
/* -------------------------------------------------- */
/* ----------------- SERIES CLASS --------------------*/
/* -------------------------------------------------- */
/* -------------------------------------------------- */


/**
 * @class Series
 * @brief Series class, representing a column of data
 * 
 * This class represents a Series object, which is a column of data values. The data values can be of type int, double, or string.
 * The Series class provides methods for adding data values, retrieving data values, and performing operations on the data.
 * 
 */
class Series {

    protected :
        vector<Cell> data; ///< Vector of data values

    public:
        bool is_numeric; ///< Flag indicating whether the data is numeric

        /**
         * @brief Constructor for Series
         */
        Series();
        /**
         * @brief Constructor for Series
         * @param data Vector of data values
         */
        Series(vector<Cell> data);

        /**
         * @brief Destructor for Series
         */
        ~Series();

        /**
         * @brief Function to add a data value to the Series
         * @param value Data value to add
         * 
         * This function adds a data value to the Series. The function also updates the is_numeric flag based on the type of the data value.
         */
        void push_back(Cell value);
        /**
         * @brief Function to retrieve a data value from the Series
         * @return Data value at the specified index
         * @throws std::runtime_error if the index is out of bounds
         * @param row Index of the data value to retrieve
         * 
         * This function retrieves the data value at the specified index from the Series.
         */
        Cell retrieve(size_t row) const;

        /**
         * @brief Function to get the size of the Series
         * @return Size of the Series
         * 
         * This function returns the number of data values in the Series.
         */
        size_t size() const;

        /**
         * @brief Function to check if the Series is empty
         * @return true if the Series is empty, false otherwise
         * 
         * This function checks if the Series is empty, i.e., if it contains no data values.
         */
        bool empty() const;

        /**
         * @brief Function to print the Series
         * @return string representation of the Series
         * 
         * This function returns a string representation of the Series, with each data value separated by a space.
         */
        string print() const;

        /**
         * @brief Calculates the mode / most frequent entry of the Series
         * @return mode (i.e. most frequent entry) of the column as a Cell
         * @throws runtime_error if the column is empty
         * 
         * This function calculates the mode of the specified column.
         */
        Cell mode() const;


        /**
         * @brief Function to convert the Series to numeric classes
         * @return Series containing the data values converted to numeric classes
         * @throws std::runtime_error if the column is not entirely numeric
         * 
         * This function converts the data values of the Series to numeric classes and returns a new Series containing the converted values.
         * This can be thought of as a generalization of one-hot encoding, where each unique value in the column is assigned a unique numeric class.
         */
        Series numeric_classes() const;

        /**
         * @brief Calculates the median of the Series, assuming it is numeric
         * @return double median of the column
         * @throws std::runtime_error if the column is empty or not numeric
         * 
         * This function calculates the median of the specified column.
         */
        double median() const;


        /**
         * @brief Calculates the mean of the Series, assuming it is numeric
         * @return double mean of the column
         * @throws std::runtime_error if the column is empty or not numeric
         * 
         * This function calculates the mean of the specified column.
         */
        double mean() const;

        /**
         * @brief Function to add two Series together
         * @param other Series to add
         * @return Series containing the sum of the two Series
         * @throws std::runtime_error if the sizes of the two Series do not match or the types are not compatible
         * 
         * This function adds the data values of the two Series together and returns a new Series containing the sum.
         * The function checks that the sizes of the two Series match and that the data values are compatible for addition.
         * 
         * @code
         * Series col1 = Series({1, 2, 3, 4, 5});
         * Series col2 = Series({4, 8, 3, -1, -7});
         * 
         * Series sum = col1 + col2;
         * 
         * printf("Sum of the two Series is {5, 10, 6, 3, -2}: %s", sum == Series({5, 10, 6, 3, -2}) ? "TRUE" : "FALSE");
         * 
         * @endcode
         * 
         */
        Series operator+(const Series& other) const;

        /**
         * @brief Function to access a data value in the Series
         * @param index Index of the data value to access
         * @return Reference to the data value at the specified index
         * @throws std::out_of_range if the index is out of bounds
         * @see Series::retrieve
         * 
         * This function allows access to a data value in the Series using the [] operator.
         */
        Cell& operator[](size_t index);

        /**
         * @brief Function to get an iterator to the beginning of the Series
         * @return Iterator to the beginning of the Series
         * 
         * This function returns an iterator to the beginning of the Series.
         */
        std::vector<Cell>::const_iterator begin() const;

        /**
         * @brief Function to get an iterator to the end of the Series
         * @return Iterator to the end of the Series
         * 
         * This function returns an iterator to the end of the Series.
         */
        std::vector<Cell>::const_iterator end() const;

        /**
         * @brief Function to compare two Series for equality
         * @param other Series to compare
         * @return true if the two Series are equal, false otherwise
         * 
         * This function compares two Series for equality. Two Series are considered equal if they have the same size and contain the same data values in the same order.
         * 
         * @code
         * 
         * 
         * Series col1 = Series({1, 2, 3, 4, 5});
         * Series col2 = Series({1, 2, 3, 4, 5});
         * Series col3 = Series({1, 2, 3, 4, 6});
         * 
         * //TRUE
         * printf("col1 and col2 are equal: %s", col1 == col2 ? "TRUE" : "FALSE");
         * //FALSE
         * printf("col1 and col3 are equal: %s", col1 == col3 ? "TRUE" : "FALSE");
         * @endcode
         */
        bool operator==(const Series& other) const;

        /**
         * @brief Helper function to convert a column to numeric values
         * @param column_name Name of the column to convert
         * @return Vector of numeric values
         * @throws std::out_of_range if the column name is not found
         * @throws std::runtime_error if the column does not entirely consist of numeric data
         * @see Series::convert_to_string
         * 
         * This function converts the values of the specified column to numeric values and returns them as a vector.
         */
        vector<double> convert_to_numeric() const;


        /**
         * @brief Helper function to convert a column to string values
         * @param column_name Name of the column to convert
         * @return Vector of string values
         * @throws std::out_of_range if the column name is not found
         * @throws std::runtime_error if the column does not entirely consist of string data
         * @see Series::convert_to_numeric
         * 
         * This function converts the values of the specified column to string values and returns them as a vector.
         */
        vector<string> convert_to_string() const;

        /**
         * @brief Function to calculate the entropy of a set of labels
         * @param column_name Name of the column containing the labels
         * @return double entropy of the set of labels
         * 
         * The entropy of a set of labels is calculated as follows:
         * 1. Count the frequency of each unique label in the set.
         * 2. For each unique label, calculate the probability of that label by dividing its frequency by the total number of labels.
         * 3. For each unique label, calculate the term -p * log2(p), where p is the probability of the label.
         * 4. Sum all the terms calculated in step 3 to get the entropy.
         * 
         * Entropy is a measure of the impurity or disorder of the set of labels. A higher entropy value indicates more disorder.
         */
        double calculateEntropy() const;


};








/* -------------------------------------------------- */
/* -------------------------------------------------- */
/* ----------------DATAFRAME CLASS -------------------*/
/* -------------------------------------------------- */
/* -------------------------------------------------- */


/**
 * @class DataFrame
 * @brief DataFrame class
 * 
 * This class represents a DataFrame object, which is a formatted 2D matrix of data along with column names.
 */
class DataFrame {
    protected:

        std::unordered_map<string, Series> data; ///< Map of column names to column data



        /**
         * @brief Function to calculate the information gain of an attribute
         * @param attributeIndex Index of the attribute for which to calculate information gain
         * @param label_column Name of the column containing the labels
         * @return double information gain of the attribute
         * 
         * Information gain is a measure of the reduction in entropy that results from splitting a set of data on a particular attribute.
         * The information gain of an attribute is calculated as follows:
         * 1. Calculate the entropy of the set of labels.
         * 2. For each unique value of the attribute, calculate the weighted average of the entropy of the labels after splitting on that value.
         * 3. Calculate the information gain as the entropy of the set of labels minus the weighted average entropy after splitting.
         * 4. Return the information gain.
         */
        double calculateInformationGain(string attribute_column, string label_column) const;

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is less than the threshold
         * @param column_name Name of the column to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is less than the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being less than the threshold.
         */
        unique_ptr<DataFrame> numeric_filter_lt(string column_name, double threshold) const;

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is less than or equal to the threshold
         * @param column_name Name of the column to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is less than or equal to the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being less than or equal to the threshold.
         */
        unique_ptr<DataFrame> numeric_filter_leq(string column_name, double threshold) const;

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is greater than the threshold
         * @param column_name Name of the column to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is greater than the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being greater than the threshold.
         */
        unique_ptr<DataFrame> numeric_filter_gt(string column_name, double threshold) const;

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is greater than or equal to the threshold
         * @param column_name Name of the column to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is greater than or equal to the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being greater than or equal to the threshold.
         */
        unique_ptr<DataFrame> numeric_filter_geq(string column_name, double threshold) const ;

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is equal to the threshold
         * @param column_name Name of the column to filter on
         * @param value Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is equal to the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being equal to the threshold.
         */
        unique_ptr<DataFrame> filter_eq(string column_name, Cell value) const;

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is not equal to the threshold
         * @param attributeIndex Index of the attribute to filter on
         * @param value Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is not equal to the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being not equal to the threshold.
         */
        unique_ptr<DataFrame> filter_neq(string column_name, Cell value) const;

        /**
         * @brief Helper function to print a single cell of the data frame
         * @param cell Cell to print
         * @return string representation of the cell
         * @see DataFrame::print
         * 
         * This function returns a string representation of a single cell of the data frame.
         * The representation depends on the type of the cell (int, double, or string).
         * This helper function is primarily used by the DataFrame::print function.
         */
        string cellToString(const Cell& cell) const;

        /**
         * @brief Helper function to check if a string is an integer
         * @param str String to check
         * @return true if the string is an integer, false otherwise
         * @see is_double
         * 
         * This function checks if a string is an integer by attempting to convert it to an integer.
         */
        static bool is_integer(const std::string& str);

        /**
         * @brief Helper function to check if a string is a double
         * @param str String to check
         * @return true if the string is a double, false otherwise
         * @see is_integer
         * 
         * This function checks if a string is a double by attempting to convert it to a double.
         */
        static bool is_double(const std::string& str);

    public:

        vector<string> columns; ///< Vector of column names

        /**
         * @brief Constructor for DataFrame
         * @param data 2D matrix of data
         * @param columns Vector of column names
         */
        DataFrame();

        /**
         * @brief Constructor for DataFrame
         * @param data 2D matrix of data
         * @param columns Vector of column names
         */
        DataFrame(const vector<vector<double>>& data, const vector<string>& columns);
        /**
         * @brief Destructor for DataFrame
         */
        ~DataFrame();


        /**
         * @brief Constructor for DataFrame
         * @param data 2D matrix of data
         * @param columns Vector of column names
         * 
         * Static function to cast a Cell to an integer
         */
        static int int_cast(Cell cell);

        /**
         * @brief Static function to cast a Cell to a double
         * @param cell Cell to cast
         * @return double value of the Cell
         * 
         * Static function to cast a Cell to a double
         */
        static double double_cast(Cell cell);

        /**
         * @brief Static function to cast a Cell to a double
         * @param cell Cell to cast
         * @return double value of the Cell
         * 
         * Static function to cast a Cell to a double
         */
        static string str_cast(Cell cell);



        /**
         * @brief Function to get the number of rows / samples in a data frame
         * @return number of rows
         */
        size_t get_num_rows() const;

        /**
         * @brief Function to get the number of columns in a data frame
         * @return number of columns
         */
        size_t get_num_columns() const;


        /**
         * @brief Function to get the index of a column in the DataFrame
         * @return the index of the specified column
         * @throws std::out_of_range if the column name is not found
         * 
         * This function returns the index of the specified column in the DataFrame.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * size_t index = df.get_column_index("c");
         * //TRUE
         * printf("Index of column 'c' is 2: %s", index == 2 ? "TRUE" : "FALSE");
         */
        size_t get_column_index(string col_name);

        /**
         * @brief Function to get the number of columns in the DataFrame
         * @return Number of columns
         * @throws std::out_of_range if the column name is not found
         * 
         * This function returns a vector containing the values of the specified column.
         * 
         * @code
         * DataFrame df;
         * 
         * vector<vector<double>> data = {
         *  {0,0,0,0},
         *  {1,0,1,0},
         *  {0,2,0,2},
         *  {3,3,3,3},
         *  {4,0,0,4}
         * };
         * vector<string> columns = {"a", "b", "c", "d"};
         *
         * unique_ptr<DataFrame> df = std::make_unique<DataFrame>(data, columns);
         * 
         * Series column_b = df.get_column("b");
         * Series expected_b = {0, 0, 2, 3, 0};
         * 
         * printf("Column correctly gives %s : %s", column_b, column_b == expected_b ? "TRUE" : "FALSE" );
         * @endcode
         */
        Series get_column(string col_name) const;

        /**
        * @brief Adds a column to the DataFrame.
        * @param name Column name.
        */
        void add_column(const std::string& name);

        void add_column(const std::string& name, const Series& column);

        /**
        * @brief Adds a row of values to the DataFrame.
        * @param values A vector of Cell (int, double, or string) matching column count.
        */
        void add_row(const std::vector<Cell>& values);

        /**
         * @brief Function to get a row of data from the DataFrame
         * @param row Index of the row to retrieve
         * @return Vector of data values in the row
         * @throws std::out_of_range if the row index is out of bounds
         * 
         * This function returns a vector containing the data values in the specified row of the DataFrame.
         */
        vector<Cell> get_row(size_t row) const;

        /**
         * @brief Function to retrieve a data value from the DataFrame
         * @param row Index of the row
         * @param col Name of the column --- this can be either a string or an integer
         * @return Data value at the specified row and column
         * @throws std::out_of_range if the column name is not found
         * 
         * This function retrieves the data value at the specified row and column from the DataFrame.
         * The column can be specified using either the column name or the column index.
         * 
         * @code
         * DataFrame df;
         * 
         * df.add_column("a");
         * df.add_column("b");
         * df.add_column("c");
         * df.add_column("d");
         * 
         * df.add_row({0,0,0,0});
         * df.add_row({1,0,1,0});
         * df.add_row({0,2,0,2});
         * df.add_row({3,3,3,3});
         * df.add_row({4,0,0,4});
         * 
         * printf("Value at row 2, column 'c' is 0: %s", df.retrieve(2, "c") == 0 ? "TRUE" : "FALSE");
         * @endcode
         * 
         */
        Cell retrieve(size_t row, Cell col);

        /**
         * @brief Function which prints the DataFrame as a table
         * @return string representation of the DataFrame
         */
        string print() const;

        /**
         * @brief Function which calculates the attribute with the greatest information gain
         * @param label_column Name of the column containing the labels
         * @return Index of the attribute with the greatest information gain
         * 
         * This function calculates the information gain for each attribute in the DataFrame and returns the
         * index of the attribute with the greatest information gain.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0, 0, 0, 0},
         * {-4, 2, 0.5, 1},
         * {0.1, 4.1, 0, 2},
         * {-2.9, 6.2, 3, 3},
         * {0.1, 8.3,  0, 4}};      // Notice the label is roughly one half the value of the second column
         * 
         * std::vector<std::string> columns = {"Feature1", "Feature2", "Feature3", "Label"};
         * 
         * DataFrame df(sample, columns);
         * int best_attribute = df.selectBestAttribute("Label");
         * 
         * printf("Best attribute is %s: %s\n", columns[1], best_attribute == 1 ? "TRUE" : "FALSE");
         * @endcode
         */
        string selectBestAttribute(string label_name);

        /**
         * @brief Function to create a bootstrap sample of the DataFrame
         * @return DataFrame containing a bootstrap sample of the data
         * 
         * This function creates a bootstrap sample of the DataFrame by randomly sampling rows with replacement.
         * The function returns a new DataFrame containing the bootstrap sample.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * std::unique_ptr<DataFrame> bootstrap_sample = df.bootstrapSample();
         * 
         * printf("Bootstrap sample has the same number of rows: %s", bootstrap_sample->get_num_rows() == df.get_num_rows() ? "TRUE" : "FALSE");
         * @endcode
         */
        unique_ptr<DataFrame> bootstrap_sample();

        /**
         * @brief Function to create a bootstrap sample of the DataFrame with a specified number of features
         * @param num_features Number of features to sample
         * @param label_column Name of the column containing the labels
         * @return DataFrame containing a bootstrap sample of the data with the specified number of features
         * 
         * This function creates a bootstrap sample of the DataFrame by randomly sampling rows with replacement and selecting a subset of features.
         * The function returns a new DataFrame containing the bootstrap sample with the specified number of features.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * std::unique_ptr<DataFrame> bootstrap_sample = df.bootstrap_sample(2, "d");
         * 
         * printf("Bootstrap sample has the same number of rows: %s", bootstrap_sample->get_num_rows() == df.get_num_rows() ? "TRUE" : "FALSE");
         * printf("Bootstrap sample has the correct number of features: %s", bootstrap_sample->get_num_columns() == 2 ? "TRUE" : "FALSE");
         * @endcode
         */
        unique_ptr<DataFrame> bootstrap_sample(size_t num_features, string label_column);

        /**
         * @brief Function to create a bootstrap sample of the DataFrame with a specified number of features and random state
         * @param num_features Number of features to sample
         * @param label_column Name of the column containing the labels
         * @param random_state Random seed for sampling
         * @return DataFrame containing a bootstrap sample of the data with the specified number of features
         * 
         * This function creates a bootstrap sample of the DataFrame by randomly sampling rows with replacement and selecting a subset of features.
         * The function returns a new DataFrame containing the bootstrap sample with the specified number of features.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * std::unique_ptr<DataFrame> bootstrap_sample = df.bootstrap_sample(2, "d", 42);
         * 
         * printf("Bootstrap sample has the same number of rows: %s", bootstrap_sample->get_num_rows() == df.get_num_rows() ? "TRUE" : "FALSE");
         * printf("Bootstrap sample has the correct number of features: %s", bootstrap_sample->get_num_columns() == 2 ? "TRUE" : "FALSE");
         * @endcode
         */
        unique_ptr<DataFrame> bootstrap_sample(size_t num_features, string label_column, size_t random_state);

        /**
         * @brief Function to filter the DataFrame based on a condition
         * @param attributeIndex Index of the attribute to filter on
         * @param threshold Threshold value for the filter
         * @param condition Condition for the filter (e.g., "lt", "leq", "gt", "geq", "eq", "neq")
         * @return DataFrame containing only the rows that satisfy the condition
         * @throws std::invalid_argument if the condition is not recognized
         * 
         * This function filters the rows of the DataFrame based on the specified condition. The condition can be one of the following:
         * - "<" (less than)
         * - "<=" (less than or equal to)
         * - ">" (greater than)
         * - ">=" (greater than or equal to)
         * - "==" (equal to)
         * - "!=" (not equal to)
         * 
         * The function returns a new DataFrame containing only the rows that satisfy the condition, using the specified attribute and threshold.
         * 
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * std::unique_ptr<DataFrame> filtered_df = df.filter(0, 1, "<");
         * std::vector<std::vector<double>> expected_data = {
         * {0,0,0,0},
         * {0,2,0,2}
         * };
         * 
         * printf("Filtered DataFrame correctly gives %s : %s", expected_data, filtered_df->get_data() == expected_data ? "TRUE" : "FALSE" );
         * @endcode
         */
        unique_ptr<DataFrame> filter(string column_name, Cell threshold, string condition);

        /**
         * @brief Function to get the head of the DataFrame
         * @param num_rows Number of rows to include in the head
         * @return DataFrame containing the head of the data
         * 
         * This function returns a new DataFrame containing the first num_rows rows of the DataFrame.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * std::unique_ptr<DataFrame> head = df.head(3);
         * std::vector<std::vector<double>> expected_data = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2}
         * };
         * 
         * printf("Head of DataFrame correctly gives %s : %s", expected_data, head->get_data() == expected_data ? "TRUE" : "FALSE" );
         * @endcode
         */
        unique_ptr<DataFrame> head(size_t num_rows) const;

        /**
         * @brief Function to get the tail of the DataFrame
         * @param num_rows Number of rows to include in the tail
         * @return DataFrame containing the tail of the data
         * 
         * This function returns a new DataFrame containing the last num_rows rows of the DataFrame.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * std::unique_ptr<DataFrame> tail = df.tail(3);
         * std::vector<std::vector<double>> expected_data = {
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}
         * };
         * 
         * printf("Tail of DataFrame correctly gives %s : %s", expected_data, tail->get_data() == expected_data ? "TRUE" : "FALSE" );
         * @endcode
         */
        unique_ptr<DataFrame> tail(size_t num_rows) const;


        /**
         * @brief Function to drop a column from the DataFrame
         * @param column_name Name of the column to drop
         * 
         * This function removes the specified column from the DataFrame.
         */
        void drop_column(string column_name);

        /**
         * @brief Function which converts a row of string data to numeric categories
         * @param col_name Name of the column to convert to numeric catagories
         * 
         * This function converts a column of string data to numeric categories, which is necessary
         * for some some classification algorithms. Thhe algorithms iterates through the data and 
         * assigns a unique numeric value to each unique string value.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0, "Yes"},
         * {1,0,1,0, "No"},
         * {0,2,0,2, "Yes"},
         * {3,3,3,3, "No"},
         * {4,0,0,4, "No"}};
         * DataFrame df(sample, {"a", "b", "c", "d", "e"});
         * 
         * df.one_hot_encode("e");
         * 
         * printf("Column 'e' is now numeric: %s", df.get_column("e").is_numeric() ? "TRUE" : "FALSE");
         */
        void one_hot_encode(string col_name);

        
        /**
         * @brief Function to read a CSV file into a DataFrame
         * @param file_path Path to the CSV file
         * @return DataFrame containing the data from the CSV file
         * 
         * This function reads a CSV file into a DataFrame. The CSV file is expected to have a header row with column names.
         * The function returns a DataFrame containing the data from the CSV file.
         * 
         * @code
         * std::unique_ptr<DataFrame> df = DataFrame::read_csv("data.csv");
         * @endcode
         */
        static unique_ptr<DataFrame> read_csv(const std::string& file_path);
        

        /**
         * @brief Function to split the DataFrame into training and testing sets
         * @param percent_training Percentage of the data to include in the training set
         * @return Pair of DataFrames containing the training and testing sets
         * 
         * This function splits the DataFrame into training and testing sets based on the specified percentage.
         * The function returns a pair of DataFrames containing the training and testing sets.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * std::pair<std::unique_ptr<DataFrame>, std::unique_ptr<DataFrame>> train_test = df.split_train_test(0.8);
         * 
         * printf("Training set has 80%% of the data: %s", train_test.first->get_num_rows() == 4 ? "TRUE" : "FALSE");
         * printf("Testing set has 20%% of the data: %s", train_test.second->get_num_rows() == 1 ? "TRUE" : "FALSE");
         * @endcode
         */
        std::pair<unique_ptr<DataFrame>, unique_ptr<DataFrame>> split_train_test(double percent_training);


        /**
         * @brief Function to split the DataFrame into training and testing sets with a specified random state
         * @param percent_training Percentage of the data to include in the training set
         * @param random_state Random seed for splitting the data
         * @return Pair of DataFrames containing the training and testing sets
         * 
         * This function splits the DataFrame into training and testing sets based on the specified percentage and random seed.
         * The function returns a pair of DataFrames containing the training and testing sets.
         * 
         * @code
         * std::vector<std::vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * std::pair<std::unique_ptr<DataFrame>, std::unique_ptr<DataFrame>> train_test = df.split_train_test(0.8, 42);
         * 
         * printf("Training set has 80%% of the data: %s", train_test.first->get_num_rows() == 4 ? "TRUE" : "FALSE");
         * printf("Testing set has 20%% of the data: %s", train_test.second->get_num_rows() == 1 ? "TRUE" : "FALSE");
         * @endcode
         */
        std::pair<unique_ptr<DataFrame>, unique_ptr<DataFrame>> split_train_test(double percent_training, size_t random_state);
        
        vector<unique_ptr<DataFrame>> split_k_fold(size_t n_folds);
        vector<unique_ptr<DataFrame>> split_k_fold(size_t n_folds, size_t seed);

        unique_ptr<DataFrame> copy() const;
};


#endif // DATAFRAME_H