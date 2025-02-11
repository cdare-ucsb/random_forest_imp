#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <map>


#include "DataFrame.h"
#include "DecisionTree.h"
#include "RandomForest.h"


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
    std::cout << "\n\n";
    return ratio;
}



int main(int argc, char* argv[]) {
    int opt;
    std::string input_file;
    std::string config_file = "config.txt";
    bool verbose = false;

    // Define short options: h (no argument), f (requires argument), o (requires argument), v (no argument)
    while ((opt = getopt(argc, argv, "hf:c:v")) != -1) {
        switch (opt) {
            case 'h':
                std::cout << "Usage: ./program [-h] [-v] [-f filename] [-c config]\n"
                          << "Options:\n"
                          << "  -h          Show help\n"
                          << "  -v          Enable verbose mode\n"
                          << "  -f filename Specify input file\n"
                          << "  -c config   Specify config file\n";
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

    // TODO: Implement DataCleaning

    // Clean Data
    df->drop_column("date");
    df->one_hot_encode("weather");


    double training_data_ratio = get_training_data_ratio();
    auto [train_df, test_df] = df->split_train_test(training_data_ratio);




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

    // TODO: Add random seed, num_folds, and label_column as arguments

    auto [best_num_trees, best_max_depth, best_min_samples_split, best_num_features] = RandomForest::hypertune(std::move(train_df),
                                                                                     "weather", 3, 123456,
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