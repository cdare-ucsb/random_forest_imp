#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>

#include "DataFrame.h"

using std::vector;
using std::string;

/* -------------------------------------------
*             CONSTRUCTOR/DESTRUCTOR
----------------------------------------------*/
DataFrame::DataFrame(std::vector<vector<double>> data, vector<string> columns) {
    this->data = data;
    this->columns = columns;    
}

// Destructor
DataFrame::~DataFrame() {}

/* -------------------------------------------
*                  GETTERS
----------------------------------------------*/
vector<vector<double>> DataFrame::get_data() {
    return this->data;
}



vector<double> DataFrame::get_column(string col_name) {
    // Find the index of the column
    size_t col_idx = -1;
    for (size_t i = 0; i < this->columns.size(); i++) {
        if (this->columns[i] == col_name) {
            col_idx = i;
        }
    }
    // Throw an exception if the column is not found
    if (col_idx == -1) {
        throw std::invalid_argument("Column not found");
    }

    // Create an empty column with the same size as the number of rows
    vector<double> column(this->data.size()); 
    for (size_t row = 0; row < this->data.size(); row++) {
        column[row] = this->data[row][col_idx];
    }
    return column;
}

int DataFrame::get_num_rows() {
    return this->data.size();
}


/* -------------------------------------------
*             PRINT FUNCTION
----------------------------------------------*/
string DataFrame::print() {

    // If the DataFrame is empty, return an empty DataFrame message
    if (this->data.empty()) {
        return "Empty DataFrame";
    }

    string output = "";
    size_t num_columns = this->columns.size();
    for (size_t i = 0; i < num_columns; i++) {
        output += this->columns[i] + "\t\t";
    }
    output += "\n";
    for (size_t i = 0; i < num_columns; i++) {
        output += "--------------";
    }
    output += "\n";

    for (size_t i = 0; i < this->data.size(); i++) {

        for (size_t j = 0; j < this->data[i].size(); j++) {
            output += std::to_string(this->data[i][j]) + "\t";
        }
        output += "\n";
        for (size_t j = 0; j < this->data[i].size(); j++) {
            output += "--------------";
        }
        output += "\n";
    }
    return output;
}



/* -------------------------------------------
*             FILTER FUNCTIONS
----------------------------------------------*/


DataFrame* DataFrame::filter(int attributeIndex, double threshold, string condition) {
    if (condition == "<") {
        return this->filter_lt(attributeIndex, threshold);
    } else if (condition == "<=") {
        return this->filter_leq(attributeIndex, threshold);
    } else if (condition == ">") {
        return this->filter_gt(attributeIndex, threshold);
    } else if (condition == ">=") {
        return this->filter_geq(attributeIndex, threshold);
    } else if (condition == "==") {
        return this->filter_eq(attributeIndex, threshold);
    } else if (condition == "!=") {
        return this->filter_neq(attributeIndex, threshold);
    } else {
        throw std::invalid_argument("Invalid condition");
    }
}

