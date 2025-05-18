#include "RandomForest.h"
#include <random>
#include <algorithm>
#include <map>
#include <iostream>
#include <stdexcept>

static std::mt19937 rng_rf(std::random_device{}());

RandomForest::RandomForest()
    : num_trees(10),
      sample_ratio(0.8),
      max_features(-1)
{
}

void RandomForest::fit(const std::vector<std::vector<double>>& X, const std::vector<std::string>& y) {
    if (X.empty() || y.empty() || X.size() != y.size()) {
        throw std::runtime_error("RandomForest::fit: Input data or labels are empty or mismatched.");
    }
    if (X[0].empty()) {
        throw std::runtime_error("RandomForest::fit: Input features are empty.");
    }

    trees.clear();

    if (max_features == -1) {
        max_features = static_cast<int>(std::sqrt(X[0].size()));
        if (max_features == 0 && X[0].size() > 0) max_features = 1;
    }

    std::cout << "Training RandomForest with " << num_trees << " trees..." << std::endl;
    for (int i = 0; i < num_trees; ++i) {
        std::cout << "  Building tree " << (i + 1) << "..." << std::endl;
        auto [sample_X, sample_y] = bootstrap_sample(X, y);

        std::unique_ptr<DecisionTree> tree = std::make_unique<DecisionTree>();
        tree->fit(sample_X, sample_y);
        trees.push_back(std::move(tree));
    }
    std::cout << "RandomForest training complete." << std::endl;
}

std::string RandomForest::predict(const std::vector<double>& features) const {
    if (trees.empty()) {
        return "Not Trained";
    }

    std::map<std::string, int> predictions_count;
    for (const auto& tree : trees) {
        std::string prediction = tree->predict(features);
        predictions_count[prediction]++;
    }

    std::string final_prediction = "";
    int max_votes = -1;

    for (const auto& pair : predictions_count) {
        if (pair.second > max_votes) {
            max_votes = pair.second;
            final_prediction = pair.first;
        }
    }
    return final_prediction;
}

void RandomForest::trainModelWithNewData(const DataProcessor& newDataProcessor) {
    fit(newDataProcessor.getProcessedFeatures(), newDataProcessor.getLabels());
}

std::pair<std::vector<std::vector<double>>, std::vector<std::string>>
RandomForest::bootstrap_sample(const std::vector<std::vector<double>>& X, const std::vector<std::string>& y) const {
    size_t num_samples = X.size();
    size_t sample_size = static_cast<size_t>(num_samples * sample_ratio);

    std::vector<std::vector<double>> sample_X;
    std::vector<std::string> sample_y;

    std::uniform_int_distribution<size_t> dist(0, num_samples - 1);

    for (size_t i = 0; i < sample_size; ++i) {
        size_t idx = dist(rng_rf);
        sample_X.push_back(X[idx]);
        sample_y.push_back(y[idx]);
    }
    return {sample_X, sample_y};
}