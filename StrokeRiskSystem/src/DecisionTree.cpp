#include "DecisionTree.h"
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <random>
#include <iostream>

static std::mt19937 rng(std::random_device{}());

DecisionTree::DecisionTree() : root(nullptr) {}

DecisionTree::~DecisionTree() {
}

void DecisionTree::fit(const std::vector<std::vector<double>>& X, const std::vector<std::string>& y) {
    if (X.empty() || y.empty() || X.size() != y.size()) {
        std::cerr << "DecisionTree::fit: Empty or mismatched data/labels." << std::endl;
        root = nullptr;
        return;
    }

    std::vector<int> feature_indices(X[0].size());
    std::iota(feature_indices.begin(), feature_indices.end(), 0);

    int max_depth = 10;
    int min_samples_split = 2;

    root = build_tree(X, y, feature_indices, 0, max_depth, min_samples_split);
}

std::string DecisionTree::predict(const std::vector<double>& features) const {
    if (!root) {
        return "Not Trained";
    }
    return traverse_tree(root.get(), features);
}

std::unique_ptr<Node> DecisionTree::build_tree(const std::vector<std::vector<double>>& X,
                                                 const std::vector<std::string>& y,
                                                 const std::vector<int>& feature_indices,
                                                 int depth, int max_depth, int min_samples_split) {
    if (y.empty()) return nullptr;
    bool all_same = true;
    for (size_t i = 1; i < y.size(); ++i) {
        if (y[i] != y[0]) {
            all_same = false;
            break;
        }
    }
    if (all_same) {
        return std::make_unique<Node>(y[0]);
    }

    if (depth >= max_depth) {
        return std::make_unique<Node>(get_most_common_label(y));
    }

    if (y.size() < static_cast<size_t>(min_samples_split)) {
        return std::make_unique<Node>(get_most_common_label(y));
    }

    if (feature_indices.empty()) {
        return std::make_unique<Node>(get_most_common_label(y));
    }

    std::pair<int, double> best_split = find_best_split(X, y, feature_indices);
    int best_feature_idx = best_split.first;
    double best_split_val = best_split.second;

    if (best_feature_idx == -1) {
        return std::make_unique<Node>(get_most_common_label(y));
    }

    std::unique_ptr<Node> node = std::make_unique<Node>(best_feature_idx, best_split_val);

    std::vector<std::vector<double>> X_left, X_right;
    std::vector<std::string> y_left, y_right;

    for (size_t i = 0; i < X.size(); ++i) {
        if (X[i][best_feature_idx] <= best_split_val) {
            X_left.push_back(X[i]);
            y_left.push_back(y[i]);
        } else {
            X_right.push_back(X[i]);
            y_right.push_back(y[i]);
        }
    }

    if (X_left.empty() || X_right.empty()) {
        return std::make_unique<Node>(get_most_common_label(y));
    }

    node->left = build_tree(X_left, y_left, feature_indices, depth + 1, max_depth, min_samples_split);
    node->right = build_tree(X_right, y_right, feature_indices, depth + 1, max_depth, min_samples_split);

    return node;
}

double DecisionTree::calculate_gini_impurity(const std::vector<std::string>& labels) const {
    if (labels.empty()) return 0.0;
    std::map<std::string, int> counts;
    for (const auto& label : labels) {
        counts[label]++;
    }

    double impurity = 1.0;
    for (const auto& pair : counts) {
        double prob = static_cast<double>(pair.second) / labels.size();
        impurity -= prob * prob;
    }
    return impurity;
}

std::pair<int, double> DecisionTree::find_best_split(const std::vector<std::vector<double>>& X,
                                                     const std::vector<std::string>& y,
                                                     const std::vector<int>& feature_indices) const {
    if (y.empty() || feature_indices.empty()) {
        return {-1, 0.0};
    }

    double best_gini_gain = -1.0;
    int best_feature_idx = -1;
    double best_split_val = 0.0;

    double current_gini = calculate_gini_impurity(y);

    std::vector<int> current_feature_indices = feature_indices;
    std::shuffle(current_feature_indices.begin(), current_feature_indices.end(), rng);
    int num_features_to_try = std::min((int)current_feature_indices.size(), (int)std::sqrt(X[0].size()));

    for (int k = 0; k < num_features_to_try; ++k) {
        int feature_idx = current_feature_indices[k];

        double sum_val = 0.0;
        for (size_t i = 0; i < X.size(); ++i) {
            sum_val += X[i][feature_idx];
        }
        double candidate_split_val = X.empty() ? 0.0 : sum_val / X.size();

        std::vector<std::string> y_left, y_right;
        for (size_t i = 0; i < X.size(); ++i) {
            if (X[i][feature_idx] <= candidate_split_val) {
                y_left.push_back(y[i]);
            } else {
                y_right.push_back(y[i]);
            }
        }

        if (y_left.empty() || y_right.empty()) continue;

        double gini_left = calculate_gini_impurity(y_left);
        double gini_right = calculate_gini_impurity(y_right);

        double weighted_gini_children = (static_cast<double>(y_left.size()) / y.size()) * gini_left +
                                        (static_cast<double>(y_right.size()) / y.size()) * gini_right;

        double gain = current_gini - weighted_gini_children;

        if (gain > best_gini_gain) {
            best_gini_gain = gain;
            best_feature_idx = feature_idx;
            best_split_val = candidate_split_val;
        }
    }

    return {best_feature_idx, best_split_val};
}

std::string DecisionTree::traverse_tree(const Node* node, const std::vector<double>& features) const {
    if (node->is_leaf) {
        return node->prediction_class;
    }

    if (features[node->feature_index] <= node->split_value) {
        return traverse_tree(node->left.get(), features);
    } else {
        return traverse_tree(node->right.get(), features);
    }
}

std::string DecisionTree::get_most_common_label(const std::vector<std::string>& labels) const {
    if (labels.empty()) return "";
    std::map<std::string, int> counts;
    for (const auto& label : labels) {
        counts[label]++;
    }

    std::string most_common = "";
    int max_count = -1;
    for (const auto& pair : counts) {
        if (pair.second > max_count) {
            max_count = pair.second;
            most_common = pair.first;
        }
    }
    return most_common;
}