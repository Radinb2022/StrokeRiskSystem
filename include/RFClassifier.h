#ifndef RFCLASSIFIER_H
#define RFCLASSIFIER_H

#include "UserProfile.h"
#include <vector>
#include <armadillo>
#include <mlpack/core.hpp>
#include <mlpack/methods/random_forest/random_forest.hpp>

class RFClassifier {
private:
    // mlpack RandomForest object
    mlpack::RandomForest<mlpack::GiniGain, mlpack::RandomDimensionSelect> rf;

public:
    // Train on labeled user profiles
    void Train(const std::vector<UserProfile>& data);

    // Predict class label (0 or 1)
    int Predict(const UserProfile& user) const;

    // Predict probability of class=1
    double PredictProba(const UserProfile& user) const;
};

#endif 