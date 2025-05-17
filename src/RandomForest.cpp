#include "RandomForest.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>

namespace {
// Helper to get a random float between min and max
float randFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max-min)));
}
}

// Recursively build a simple tree (random feature, random threshold)
TreeNode* buildTree(const std::vector<UserProfile>& data, int depth, int max_depth) {
    if (data.empty() || depth >= max_depth) {
        // Majority class leaf
        int at_risk = 0;
        for (const auto& u : data) at_risk += (u.healthIndicators.count("AtRisk") && u.healthIndicators.at("AtRisk"));
        TreeNode* leaf = new TreeNode();
        leaf->is_leaf = true;
        leaf->prediction = (at_risk > (int)data.size()/2) ? 1 : 0;
        return leaf;
    }
    // Randomly pick a feature
    std::vector<std::string> features = {"HighBloodPressure", "ChestPain", "Dizziness", "Fatigue & Weakness", "Swelling (Edema)", "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating", "Chest Discomfort (Activity)", "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom", "Age"};
    std::string feature = features[rand() % features.size()];
    float threshold = 0;
    if (feature == "Age") {
        int min_age = 200, max_age = -1;
        for (const auto& u : data) {
            min_age = std::min(min_age, u.age);
            max_age = std::max(max_age, u.age);
        }
        threshold = randFloat(min_age, max_age);
    }
    // Split data
    std::vector<UserProfile> left, right;
    for (const auto& u : data) {
        if (feature == "Age") {
            if (u.age < threshold) left.push_back(u);
            else right.push_back(u);
        } else {
            if (u.healthIndicators.count(feature) && u.healthIndicators.at(feature)) left.push_back(u);
            else right.push_back(u);
        }
    }
    TreeNode* node = new TreeNode();
    node->feature = feature;
    node->threshold = threshold;
    node->is_leaf = false;
    node->left = buildTree(left, depth+1, max_depth);
    node->right = buildTree(right, depth+1, max_depth);
    return node;
}

void RandomForest::train(const std::vector<UserProfile>& data, int n_trees, int max_depth) {
    srand((unsigned)time(0));
    trees.clear();
    for (int i = 0; i < n_trees; ++i) {
        // Bootstrap sample
        std::vector<UserProfile> sample;
        for (size_t j = 0; j < data.size(); ++j) sample.push_back(data[rand()%data.size()]);
        trees.push_back(buildTree(sample, 0, max_depth));
    }
}

int predictTree(const TreeNode* node, const UserProfile& user) {
    if (!node) return 0;
    if (node->is_leaf) return node->prediction;
    if (node->feature == "Age") {
        if (user.age < node->threshold) return predictTree(node->left, user);
        else return predictTree(node->right, user);
    } else {
        if (user.healthIndicators.count(node->feature) && user.healthIndicators.at(node->feature))
            return predictTree(node->left, user);
        else
            return predictTree(node->right, user);
    }
}

int RandomForest::predict(const UserProfile& user) const {
    int votes = 0;
    for (const auto& t : trees) votes += predictTree(t, user);
    return (votes > (int)trees.size()/2) ? 1 : 0;
}

float RandomForest::predict_proba(const UserProfile& user) const {
    int votes = 0;
    for (const auto& t : trees) votes += predictTree(t, user);
    return (float)votes / trees.size();
}

void freeTree(TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

RandomForest::~RandomForest() {
    for (auto t : trees) freeTree(t);
} 