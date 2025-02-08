#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <variant>

#include "DataFrame.h"

using std::vector;
using std::string;
using std::unique_ptr;

using Cell = std::variant<int,double,std::string>;


/* -------------------------------------------------- */
/* -------------------------------------------------- */
/* ------------- SEREIS IMPLEMENTATION ---------------*/
/* -------------------------------------------------- */
/* -------------------------------------------------- */

Series::Series() : is_numeric(true) {}


Series::Series(vector<Cell> data) : data(data), is_numeric(true) {
    for (const auto& value : data) {
        this->is_numeric &= std::visit([](auto&& value) -> bool {
            using T = std::decay_t<decltype(value)>;
            return std::is_same_v<T, int> || std::is_same_v<T, double>;
        }, value);
    }
}

Series::~Series() {}

void Series::push_back(Cell value) {
    
    // Continually update the is_numeric flag
    this->is_numeric &= std::visit([](auto&& value) -> bool {
        using T = std::decay_t<decltype(value)>;
        return std::is_same_v<T, int> || std::is_same_v<T, double>;
    }, value);

    data.push_back(value);
}

Cell Series::retrieve(size_t row) const {
    if (row >= data.size()) {
        throw std::runtime_error("row index out of bounds");
    }
    return data[row];
}

size_t Series::size() const {
    return data.size();
}

bool Series::empty() const {
    return data.empty();
}


string Series::print() const {
    std::ostringstream oss;
    for (const auto& val : data) {
        std::visit([&](auto&& value) {
            oss << value << " ";
        }, val);
    }
    return oss.str();
}

Cell Series::mode() const {
    if (data.empty()) {
        throw std::runtime_error("Cannot compute mode on an empty column!");
    }

    // Frequency map to count occurrences of each value
    std::map<Cell, int> frequency_map;

    for (const auto& cell : data) {
        frequency_map[cell]++;
    }

    // Find the maximum frequency
    int max_frequency = 0;
    Cell mode_value = data[0];  // Default to first value

    for (const auto& cell : data) {
        int& count = frequency_map[cell];  // Get reference to count (increments automatically)
        count++;

        if (count > max_frequency) {
            max_frequency = count;
            mode_value = cell;
        }
    }

    return mode_value;
}


double Series::median() const {

    std::vector<double> numeric_values = this->convert_to_numeric();

    if (numeric_values.empty()) {
        throw std::runtime_error("Cannot compute median on an empty column!");
    }

    // Sort the numeric values
    std::sort(numeric_values.begin(), numeric_values.end());

    size_t n = numeric_values.size();
    if (n % 2 == 0) {
        // If even, return the average of middle elements
        return (numeric_values[n / 2 - 1] + numeric_values[n / 2]) / 2.0;
    } else {
        // If odd, return the middle element
        return numeric_values[n / 2];
    }
}


Series Series::convertToNumericClasses() const {
    std::unordered_map<std::string, int> class_map;
    Series numeric_classes;
    int class_counter = 0;

    for (const auto& cell : data) {
        std::visit([&](auto&& value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::string>) {
                if (class_map.find(value) == class_map.end()) {
                    class_map[value] = class_counter++;
                }
                numeric_classes.push_back(class_map[value]);
            } else {
                throw std::runtime_error("convertToNumericClasses: Series contains non-string data.");
            }
        }, cell);
    }

    return numeric_classes;
}

vector<double> Series::convert_to_numeric() const {
    std::vector<double> numeric_values;

    for (const auto& cell : data) {
        std::visit([&](auto&& value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                numeric_values.push_back(static_cast<double>(value));
            } else {
                throw std::runtime_error("The column does not entirely consist of numeric data");
            }
        }, cell);
    }

    return numeric_values;
}

vector<string> Series::convert_to_string() const {
    std::vector<string> string_values;

    for (const auto& cell : data) {
        std::visit([&](auto&& value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::string>) {
                string_values.push_back(static_cast<std::string>(value));
            } else if (std::is_same_v<T, int>) {
                string_values.push_back(std::to_string(static_cast<int>(value)));
            } else if (std::is_same_v<T, double>) {
                string_values.push_back(std::to_string(static_cast<double>(value)));
            } else {
                throw std::runtime_error("The column does not entirely consist of string data");
            }
        }, cell);
    }

    return string_values;
}



