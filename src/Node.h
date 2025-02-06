#ifndef NODE_H
#define NODE_H

using std::unique_ptr;
using std::vector;


class Node {
    public:
        unique_ptr<Node> left;
        unique_ptr<Node> right;
        // Constructor
        Node();
        // Destructor
        virtual  ~Node();
        virtual double predict(const vector<double>& sample) const = 0;
        virtual std::string print() = 0;
        virtual int get_num_nodes() = 0;
        virtual int get_height() = 0;
};


class LeafNode : public Node {
    protected:
        double value;
        unique_ptr<Node> left;
        unique_ptr<Node> right;
    public:
        // Constructor
        LeafNode(double val);
        // Destructor
        ~LeafNode();

        // Override methods
        double predict(const vector<double>& sample) const override;
        std::string print() override;

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
        unique_ptr<Node> left;
        unique_ptr<Node> right;
        // Constructor
        DecisionNode(int feature_index, double threshold, unique_ptr<Node> left_child, unique_ptr<Node> right_child);
        // Destructor
        ~DecisionNode();

        // Override methods
        double predict(const vector<double>& sample) const override;
        std::string print() override;

        // Setters
        void set_feature_index(int idx);
        void set_threshold(double thr);

        // Getters
        int get_feature_index() const;
        double get_threshold() const;
        int get_num_nodes() override;
        int get_height() override;
};

#endif // NODE_H