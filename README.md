# Random Forest Implementation with Custom DataFrame and Decision Tree

*Author: Chris Dare*

## Project Overview

This project implements a **Random Forest** from scratch, including a custom **DataFrame** class for data handling, **Decision Tree** for individual tree creation, and **Node** for tree structure. The **Random Forest** utilizes bootstrap sampling and random feature selection to construct multiple decision trees in parallel. The goal is to demonstrate the core principles of Random Forests in a highly customizable and modular way.

---

## **Table of Contents**

1. [Project Structure](#project-structure)
2. [Features](#features)
3. [How It Works](#how-it-works)
4. [Usage](#usage)
5. [Example](#example)
6. [Building and Running the Program](#building-and-running-the-program)
7. [Future Improvements](#future-improvements)

---

## Project Structure

- **DataFrame.cpp / DataFrame.h**: Implements a custom DataFrame class for managing and manipulating tabular data.
- **Node.cpp / Node.h**: Represents individual nodes in the decision tree, storing split criteria and child nodes.
- **DecisionTree.cpp / DecisionTree.h**: Implements a decision tree for classification or regression.
- **RandomForest.cpp / RandomForest.h**: Implements a random forest that builds multiple decision trees in parallel with bootstrap sampling and random feature selection.
- **Driver.cpp**: Contains the main function to demonstrate and test the entire system.

---

## Features

- **Custom DataFrame**: A class for data manipulation with support for numeric and categorical data.
- **Decision Tree**: Supports binary splits, calculates information gain, and builds trees recursively.
- **Random Forest**:
  - Bootstrap sampling of rows and random selection of features.
  - Parallel tree construction using `std::async`.
  - Feature mapping to ensure each tree predicts based only on the features it was trained on.
- **Multi-threaded Execution**: Trees are trained asynchronously for improved performance.
- **Error Handling**: Ensures robustness with comprehensive checks for invalid inputs.

---

## How It Works

1. **DataFrame**:
   - Creates a Series class (like pandas) which abstracts a single row or column, and supports column operations (e.g. median, mean, etc.)
   - Loads and manipulates tabular data by constructing a DataFrame as an unordered map from Column names (strings) to Series.
   - Provides methods for filtering, row retrieval, and bootstrap sampling.
2. **DecisionTree**:
   - Trains on a specified DataFrame using the ID3 algorthim.
   - Uses a greedy approach to on nodes to split data recursively based on the best feature.
3. **RandomForest**:
   - Trains multiple decision trees in parallel.
   - Each tree is trained on a different bootstrap sample with a random subset of features.
   - During prediction, it ensures each tree uses only the features it was trained on.
   - Aggregates predictions from individual trees using majority voting (for classification).
4. **driver.cpp**:
   - Provides a simple interface for users to test the model.

---

## Usage

### Sample Code

```cpp
#include "DataFrame.h"
#include "RandomForest.h"

int main() {
    // Load data into a DataFrame
    auto data = DataFrame::read_csv("data.csv");

    // Create and train the Random Forest
    RandomForest rf(10, 5, 2, 3);  // 10 trees, max depth 5, min samples split 2, 3 features per tree
    rf.fit(std::move(data), "label");

    // Sample input for prediction
    std::vector<double> sample = {25.0, 50000.0, 5.0};
    double prediction = rf.predict(sample);

    std::cout << "Prediction: " << prediction << std::endl;
    
    return 0;
}
```

---

## Building and Running the Program

### Prerequisites

- C++17 or higher
- C++ standard libraries (`<vector>`, `<string>`, `<map>`, `<future>`, etc.)

### Build Instructions

1. Clone the repository.
2. Call the Makefil using `make`:
3. Adjust the configurations files `config.txt` dictate the parameters of the grid search in tuning hyperparameters. The file `config.txt` is of the format
```
num_trees: 5-7
max_depth: 4-7
min_samples_split: 1-4
num_features: 1-4
```
where the first number is the lower-bound and the second number is the upper-bound for that particular variable in the grid search. 

**Note:** The performance of the program varies drastically depending on these variables.

4. Adjust the configuration file `clean.txt` to indicate which categorical features should be converted to numeric catagories and which cat egories should be dropped entirely. The file `clean.txt` is of the format:
```
drop: col1,col2,col3
one_hot_encode: col4,col5,col6
```
**Note:** The string names of the columns must match the column names of the .csv exactly --- the program will not run if these are not set correctly.

Since the "clean files" are effectively tied to a single CSV file (they depend uniquely on the column names and types), it is generally suggested these are stored **with** the CSV files and follow similar naming conventions.

5. Run the program:
   ```bash
   ./Driver -f file_name [-c config_file] [-l cleaning_file] [-v verbose] [-s seed]
   ```
The only necessary argument is the `file_name`, which specifies the `.csv` file that will be converted into a DataFrame. Other filenames can be used for the parameter configuration file and data cleaning file by using the `-c` and `-l` flags, respectively. 

The `-v` flag sets the output to be *verbose* in the sense that significantly more information is printed to the console during the training process. 

The `-s` flag allows one to enter a random state which seeds the random processes that occur while fitting the random forest (specifically, the bootstrap sampling and random selection of features). This allows for reproducable results.

---

## Example

Suppose we have a sample dataset (`hire_data.csv`) consisting of hiring data for final-round candidates of a company over the month of December before the holidays. Each entry consists of the date the candidate was interviewed, the age of the candidate, the salary that they are asking, the years of (relevant) experience the candidate has, the candidates' highest degree level, and whether the candiate was hired (Y/N). 

```csv
date,age,salary_expect,experience,degree_lvl,hired
12-01-2024,25,50000,2,B,N
12-03-2024,30,60000,7,B,Y
12-03-2024,28,80000,1,PHD,Y
12-04-2024,40,100000,15,B,N
12-05-2024,35,80000,10,B,Y
12-05-2024,32,90000,5,PHD,Y
12-05-2024,29,80000,4,B,N
12-06-2024,29,90000,3,M,N
12-06-2024,31,85000,5,B,N
12-06-2024,27,60000,4,B,Y
12-09-2024,25,65000,2,B,N
12-09-2024,31,90000,5,M,N
12-10-2024,32,85000,9,B,Y
12-11-2024,51,120000,16,B,Y
12-11-2024,60,134000,24,B,N
12-12-2024,32,92000,10,B,Y
12-13-2024,35,100000,10,M,Y
12-13-2024,41,120000,13,B,N
12-13-2024,44,124000,15,PHD,Y
12-16-2024,29,80000,4,B,N
12-16-2024,31,90000,5,B,N
12-16-2024,32,95000,6,PHD,Y
12-17-2024,33,100000,7,M,Y
12-17-2024,35,110000,8,B,N
12-18-2024,37,120000,9,PHD,Y
12-18-2024,29,80000,4,B,N
12-18-2024,31,90000,5,B,N
12-19-2024,32,95000,6,PHD,Y
12-20-2024,33,100000,7,M,Y
12-20-2024,35,110000,8,B,N
12-20-2024,37,120000,9,PHD,Y
12-20-2024,29,80000,4,B,N
```

Since the date a candidate was interviewed doesnt make much difference, one can safely drop the `date` column; due to the age discrimination employment act, it's probably also a good idea to drop `age`. Additionally, one would need to convert all non-numeric catagorical columns. Thus, the "clean" file (`clean_hire_data.txt`) should resemble the following:

```
drop: date,age
one_hot_encode: degree_lvl,hired
```
Assuming we're okay with the default `config.txt` settings, we would run
```
make
```
followed by
```
./Driver -f hire_data.csv -l clean_hire_data.txt -v -s 123456
```
Once the program begins running, the user will be prompted for the proportion of data to use as training data. For a 80-20 split, one would enter `0.8`. In this example, we are trying to classify whether a candiate should be hired or not; thus, one would type `hired` in the next prompt asking for the which variable should be the *label*. 

### Expected Output:

Assuming an 80-20 split was used, the seed `123456` yields an optimal model with parameters

```
Best hyperparameters:
  num_trees: 7
  max_depth: 4
  min_samples_split: 1
  num_features: 3
```

Since the data set is trivially small, the model scores 100% in terms of testing accuracy. A more veteran user can quickly tinker with the datasets to learn that the model is not so precise in practice.

---

## Future Improvements

- Add support for continuous features with better splitting criteria (e.g. regression).
- Add visualization for decision trees.
- Further develop the gradient-boosting classifier.

---

## License

MIT License

Copyright (c) 2025 Christopher Dare

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


---

