#ifndef CLASSIFIER_H
#define CLASSIFIER_H


#include <vector>
#include <string>
#include <memory>
#include "DataFrame.h"



class Classifier {

    public:
        Classifier() {};
        ~Classifier() {};

        virtual double predict(const std::vector<double>& sample) const = 0;
        virtual void fit(std::shared_ptr<DataFrame> data, const std::string& label_column) = 0;
};

#endif // CLASSIFIER_H