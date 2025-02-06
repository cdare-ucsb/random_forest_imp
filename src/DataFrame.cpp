#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>
#include <iomanip>
#include <sstream>

#include "DataFrame.h"

using std::vector;
using std::string;
using std::unique_ptr;


// Helper functions
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

unique_ptr<DataFrame> DataFrame::filter_lt(int attributeIndex, double threshold) {
    if (attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (const auto& row : this->data) {
        if (row[attributeIndex] < threshold) {
            filtered_data.push_back(row);
        }
    }

    return std::make_unique<DataFrame>(filtered_data, this->columns);
}

unique_ptr<DataFrame> DataFrame::filter_leq(int attributeIndex, double threshold) {
    if (attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (const auto& row : this->data) {
        if (row[attributeIndex] <= threshold) {
            filtered_data.push_back(row);
        }
    }

    return std::make_unique<DataFrame>(filtered_data, this->columns);
}

unique_ptr<DataFrame> DataFrame::filter_gt(int attributeIndex, double threshold) {
    if (attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (const auto& row : this->data) {
        if (row[attributeIndex] > threshold) {
            filtered_data.push_back(row);
        }
    }

    return std::make_unique<DataFrame>(filtered_data, this->columns);
}

unique_ptr<DataFrame> DataFrame::filter_geq(int attributeIndex, double threshold) {
    if (attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (const auto& row : this->data) {
        if (row[attributeIndex] >= threshold) {
            filtered_data.push_back(row);
        }
    }

    return std::make_unique<DataFrame>(filtered_data, this->columns);
}

unique_ptr<DataFrame> DataFrame::filter_eq(int attributeIndex, double threshold) {
    if (attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (const auto& row : this->data) {
        if (row[attributeIndex] == threshold) {
            filtered_data.push_back(row);
        }
    }

    return std::make_unique<DataFrame>(filtered_data, this->columns);
}

unique_ptr<DataFrame> DataFrame::filter_neq(int attributeIndex, double threshold) {
    if (attributeIndex >= this->columns.size() || attributeIndex < 0) {
        throw std::invalid_argument("Attribute index out of range");
    }

    vector<vector<double>> filtered_data;
    for (const auto& row : this->data) {
        if (row[attributeIndex] != threshold) {
            filtered_data.push_back(row);
        }
    }

    return std::make_unique<DataFrame>(filtered_data, this->columns);
}


// Constructor
DataFrame::DataFrame(std::vector<vector<double>> data, vector<string> columns) : columns(columns), data(data) {}
// Destructor
DataFrame::~DataFrame() {}

// Getters
vector<vector<double>> DataFrame::get_data() {
    return this->data;
}

int DataFrame::get_num_rows() {
    return this->data.size();
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



string DataFrame::print() {
    if (this->data.empty()) {
        return "Empty DataFrame";
    }

    std::ostringstream oss;
    size_t num_columns = this->columns.size();

    // Determine column widths
    std::vector<size_t> column_widths(num_columns, 10); // Default width

    // Find max width for each column
    for (size_t i = 0; i < num_columns; i++) {
        column_widths[i] = std::max(column_widths[i], this->columns[i].size());
    }
    
    for (const auto& row : this->data) {
        for (size_t i = 0; i < row.size(); i++) {
            std::string value = std::to_string(row[i]);
            column_widths[i] = std::max(column_widths[i], value.size());
        }
    }

    // Print column headers
    oss << "+";
    for (size_t i = 0; i < num_columns; i++) {
        oss << std::string(column_widths[i] + 2, '-') << "+";
    }
    oss << "\n|";

    for (size_t i = 0; i < num_columns; i++) {
        oss << " " << std::setw(column_widths[i]) << std::left << this->columns[i] << " |";
    }
    oss << "\n+";

    for (size_t i = 0; i < num_columns; i++) {
        oss << std::string(column_widths[i] + 2, '-') << "+";
    }
    oss << "\n";

    // Print rows
    for (const auto& row : this->data) {
        oss << "|";
        for (size_t i = 0; i < row.size(); i++) {
            oss << " " << std::setw(column_widths[i]) << std::left << row[i] << " |";
        }
        oss << "\n";
    }

    // Bottom border
    oss << "+";
    for (size_t i = 0; i < num_columns; i++) {
        oss << std::string(column_widths[i] + 2, '-') << "+";
    }
    oss << "\n";

    return oss.str();
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


double DataFrame::column_mode(string col_name) {
    vector<double> column = this->get_column(col_name);
    std::map<double, int> frequency;
    for (double val : column) {
        frequency[val]++;
    }

    double mode = column[0];
    int max_count = 0;
    for (const auto& [val, count] : frequency) {
        if (count > max_count) {
            max_count = count;
            mode = val;
        }
    }

    return mode;
}

// Filter method
unique_ptr<DataFrame> DataFrame::filter(int attributeIndex, double threshold, string condition) {
    if (condition == "<") {
        return filter_lt(attributeIndex, threshold);
    } else if (condition == "<=") {
        return filter_leq(attributeIndex, threshold);
    } else if (condition == ">") {
        return filter_gt(attributeIndex, threshold);
    } else if (condition == ">=") {
        return filter_geq(attributeIndex, threshold);
    } else if (condition == "==") {
        return filter_eq(attributeIndex, threshold);
    } else if (condition == "!=") {
        return filter_neq(attributeIndex, threshold);
    } else {
        throw std::invalid_argument("Invalid condition");
    }
}





