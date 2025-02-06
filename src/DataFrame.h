#ifndef DATAFRAME_H
#define DATAFRAME_H

using std::vector;
using std::string;
using std::unique_ptr;


/**
 * @class DataFrame
 * @brief DataFrame class
 * 
 * This class represents a DataFrame object, which is a formatted 2D matrix of data along with column names.
 */
class DataFrame {
    protected:
        vector<vector<double>> data; ///< 2D matrix of data

        /**
         * @brief Function to calculate the entropy of a set of labels
         * @param labels vector of labels
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
        double calculateEntropy(const vector<double>& labels);
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
        double calculateInformationGain(int attributeIndex, string label_column);

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is less than the threshold
         * @param attributeIndex Index of the attribute to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is less than the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being less than the threshold.
         */
        unique_ptr<DataFrame> filter_lt(int attributeIndex, double threshold);

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is less than or equal to the threshold
         * @param attributeIndex Index of the attribute to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is less than or equal to the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being less than or equal to the threshold.
         */
        unique_ptr<DataFrame> filter_leq(int attributeIndex, double threshold);

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is greater than the threshold
         * @param attributeIndex Index of the attribute to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is greater than the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being greater than the threshold.
         */
        unique_ptr<DataFrame> filter_gt(int attributeIndex, double threshold);

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is greater than or equal to the threshold
         * @param attributeIndex Index of the attribute to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is greater than or equal to the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being greater than or equal to the threshold.
         */
        unique_ptr<DataFrame> filter_geq(int attributeIndex, double threshold);

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is equal to the threshold
         * @param attributeIndex Index of the attribute to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is equal to the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being equal to the threshold.
         */
        unique_ptr<DataFrame> filter_eq(int attributeIndex, double threshold);

        /**
         * @brief Helper function to filter all rows where the value of the attribute at the given index is not equal to the threshold
         * @param attributeIndex Index of the attribute to filter on
         * @param threshold Threshold value for the filter
         * @return DataFrame containing only the rows where the value of the attribute is not equal to the threshold
         * 
         * This function filters the rows of the DataFrame based on the value of the attribute at the given index being not equal to the threshold.
         */
        unique_ptr<DataFrame> filter_neq(int attributeIndex, double threshold);


    public:
        vector<string> columns; ///< Vector of column names

        /**
         * @brief Constructor for DataFrame
         * @param data 2D matrix of data
         * @param columns Vector of column names
         */
        DataFrame(std::vector<vector<double>> data, vector<string> columns);
        /**
         * @brief Destructor for DataFrame
         */
        ~DataFrame();

        /**
         * @brief Function to get the data of the DataFrame
         * @return 2D matrix of data
         */
        vector<vector<double>> get_data();

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
         * vector<vector<double>> sample = {
         * {0,0,0,0},
         * {1,0,1,0},
         * {0,2,0,2},
         * {3,3,3,3},
         * {4,0,0,4}};
         * 
         * DataFrame df(sample, {"a", "b", "c", "d"});
         * 
         * vector<double> column_b = df.get_column("b");
         * vector<double> expected_b = {0, 0, 2, 3, 0};
         * 
         * printf("Column correctly gives %s : %s", column_b, column_b == expected_b ? "TRUE" : "FALSE" );
         * @endcode
         */
        vector<double> get_column(string col_name);

        /**
         * @brief Function which prints the DataFrame as a table
         * @return string representation of the DataFrame
         */
        string print();

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
        int selectBestAttribute(string label_column);

        /**
         * @brief Calculates the median of a column
         * @param col_name Name of the column
         * @return double median of the column
         * @throws std::out_of_range if the column name is not found
         * 
         * This function calculates the median of the specified column.
         */
        double column_median(string col_name);

        /**
         * @brief Calculates the mode of a column
         * @param col_name Name of the column
         * @return double mode of the column
         * @throws std::out_of_range if the column name is not found
         * 
         * This function calculates the mode of the specified column.
         */
        double column_mode(string col_name);

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
        unique_ptr<DataFrame> filter(int attributeIndex, double threshold, string condition);

        
};


#endif // DATAFRAME_H