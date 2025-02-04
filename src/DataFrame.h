#ifndef DATAFRAME_H
#define DATAFRAME_H

using std::vector;
using std::string;

class DataFrame {
    protected:
        vector<vector<double>> data;
        vector<string> target;


        // Helper functions
        double calculateEntropy(const vector<double>& labels);
        DataFrame* filter_lt(int attributeIndex, double threshold);
        DataFrame* filter_gt(int attributeIndex, double threshold);
        DataFrame* filter_leq(int attributeIndex, double threshold);
        DataFrame* filter_geq(int attributeIndex, double threshold);
        DataFrame* filter_eq(int attributeIndex, double threshold);
        DataFrame* filter_neq(int attributeIndex, double threshold);

    public:
        vector<string> columns;

        // Constructor
        DataFrame(vector<vector<double>> data, vector<string> columns);
        // Destructor
        ~DataFrame();

        // Getters
        vector<vector<double>> get_data();
        int get_num_rows();
        vector<double> get_column(string col_name);

        // Print method
        string print();

        int selectBestAttribute(string label_column);
        double column_median(string col_name);

        // Filter method
        DataFrame* filter(int attributeIndex, double threshold, string condition);

        // Helper functions
        
        double calculateInformationGain(int attributeIndex, string label_column);
        
};



#endif // DATAFRAME_H