Series Series::operator+(const Series& other) const {
    Series result;
    if (this->data.size() != other.data.size()) {
        throw std::runtime_error("Series sizes do not match");
    }

    for (size_t i = 0; i < this->data.size(); ++i) {
        result.push_back(std::visit([](auto&& value1, auto&& value2) -> Cell {
            using T1 = std::decay_t<decltype(value1)>;
            using T2 = std::decay_t<decltype(value2)>;
            if constexpr (std::is_same_v<T1, int> && std::is_same_v<T2, int>) {
                return static_cast<int>(value1) + static_cast<int>(value2);
            } else if constexpr (std::is_same_v<T1, double> && std::is_same_v<T2, double>) {
                return static_cast<double>(value1) + static_cast<double>(value2);
            } else if constexpr (std::is_same_v<T1, int> && std::is_same_v<T2, double>) {
                return static_cast<double>(value1) + static_cast<double>(value2);
            } else if constexpr (std::is_same_v<T1, double> && std::is_same_v<T2, int>) {
                return static_cast<double>(value1) + static_cast<double>(value2);
            } else if constexpr (std::is_same_v<T1, std::string> && std::is_same_v<T2, std::string>) {
                return static_cast<std::string>(value1) + static_cast<std::string>(value2);
            } else if constexpr (std::is_same_v<T1, std::string> && (std::is_same_v<T2, int> || std::is_same_v<T2, double>)) {
                return static_cast<std::string>(value1) + std::to_string(value2);
            } else if constexpr ((std::is_same_v<T1, int> || std::is_same_v<T1, double>) && std::is_same_v<T2, std::string>) {
                return std::to_string(value1) + static_cast<std::string>(value2);
            } else {
                throw std::runtime_error("Unsupported data types");
            }
        }, this->data[i], other.data[i]));
    }

    return result;
}

Cell& Series::operator[](size_t index) {
    if (index >= data.size()) {
        throw std::out_of_range("index out of bounds");
    }
    return data[index];
}

bool Series::operator==(const Series& other) const {
    if (this->data.size() != other.data.size()) {
        return false;
    }

    for (size_t i = 0; i < this->data.size(); ++i) {
        bool is_equal = std::visit([](auto&& value1, auto&& value2) -> bool {
            using T1 = std::decay_t<decltype(value1)>;
            using T2 = std::decay_t<decltype(value2)>;
            if constexpr (std::is_same_v<T1, int> && std::is_same_v<T2, int>) {
                return static_cast<int>(value1) == static_cast<int>(value2);
            } else if constexpr (std::is_same_v<T1, double> && std::is_same_v<T2, double>) {
                return static_cast<double>(value1) == static_cast<double>(value2);
            } else if constexpr (std::is_same_v<T1, std::string> && std::is_same_v<T2, std::string>) {
                return static_cast<std::string>(value1) == static_cast<std::string>(value2);
            } else {
                return false;
            }
        }, this->data[i], other.data[i]);

        if (!is_equal) {
            return false;
        }
    }

    return true;
}


std::vector<Cell>::iterator Series::begin() {
    return data.begin();
}

std::vector<Cell>::iterator Series::end() {
    return data.end();
}


double Series::calculateEntropy() const {
    
    std::map<Cell, int> frequency_map;
    // Count occurrences of each unique value
    for (const auto& value : data) {
        frequency_map[value]++;
    }

    double entropy = 0.0;

    // Compute entropy using the formula
    for (const auto& [key, count] : frequency_map) {
        double probability = static_cast<double>(count) / data.size();
        entropy -= probability * std::log2(probability);
    }

    return entropy;
}









/* -------------------------------------------------- */
/* -------------------------------------------------- */
/* ---------- DATAFRAME IMPLEMENTATION ---------------*/
/* -------------------------------------------------- */
/* -------------------------------------------------- */



