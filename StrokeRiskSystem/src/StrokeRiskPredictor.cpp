#include "StrokeRiskPredictor.h"
#include <stdexcept>
#include <iostream>

StrokeRiskPredictor::StrokeRiskPredictor(RandomForest* model)
    : m_randomForestModel(model) {
    if (!m_randomForestModel) {
        throw std::runtime_error("StrokeRiskPredictor: RandomForest model pointer cannot be null.");
    }
}

std::string StrokeRiskPredictor::predict(const UserProfile& userProfile, const DataProcessor& processor) const {
    if (!m_randomForestModel) {
        throw std::runtime_error("StrokeRiskPredictor: Model not initialized for prediction.");
    }

    std::vector<double> features = processor.preprocessUserProfile(userProfile);

    if (features.empty()) {
        throw std::runtime_error("StrokeRiskPredictor: Failed to preprocess user profile into features.");
    }

    std::string prediction = m_randomForestModel->predict(features);

    std::cout << "Prediction for user profile: " << prediction << std::endl;
    return prediction;
}