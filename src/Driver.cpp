#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <map>
#include <random>
#include <climits>


#include "DataFrame.h"
#include "DecisionTree.h"
#include "RandomForest.h"

// Helper function to trim whitespace from the beginning and end of a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}


std::pair<int, int> parse_bounds(const std::string& bound_str) {
    int lower, upper;
    char dash;
    std::istringstream iss(bound_str);
    if (iss >> lower >> dash >> upper && dash == '-' && upper > lower) {
        return {lower, upper};
    } else {
        throw std::invalid_argument("Invalid bounds format");
    }
}


std::pair<std::vector<std::string>, std::vector<std::string>> read_clean_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::vector<std::string> drop_columns;
    std::vector<std::string> one_hot_encode_columns;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::string token;
        if (line.find("drop:") == 0) {
            line_stream.ignore(5);  // Skip "drop:"
            while (std::getline(line_stream, token, ',')) {
                drop_columns.push_back(trim(token));  // trim to remove any extra spaces
            }
        } else if (line.find("one_hot_encode:") == 0) {
            line_stream.ignore(15);  // Skip "one_hot_encode:"
            while (std::getline(line_stream, token, ',')) {
                one_hot_encode_columns.push_back(trim(token));  // trim to remove any extra spaces
            }
        }
    }

    file.close();
    return {drop_columns, one_hot_encode_columns};
}



double get_training_data_ratio() {
    double ratio;
    std::cout << "Enter the proportion of data to use as training data (decimal between 0 and 1): ";
    
    while (true) {
        std::cin >> ratio;
        
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
            std::cout << "Invalid input. Please enter a decimal between 0 and 1: ";
        } else if (ratio <= 0.0 || ratio >= 1.0) {
            std::cout << "Input must be greater than 0 and less than 1. Try again: ";
        } else {
            break;
        }
    }
    return ratio;
}

std::string get_label_column() {
    std::string label_column;
    std::cout << "Enter the name of the label column: ";
    std::getline(std::cin, label_column);

    return label_column;
}