// Constructor
DataFrame::DataFrame() : data(std::unordered_map<string, Series>()), columns(vector<string>()) {}
// Destructor
DataFrame::~DataFrame() {}



// Function to calculate information gain of an attribute
double DataFrame::calculateInformationGain(string attribute_name, string label_name) const {
    // Check if the attribute and label columns exist
    if (data.find(attribute_name) == data.end()) {
        throw std::runtime_error("attribute column not found");
    }
    if (data.find(label_name) == data.end()) {
        throw std::runtime_error("label column not found");
    }

    // Get the label data
    Series label_data = data.at(label_name);

    // Compute the entropy of the entire dataset w.r.t. label column
    double total_entropy = label_data.calculateEntropy();

    // map to store subsets of data based on attribute values
    std::map<Cell, vector<size_t>> partitions;
    size_t total_count = label_data.size();

    // Partition data based on attribute values
    for (size_t row = 0; row < total_count; ++row) {
        Series attribute_data = data.at(attribute_name);
        partitions[attribute_data[row]].push_back(row);
    }

    // Compute weighted entropy after the split
    double weighted_entropy = 0.0;

    for (const auto& [attribute_value, row_indices] : partitions) {
        // Create a temporary DataFrame for subset
        DataFrame subset;
        for (const auto& col : columns) {
            subset.addColumn(col);
        }

        for (size_t row : row_indices) {
            std::vector<Cell> new_row;
            for (const auto& col : columns) {
                Series col_data = data.at(col);
                new_row.push_back(col_data[row]);
            }
            subset.addRow(new_row);
        }

        // Compute entropy of this subset
        double subset_entropy = subset.data.at(label_name).calculateEntropy();
        double subset_weight = static_cast<double>(row_indices.size()) / total_count;

        weighted_entropy += subset_weight * subset_entropy;
    }

    // Information Gain = Total Entropy - Weighted Entropy after split
    return total_entropy - weighted_entropy;
}


/*----------------FILTER METHODS ---------------------*/

unique_ptr<DataFrame> DataFrame::filter_neq(string column_name, Cell value) const {
    if (data.find(column_name) == data.end()) {
        throw std::runtime_error("Column not found");
    }

    // Create new DataFrame for filtered results
    unique_ptr<DataFrame> filtered_df = std::make_unique<DataFrame>();

    // Copy column names to the new DataFrame
    for (const auto& col : columns) {
        filtered_df->addColumn(col);
    }

    size_t num_rows = this->data.begin()->second.size();

    // Iterate through rows and add only those where column[attribute_index] != value
    for (size_t row = 0; row < num_rows; ++row) {
        bool should_include = std::visit(
            [](auto&& cell_val, auto&& filter_val) -> bool {
                using T1 = std::decay_t<decltype(cell_val)>;
                using T2 = std::decay_t<decltype(filter_val)>;
                
                // Ensure both values are of the same type before comparing
                if constexpr (std::is_same_v<T1, T2>) {
                    return cell_val != filter_val;
                } else {
                    return false;  // If types are different, do not filter out the row
                }
            },
            this->data.at(column_name).retrieve(row), value);

        if (should_include) {
            std::vector<Cell> new_row;
            for (const auto& col : columns) {
                Series col_data = data.at(col);
                new_row.push_back(data.at(col).retrieve(row));
            }
            filtered_df->addRow(static_cast<const std::vector<Cell>&>(new_row));
        }
    }

    return filtered_df;
}


