#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <limits>

struct Node {
    bool is_leaf;
    std::string prediction_class;

    int feature_index;
    double split_value;

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(const std::string& prediction_class) :
        is_leaf(true), prediction_class(prediction_class), feature_index(-1), split_value(0.0) {}

    Node(int feature_index, double split_value) :
        is_leaf(false), prediction_class(""), feature_index(feature_index), split_value(split_value) {}

    Node(int feature_index) :
        is_leaf(false), prediction_class(""), feature_index(feature_index), split_value(std::numeric_limits<double>::lowest()) {}
};

class DecisionTree {
public:
    DecisionTree();
    ~DecisionTree();

    void fit(const std::vector<std::vector<double>>& X, const std::vector<std::string>& y);

    std::string predict(const std::vector<double>& features) const;

private:
    std::unique_ptr<Node> root;

    std::unique_ptr<Node> build_tree(const std::vector<std::vector<double>>& X,
                                     const std::vector<std::string>& y,
                                     const std::vector<int>& feature_indices,
                                     int depth, int max_depth, int min_samples_split);

    double calculate_gini_impurity(const std::vector<std::string>& labels) const;

    std::pair<int, double> find_best_split(const std::vector<std::vector<double>>& X,
                                           const std::vector<std::string>& y,
                                           const std::vector<int>& feature_indices) const;

    std::string traverse_tree(const Node* node, const std::vector<double>& features) const;

    std::string get_most_common_label(const std::vector<std::string>& labels) const;
};

#endif // DECISION_TREE_H