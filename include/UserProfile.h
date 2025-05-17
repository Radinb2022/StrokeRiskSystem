#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <string>
#include <map>
#include <armadillo>

class UserProfile {
public:
    int age;
    std::map<std::string, bool> healthIndicators;
    int label;  // 0 for not at risk, 1 for at risk

    // Default constructor
    UserProfile() : age(0), label(0) {}

    // Constructor with age
    UserProfile(int a) : age(a), label(0) {}

    // Add a health indicator
    void addHealthIndicator(const std::string& indicator, bool value) {
        healthIndicators[indicator] = value;
    }

    // Convert health indicators to feature vector
    arma::vec Features() const {
        size_t numFeatures = healthIndicators.size() + 1;  // +1 for age
        arma::vec features(numFeatures);
        
        features(0) = age;  // First feature is age
        size_t idx = 1;
        for (const auto& kv : healthIndicators) {
            features(idx++) = kv.second ? 1.0 : 0.0;
        }
        
        return features;
    }

    // Get the label (0 or 1)
    int Label() const {
        return label;
    }

    // Set the label
    void setLabel(int l) {
        label = l;
    }
};

// Supported healthIndicators keys:
// "HighBloodPressure", "ChestPain", "Dizziness", "Fatigue & Weakness", 
// "Swelling (Edema)", "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating", 
// "Chest Discomfort (Activity)", "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom"

#endif