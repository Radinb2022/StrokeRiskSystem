#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H

#include "UserProfile.h"
#include <vector>
#include <string>

struct TreeNode {
    std::string feature;
    float threshold;
    bool is_leaf;
    int prediction; // 0 = Not At Risk, 1 = At Risk
    TreeNode* left;
    TreeNode* right;
    TreeNode() : is_leaf(false), prediction(-1), left(nullptr), right(nullptr) {}
};

class RandomForest {
public:
    std::vector<TreeNode*> trees;
    void train(const std::vector<UserProfile>& data, int n_trees = 10, int max_depth = 3);
    int predict(const UserProfile& user) const;
    float predict_proba(const UserProfile& user) const;
    ~RandomForest();
};

#endif 
