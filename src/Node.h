#ifndef NODE_H
#define NODE_H

class Node {
    public:
        Node* left;
        Node* right;
        // Constructor
        Node();
        // Destructor
        virtual  ~Node();
        virtual double predict(std::vector<double> sample) = 0;
        virtual std::string print(std::vector<std::string> col_names) = 0;
        virtual int get_num_nodes() = 0;
        virtual int get_height() = 0;
};


class LeafNode : public Node {
    protected:
        double value;
        Node* left;
        Node* right;
    public:
        // Constructor
        LeafNode(double val);
        // Destructor
        ~LeafNode();

        // Override methods
        double predict(std::vector<double> sample) override;
        std::string print(std::vector<std::string> col_names) override;

        // Setters
        void set_value(double val);
        int get_num_nodes() override;
        int get_height() override;
        
};

class DecisionNode : public Node{
    protected:
        int feature_index;
        double threshold;

    public:
        Node* left;
        Node* right;
        // Constructor
        DecisionNode(int feature_index, double threshold, Node* left_child, Node* right_child);
        // Destructor
        ~DecisionNode();

        // Override methods
        double predict(std::vector<double> sample) override;
        std::string print(std::vector<std::string> col_names) override;

        // Setters
        void set_feature_index(int idx);
        void set_threshold(double thr);

        // Getters
        int get_feature_index();
        double get_threshold();
        int get_num_nodes() override;
        int get_height() override;
};

#endif // NODE_H