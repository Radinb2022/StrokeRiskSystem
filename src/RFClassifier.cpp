#include "../include/RFClassifier.h"
#include <armadillo>

void RFClassifier::Train(const std::vector<UserProfile>& dataset) {
    // Convert dataset to feature matrix and labels
    arma::mat features(dataset[0].Features().n_elem, dataset.size());
    arma::Row<size_t> labels(dataset.size());
    
    for (size_t i = 0; i < dataset.size(); ++i) {
        features.col(i) = dataset[i].Features();
        labels(i) = dataset[i].Label();
    }
    
    // Train the random forest
    rf = mlpack::RandomForest<mlpack::GiniGain, mlpack::RandomDimensionSelect>();
    rf.Train(features, labels, 10, 3);  // 10 trees, 3 features per split
}

int RFClassifier::Predict(const UserProfile& user) const {
    arma::mat features = user.Features();
    arma::Row<size_t> prediction;
    arma::mat probabilities;
    
    rf.Classify(features, prediction, probabilities);
    return prediction(0);
}

double RFClassifier::PredictProba(const UserProfile& user) const {
    arma::mat features = user.Features();
    arma::Row<size_t> prediction;
    arma::mat probabilities;
    
    rf.Classify(features, prediction, probabilities);
    return probabilities(1);  // Probability of class 1 (at risk)
} 