int main(int argc, char* argv[]) {
    int opt;
    std::string input_file;
    std::string config_file = "config.txt";
    std:string cleaning_file = "clean.txt";
    bool verbose = false;
    
    
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, INT_MAX);

    int seed = dist(gen);

    // Define short options: h (no argument), f (requires argument), o (requires argument), v (no argument)
    while ((opt = getopt(argc, argv, "hf:c:vl:s:")) != -1) {
        switch (opt) {
            case 'h':
                std::cout << "Usage: ./program [-h] [-v] [-f filename] [-c config] [-l cleaning file] [-s seed]\n"
                          << "Options:\n"
                          << "  -h                Show help\n"
                          << "  -v                Enable verbose mode\n"
                          << "  -f filename       Specify input file\n"
                          << "  -c config         Specify config file\n"
                          << "  -l cleaning file  Specify cleaning file\n"
                          << "  -s seed           Specify a random seed\n";
                return 0;
            case 'f':
                input_file = optarg;
                break;
            case 'c':
                config_file = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            case 'l':
                // Cleaning file option
                cleaning_file = optarg;
                break;
            case 's':
                seed = std::stoi(optarg);
                break;
            case '?':
                std::cerr << "Unknown option: " << char(optopt) << "\n";
                return 1;
            default:
                std::cerr << "Error parsing options.\n";
                return 1;
        }
    }

    // Print the parsed options
    if (verbose) {
        std::cout << "\033[32mVerbose mode enabled.\033[0m\n";
    }
    if (!input_file.empty() && verbose) {
        std::cout << "Input file: " << input_file << "\n";
    }
    if (config_file != "config.txt" && verbose) {
        std::cout << "Config file: " << config_file << "\n";
    }
    if (cleaning_file != "clean.txt" && verbose) {
        std::cout << "Config file: " << cleaning_file << "\n";
    }

    // Handle remaining command-line arguments (non-option arguments)
    if (optind < argc) {
        std::cout << "Non-option arguments:\n";
        for (int i = optind; i < argc; ++i) {
            std::cout << "  " << argv[i] << "\n";
        }
    }


    std::ifstream file(config_file);
    if (!file) {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    std::map<std::string, std::pair<int, int>> hyperparameter_bounds;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string hyperparameter, bounds;
        if (std::getline(iss, hyperparameter, ':') && std::getline(iss, bounds)) {
            try {
                hyperparameter_bounds[hyperparameter] = parse_bounds(bounds);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error parsing bounds for " << hyperparameter << ": " << e.what() << "\n";
            }
        }
    }

    if (verbose) {
        std::cout << "\nParsed hyperparameter bounds:\n";
        for (const auto& [key, value] : hyperparameter_bounds) {
            std::cout << key << ": [" << value.first << ", " << value.second << "]\n";
        }
    }


    unique_ptr<DataFrame> df = DataFrame::read_csv(input_file);

    if (verbose) {
        std::cout << "\n\n";
        std::cout << "Read DataFrame from " << input_file << ":\n";
        std::cout << df->head(5)->print();
        std::cout << ".\n";
        std::cout << ".\n";
        std::cout << ".\n\n";
    }

    

    // Read and apply cleaning instructions from config file
    auto [drop_columns, one_hot_encode_columns] = read_clean_file(cleaning_file);
    if (verbose) {
        std::cout << "\n\n\033[32mData Cleaning:\033[0m\n" << "\tDrop Columns:\n";
    }
    // Clean Data
    for (const auto& col : drop_columns) {
        if (verbose) {
            std::cout << "\t\t" << col << "\n";
        }
        df->drop_column(col);
    }
    if (verbose) {
        std::cout << "\n\tOne Hot Encode Columns:\n";
    }
    for (const auto& col : one_hot_encode_columns) {
        if (verbose) {
            std::cout << "\t\t" << col << "\n";
        }
        df->one_hot_encode(col);
    }

    if (verbose) {
        std::cout << "\n\n";
    }

    double training_data_ratio = get_training_data_ratio();
    if (verbose) {
        std::cout << "\n";
    }
    auto [train_df, test_df] = df->split_train_test(training_data_ratio);

    // Clear any leftover input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string label_col = get_label_column();
    if (std::find(df->columns.begin(), df->columns.end(), label_col) == df->columns.end()) {
        std::cerr << "Label column not found.\n";
        return 1;
    }

    if (verbose) {
        std::cout << "\n\n";
    }

    vector<int> num_trees_values;
    vector<int> max_depth_values;
    vector<int> min_samples_split_values;
    vector<int> num_features_values;

    for (const auto& [key, value] : hyperparameter_bounds) {
        if (key == "num_trees") {
            for (int i = value.first; i <= value.second; ++i) {
                num_trees_values.push_back(i);
            }
        } else if (key == "max_depth") {
            for (int i = value.first; i <= value.second; ++i) {
                max_depth_values.push_back(i);
            }
        } else if (key == "min_samples_split") {
            for (int i = value.first; i <= value.second; ++i) {
                min_samples_split_values.push_back(i);
            }
        } else if (key == "num_features") {
            for (int i = value.first; i <= value.second; ++i) {
                num_features_values.push_back(i);
            }
        }
    }


    auto [best_num_trees, best_max_depth, best_min_samples_split, best_num_features] = RandomForest::hypertune(std::move(train_df),
                                                                                     label_col, 3, seed,
                                                                                     num_trees_values,
                                                                                     max_depth_values,
                                                                                     min_samples_split_values,
                                                                                     num_features_values,
                                                                                     verbose);

    if (verbose) {
        std::cout << "\n\n";
        std::cout << "Best hyperparameters:\n";
        std::cout << "  num_trees: " << best_num_trees << "\n";
        std::cout << "  max_depth: " << best_max_depth << "\n";
        std::cout << "  min_samples_split: " << best_min_samples_split << "\n";
        std::cout << "  num_features: " << best_num_features << "\n";
    }

    return 0;
}