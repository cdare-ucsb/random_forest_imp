#ifndef CLASSIFIER_H
#define CLASSIFIER_H


#include <vector>
#include <string>
#include <memory>
#include "DataFrame.h"


/**
 * @class Classifier
 * @brief Abstract base class for classification models
 * 
 * This class represents an abstract base class for classification models. Each classification model
 * should inherit from this class and implement the predict and fit methods.
 */
class Classifier {

    public:
        /**
         * @brief Constructor for the Classifier class
         */
        Classifier() {}; 
        /**
         * @brief Destructor for the Classifier class
         */
        ~Classifier() {};

        /**
         * @brief Function to make a prediction
         * @param sample Vector of feature values for a single sample
         * @return Predicted class label for the sample
         * 
         * This function takes a vector of feature values for a single sample and returns the predicted class label.
         */
        virtual double predict(const std::vector<double>& sample) const = 0;

        /**
         * @brief Function to fit the model to the data
         * @param data Data to fit the model to
         * @param label_column Name of the column containing the labels
         * 
         * This function fits the model to the data by training the model on the data. The function takes a DataFrame
         * containing the data and the name of the column containing the labels.
         */
        virtual void fit(std::shared_ptr<DataFrame> data, const std::string& label_column) = 0;
};

#endif // CLASSIFIER_H