unique_ptr<DataFrame> DataFrame::filter_eq(string column_name, Cell value) const {
    if (data.find(column_name) == data.end()) {
        throw std::runtime_error("Column not found");
    }

    // Create new DataFrame for filtered results
    unique_ptr<DataFrame> filtered_df = std::make_unique<DataFrame>();

    // Copy column names to the new DataFrame
    for (const auto& col : columns) {
        filtered_df->addColumn(col);
    }

    size_t num_rows = this->data.begin()->second.size();

    // Iterate through rows and add only those where column[attribute_index] != value
    for (size_t row = 0; row < num_rows; ++row) {
        bool should_include = std::visit(
            [](auto&& cell_val, auto&& filter_val) -> bool {
                using T1 = std::decay_t<decltype(cell_val)>;
                using T2 = std::decay_t<decltype(filter_val)>;
                
                // Ensure both values are of the same type before comparing
                if constexpr (std::is_same_v<T1, T2>) {
                    return cell_val == filter_val;
                } else {
                    return false;  // If types are different, do not filter out the row
                }
            },
            this->data.at(column_name).retrieve(row), value);

        if (should_include) {
            std::vector<Cell> new_row;
            for (const auto& col : columns) {
                Series col_data = data.at(col);
                new_row.push_back(data.at(col).retrieve(row));
            }
            filtered_df->addRow(static_cast<const std::vector<Cell>&>(new_row));
        }
    }

    return filtered_df;
}



unique_ptr<DataFrame> DataFrame::numeric_filter_geq(string column_name, double threshold) const {
    // Ensure column exists
    if (data.find(column_name) == data.end()) {
        throw std::runtime_error("Column not found: " + column_name);
    }

    // Ensure the column is numeric
    Series column_data = data.at(column_name);
    if (!column_data.is_numeric) {
        throw std::runtime_error("Column is not numeric: " + column_name);
    }

    // Create a new filtered DataFrame
    unique_ptr<DataFrame> filtered_df = std::make_unique<DataFrame>();

    // Copy column names to the new DataFrame
    for (const auto& col : columns) {
        filtered_df->addColumn(col);
    }

    // Iterate through rows and filter values
    for (size_t row = 0; row < column_data.size(); ++row) {
        bool should_include = std::visit(
            [&](auto&& cell_val) -> bool {
                using T = std::decay_t<decltype(cell_val)>;
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                    return static_cast<double>(cell_val) >= threshold;
                }
                return false;
            },
            column_data[row]);

        if (should_include) {
            std::vector<Cell> new_row;
            for (const auto& col : columns) {
                new_row.push_back(data.at(col).retrieve(row));
            }
            filtered_df->addRow(new_row);
        }
    }

    return filtered_df;
}


unique_ptr<DataFrame> DataFrame::numeric_filter_gt(string column_name, double threshold) const {
    // Ensure column exists
    if (data.find(column_name) == data.end()) {
        throw std::runtime_error("Column not found: " + column_name);
    }

    // Ensure the column is numeric
    Series column_data = data.at(column_name);
    if (!column_data.is_numeric) {
        throw std::runtime_error("Column is not numeric: " + column_name);
    }

    // Create a new filtered DataFrame
    unique_ptr<DataFrame> filtered_df = std::make_unique<DataFrame>();

    // Copy column names to the new DataFrame
    for (const auto& col : columns) {
        filtered_df->addColumn(col);
    }

    // Iterate through rows and filter values
    for (size_t row = 0; row < column_data.size(); ++row) {
        bool should_include = std::visit(
            [&](auto&& cell_val) -> bool {
                using T = std::decay_t<decltype(cell_val)>;
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                    return static_cast<double>(cell_val) > threshold;
                }
                return false;
            },
            column_data[row]);

        if (should_include) {
            std::vector<Cell> new_row;
            for (const auto& col : columns) {
                new_row.push_back(data.at(col).retrieve(row));
            }
            filtered_df->addRow(new_row);
        }
    }

    return filtered_df;
}



unique_ptr<DataFrame> DataFrame::numeric_filter_leq(string column_name, double threshold) const {
    // Ensure column exists
    if (data.find(column_name) == data.end()) {
        throw std::runtime_error("Column not found: " + column_name);
    }

    // Ensure the column is numeric
    Series column_data = data.at(column_name);
    if (!column_data.is_numeric) {
        throw std::runtime_error("Column is not numeric: " + column_name);
    }

    // Create a new filtered DataFrame
    unique_ptr<DataFrame> filtered_df = std::make_unique<DataFrame>();

    // Copy column names to the new DataFrame
    for (const auto& col : columns) {
        filtered_df->addColumn(col);
    }

    // Iterate through rows and filter values
    for (size_t row = 0; row < column_data.size(); ++row) {
        bool should_include = std::visit(
            [&](auto&& cell_val) -> bool {
                using T = std::decay_t<decltype(cell_val)>;
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                    return static_cast<double>(cell_val) <= threshold;
                }
                return false;
            },
            column_data[row]);

        if (should_include) {
            std::vector<Cell> new_row;
            for (const auto& col : columns) {
                new_row.push_back(data.at(col).retrieve(row));
            }
            filtered_df->addRow(new_row);
        }
    }

    return filtered_df;
}


