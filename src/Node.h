#ifndef NODE_H
#define NODE_H

class Node {
    protected:
        Node* left;
        Node* right;
    public:
        // Constructor
        Node();
        // Destructor
        virtual  ~Node();
        virtual double predict(std::vector<double> sample) = 0;
        virtual std::string print() = 0;
};


class LeafNode : public Node {
    protected:
        double value;
    public:
        // Constructor
        LeafNode(double val);
        // Destructor
        ~LeafNode();

        // Override methods
        double predict(std::vector<double> sample) override;
        std::string print() override;

        // Setters
        void set_value(double val);
        
};

class DecisionNode : public Node{
    protected:
        int feature_index;
        double threshold;
        Node* left;
        Node* right;
    public:
        // Constructor
        DecisionNode(int feature_index, double threshold, Node* left_child, Node* right_child);
        // Destructor
        ~DecisionNode();

        // Override methods
        double predict(std::vector<double> sample) override;
        std::string print() override;

        // Setters
        void set_feature_index(int idx);
        void set_threshold(double thr);
        void set_left(Node* left_child);
        void set_right(Node* right_child);

        // Getters
        int get_feature_index();
        double get_threshold();
        Node* get_left();
        Node* get_right();
};

#endif // NODE_H