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
/* ----------------- SEREIS CLASS --------------------*/
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
        Series convertToNumericClasses() const;

        /**
         * @brief Calculates the median of the Series, assuming it is numeric
         * @return double median of the column
         * @throws std::runtime_error if the column is empty or not numeric
         * 
         * This function calculates the median of the specified column.
         */
        double median() const;

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
        std::vector<Cell>::iterator begin();

        /**
         * @brief Function to get an iterator to the end of the Series
         * @return Iterator to the end of the Series
         * 
         * This function returns an iterator to the end of the Series.
         */
        std::vector<Cell>::iterator end();

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
        vector<string> columns; ///< Vector of column names



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

    public:

        /**
         * @brief Constructor for DataFrame
         * @param data 2D matrix of data
         * @param columns Vector of column names
         */
        DataFrame();
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
        static string str_cast(Cell cell);



        /**
         * @brief Function to get the column names of the DataFrame
         * @return Vector of column names
         */
        int get_num_rows();

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
         * df.addColumn("a");
         * df.addColumn("b");
         * df.addColumn("c");
         * df.addColumn("d");
         * 
         * df.addRow({0,0,0,0});
         * df.addRow({1,0,1,0});
         * df.addRow({0,2,0,2});
         * df.addRow({3,3,3,3});
         * df.addRow({4,0,0,4});;
         * 
         * Series column_b = df.get_column("b");
         * Series expected_b = {0, 0, 2, 3, 0};
         * 
         * printf("Column correctly gives %s : %s", column_b, column_b == expected_b ? "TRUE" : "FALSE" );
         * @endcode
         */
        Series get_column(string col_name);

        /**
        * @brief Adds a column to the DataFrame.
        * @param name Column name.
        */
        void addColumn(const std::string& name);

        /**
        * @brief Adds a row of values to the DataFrame.
        * @param values A vector of Cell (int, double, or string) matching column count.
        */
        void addRow(const std::vector<Cell>& values);

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
         * df.addColumn("a");
         * df.addColumn("b");
         * df.addColumn("c");
         * df.addColumn("d");
         * 
         * df.addRow({0,0,0,0});
         * df.addRow({1,0,1,0});
         * df.addRow({0,2,0,2});
         * df.addRow({3,3,3,3});
         * df.addRow({4,0,0,4});
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

        
};


#endif // DATAFRAME_H