unique_ptr<DataFrame> DataFrame::numeric_filter_lt(string column_name, double threshold) const {
    // Ensure column exists
    if (data.find(column_name) == data.end()) {
        throw std::runtime_error("Column not found: " + column_name);
    }

    // Ensure the column is numeric
    Series column_data = data.at(column_name);
    if (!column_data.is_numeric) {
        throw std::runtime_error("Column is not numeric: " + column_name);
    }

    // Create a new filtered DataFrame
    unique_ptr<DataFrame> filtered_df = std::make_unique<DataFrame>();

    // Copy column names to the new DataFrame
    for (const auto& col : columns) {
        filtered_df->addColumn(col);
    }

    // Iterate through rows and filter values
    for (size_t row = 0; row < column_data.size(); ++row) {
        bool should_include = std::visit(
            [&](auto&& cell_val) -> bool {
                using T = std::decay_t<decltype(cell_val)>;
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                    return static_cast<double>(cell_val) < threshold;
                }
                return false;
            },
            column_data[row]);

        if (should_include) {
            std::vector<Cell> new_row;
            for (const auto& col : columns) {
                new_row.push_back(data.at(col).retrieve(row));
            }
            filtered_df->addRow(new_row);
        }
    }

    return filtered_df;
}



void DataFrame::addRow(const std::vector<Cell>& values) {
    if (values.size() != this->columns.size()) {
        throw std::runtime_error("Row size does not match column count.");
    }
    for (size_t i = 0; i < values.size(); ++i) {
        this->data[this->columns[i]].push_back(values[i]);
    }
}



void DataFrame::addColumn(const std::string& name) {
    if (data.find(name) == data.end()) {
        data[name] = {};
        columns.push_back(name);
    } else {
        throw std::runtime_error("Column already exists");
    }
}



Series DataFrame::get_column(string col_name) {
    if (data.find(col_name) == data.end()) {
        throw std::invalid_argument("Column not found");
    }
    return data[col_name];
}



int DataFrame::get_num_rows() {
    if (data.empty()) return 0;
    return this->data.begin()->second.size();
}

// Function which returns the value of a cell when called using row and col 
// the col can be either a string or an integer 
Cell DataFrame::retrieve(size_t row, Cell col) {
    if (row >= this->get_num_rows()) {
        throw std::runtime_error("row index out of bounds");
    }


    int col_int_cast = std::visit([](auto&& value) -> double {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int>) {
                return static_cast<int>(value);
            } else {
                // if not an integer, return a negative value that will fail the bounds check
                return -1;
            }
        }, col);
    if (col_int_cast >= 0 && col_int_cast < this->columns.size()) {
        string column_name = this->columns[col_int_cast];
        return (data[column_name])[row];
    }

    std::string col_str_cast = std::visit([](auto&& value) -> std::string {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, std::string>) {
            return static_cast<std::string>(value);
        } else {
            return "";
        }
    }, col);

    if (!col_str_cast.empty()) {
        return (data[col_str_cast])[row];
    } else {
        throw std::runtime_error("Invalid column identifier");
    }
}






string DataFrame::cellToString(const Cell& cell) const {
    return std::visit([](auto&& value) -> std::string {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, std::string>) {
            return value;  // Keep strings as is
        } else {
            return std::to_string(value);  // Convert int & double to string
        }
    }, cell);
}