DataFrame* DataFrame::filter_lt(int attributeIndex, double threshold) {

    if(attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (size_t i = 0; i < this->data.size(); i++) {
        if (this->data[i][attributeIndex] < threshold) {
            filtered_data.push_back(this->data[i]);
        }
    }

    return new DataFrame(filtered_data, this->columns);
}

DataFrame* DataFrame::filter_leq(int attributeIndex, double threshold) {

    if(attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (size_t i = 0; i < this->data.size(); i++) {
        if (this->data[i][attributeIndex] <= threshold) {
            filtered_data.push_back(this->data[i]);
        }
    }

    return new DataFrame(filtered_data, this->columns);
}

DataFrame* DataFrame::filter_gt(int attributeIndex, double threshold) {

    if(attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (size_t i = 0; i < this->data.size(); i++) {
        if (this->data[i][attributeIndex] > threshold) {
            filtered_data.push_back(this->data[i]);
        }
    }

    return new DataFrame(filtered_data, this->columns);
}

DataFrame* DataFrame::filter_geq(int attributeIndex, double threshold) {
    if(attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (size_t i = 0; i < this->data.size(); i++) {
        if (this->data[i][attributeIndex] >= threshold) {
            filtered_data.push_back(this->data[i]);
        }
    }

    return new DataFrame(filtered_data, this->columns);
}

DataFrame* DataFrame::filter_eq(int attributeIndex, double threshold) {
    if(attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (size_t i = 0; i < this->data.size(); i++) {
        if (this->data[i][attributeIndex] == threshold) {
            filtered_data.push_back(this->data[i]);
        }
    }

    return new DataFrame(filtered_data, this->columns);
}

DataFrame* DataFrame::filter_neq(int attributeIndex, double threshold) {
    if(attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }
    
    vector<vector<double>> filtered_data;
    for (size_t i = 0; i < this->data.size(); i++) {
        if (this->data[i][attributeIndex] != threshold) {
            filtered_data.push_back(this->data[i]);
        }
    }

    return new DataFrame(filtered_data, this->columns);
}



/* -------------------------------------------
*             STATISTICS FUNCTIONS
----------------------------------------------*/

// Function to calculate entropy of a dataset
double DataFrame::calculateEntropy(const vector<double>& labels) {

    std::map<double, double> frequency;
    for (double label : labels) {
        frequency[label]++;
    }

    double entropy = 0.0;
    int total = labels.size();

    for (auto& [label, count] : frequency) {
        double probability = static_cast<double>(count) / total;
        entropy -= probability * log2(probability);
    }

    return entropy;
}



// Function to calculate information gain of an attribute
double DataFrame::calculateInformationGain(int attributeIndex, string label_column) {

    // Check if the attribute index is valid
    if (attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    if (this->data.empty()) {
        throw std::invalid_argument("No data in the DataFrame");
    }

    // Extract the target column
    size_t label_idx = -1;
    for (size_t i = 0; i < this->columns.size(); i++) {
        if (this->columns[i] == label_column) {
            label_idx = i;
        }
    }
    // Check if label column is in the valid
    if (label_idx == -1) {
        throw std::invalid_argument("Label column not in list of columns");
    }
    if (attributeIndex == label_idx) {
        throw std::invalid_argument("Cannot calculate information gain for the label column");
    }



    vector<double> labels = this->get_column(label_column);
    double initialEntropy = this->calculateEntropy(labels);

    // Splitting the dataset based on the attribute
    std::map<double, vector<double>> subsets;
    for (size_t i = 0; i < this->data.size(); ++i) {
        subsets[this->data[i][attributeIndex]].push_back(labels[i]);
    }

    // Compute weighted entropy of the split
    double weightedEntropy = 0.0;
    int totalSize = this->columns.size();

    for (auto& [attributeValue, subsetLabels] : subsets) {
        double subsetEntropy = calculateEntropy(subsetLabels);
        weightedEntropy += (static_cast<double>(subsetLabels.size()) / totalSize) * subsetEntropy;
    }

    return initialEntropy - weightedEntropy;
}



// Function to find the best attribute (highest information gain)
int DataFrame::selectBestAttribute(string label_column) {

    if (this->data.empty()) {
        throw std::invalid_argument("No data in the DataFrame");
    }

    // Extract the target column
    size_t label_idx = -1;
    for (size_t i = 0; i < this->columns.size(); i++) {
        if (this->columns[i] == label_column) {
            label_idx = i;
        }
    }
    // Check if label column is in the valid
    if (label_idx == -1) {
        throw std::invalid_argument("Label column not in list of columns");
    }



    int bestAttribute = -1;
    double maxGain = -std::numeric_limits<double>::infinity();

    for (size_t attributeIndex = 0; attributeIndex < this->columns.size(); attributeIndex++) {
        if (attributeIndex == label_idx) {
            continue;
        }

        double gain = calculateInformationGain(attributeIndex, label_column);
        if (gain > maxGain) {
            maxGain = gain;
            bestAttribute = attributeIndex;
        }
    }

    return bestAttribute;
}


double DataFrame::column_median(string col_name) {
    vector<double> column = this->get_column(col_name);
    size_t size = column.size();
    if (size == 0) {
        throw std::invalid_argument("Column is empty");
    }

    std::sort(column.begin(), column.end());
    if (size % 2 == 0) {
        return (column[size / 2 - 1] + column[size / 2]) / 2;
    } else {
        return column[size / 2];
    }
}