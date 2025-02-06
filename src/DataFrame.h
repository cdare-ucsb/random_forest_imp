#ifndef DATAFRAME_H
#define DATAFRAME_H

using std::vector;
using std::string;
using std::unique_ptr;

class DataFrame {
    protected:
        vector<vector<double>> data;

        // Function to calculate entropy of a dataset
        double calculateEntropy(const vector<double>& labels);
        // Function to calculate information gain of an attribute
        double calculateInformationGain(int attributeIndex, string label_column);
        unique_ptr<DataFrame> filter_lt(int attributeIndex, double threshold);
        unique_ptr<DataFrame> filter_leq(int attributeIndex, double threshold);
        unique_ptr<DataFrame> filter_gt(int attributeIndex, double threshold);
        unique_ptr<DataFrame> filter_geq(int attributeIndex, double threshold);
        unique_ptr<DataFrame> filter_eq(int attributeIndex, double threshold);
        unique_ptr<DataFrame> filter_neq(int attributeIndex, double threshold);


    public:
        vector<string> columns;

        // Constructor
        DataFrame(std::vector<vector<double>> data, vector<string> columns);
        // Destructor
        ~DataFrame();

        // Getters
        vector<vector<double>> get_data();
        int get_num_rows();
        vector<double> get_column(string col_name);

        // Function to print the DataFrame
        string print();

        // Function to find the best attribute (highest information gain)
        int selectBestAttribute(string label_column);
        double column_median(string col_name);
        double column_mode(string col_name);

        // Filter method
        unique_ptr<DataFrame> filter(int attributeIndex, double threshold, string condition);

        
};








#endif // DATAFRAME_H