string DataFrame::print() const {
    if (data.empty()) {
        return "Empty DataFrame!\n";
    }

    std::ostringstream oss;

    // Determine column widths
    std::unordered_map<std::string, size_t> column_widths;
    for (const auto& col : columns) {
        column_widths[col] = col.size();
    }

    // Adjust column width based on data
    for (const auto& col : columns) {
        Series column_data = data.at(col);
        for (const auto& cell : column_data) {
            size_t cell_width = cellToString(cell).size();
            column_widths[col] = std::max(column_widths[col], cell_width);
        }
    }

    // Compute total table width
    size_t total_width = 1;
    for (const auto& col : columns) {
        total_width += column_widths[col] + 3;
    }

    // Print top border
    oss << std::string(total_width, '-') << "\n";

    // Print column headers
    oss << "|";
    for (const auto& col : columns) {
        oss << " " << std::setw(column_widths[col]) << std::left << col << " |";
    }
    oss << "\n";

    // Print separator line
    oss << std::string(total_width, '-') << "\n";

    // Determine row count
    size_t num_rows = data.begin()->second.size();

    // Print each row
    for (size_t row = 0; row < num_rows; ++row) {
        oss << "|";
        for (const auto& col : columns) {
            oss << " " << std::setw(column_widths[col]) << std::left << cellToString(data.at(col).retrieve(row)) << " |";
        }
        oss << "\n";
    }

    // Print bottom border
    oss << std::string(total_width, '-') << "\n";

    return oss.str();
}





// Function to find the best attribute (highest information gain)
string DataFrame::selectBestAttribute(string label_name) {

    if (data.find(label_name) == data.end()) {
        throw std::invalid_argument("Label column not found");
    }

    string bestAttribute = "";
    double maxGain = -std::numeric_limits<double>::infinity();

    for (const std::string& attribute_name : columns) {
        if (attribute_name == label_name) {
            continue;
        }

        double gain = calculateInformationGain(attribute_name, label_name);
        if (gain > maxGain) {
            maxGain = gain;
            bestAttribute = attribute_name;
        }
    }

    return bestAttribute;
}


// Filter method
unique_ptr<DataFrame> DataFrame::filter(string column_name, Cell threshold, string condition) {
    if (condition == "<") {
        // typecast threshold as double
        double threshold_value = std::visit([](auto&& value) -> double {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                return static_cast<double>(value);
            } else {
                throw std::invalid_argument("Threshold must be numeric for '<' condition");
            }
        }, threshold);
        return numeric_filter_lt(column_name, threshold_value);
    } else if (condition == "<=") {
        // typecast threshold as double
        double threshold_value = std::visit([](auto&& value) -> double {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                return static_cast<double>(value);
            } else {
                throw std::invalid_argument("Threshold must be numeric for '<=' condition");
            }
        }, threshold);
        return numeric_filter_leq(column_name, threshold_value);
    } else if (condition == ">") {
        // typecast threshold as double
        double threshold_value = std::visit([](auto&& value) -> double {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                return static_cast<double>(value);
            } else {
                throw std::invalid_argument("Threshold must be numeric for '>' condition");
            }
        }, threshold);
        return numeric_filter_gt(column_name, threshold_value);
    } else if (condition == ">=") {
        // typecast threshold as double
        double threshold_value = std::visit([](auto&& value) -> double {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                return static_cast<double>(value);
            } else {
                throw std::invalid_argument("Threshold must be numeric for '>=' condition");
            }
        }, threshold);
        return numeric_filter_geq(column_name, threshold_value);

    } else if (condition == "==") {
        return filter_eq(column_name, threshold);
    } else if (condition == "!=") {
        return filter_neq(column_name, threshold);
    } else {
        throw std::invalid_argument("Invalid condition");
    }
}


int DataFrame::int_cast(Cell cell) {
    return std::visit([](auto&& value) -> double {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int>) {
                return static_cast<int>(value);
            } else {
                throw std::invalid_argument("Cell is non-numeric");
            }
        }, cell);
}

string DataFrame::str_cast(Cell cell) {
    return std::visit([](auto&& value) -> string {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, string>) {
                return static_cast<string>(value);
            } else {
                throw std::invalid_argument("Cell is not a string");
            }
        }, cell);
}