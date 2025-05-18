#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H

#include "DecisionTree.h"
#include "DataProcessor.h"
#include "UserProfile.h"
#include <vector>
#include <string>
#include <memory>

class RandomForest {
public:
    RandomForest();

    void fit(const std::vector<std::vector<double>>& X, const std::vector<std::string>& y);

    std::string predict(const std::vector<double>& features) const;

    void trainModelWithNewData(const DataProcessor& newDataProcessor);

private:
    std::vector<std::unique_ptr<DecisionTree>> trees;
    int num_trees;
    double sample_ratio;
    int max_features;

    std::pair<std::vector<std::vector<double>>, std::vector<std::string>>
    bootstrap_sample(const std::vector<std::vector<double>>& X, const std::vector<std::string>& y) const;
};

#endif // RANDOM_FOREST_H