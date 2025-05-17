#include "../include/KaggleData.h"
#include <fstream>
#include <sstream>
#include <iostream>

void KaggleData::loadSampleData() {
    // Sample data for testing
    UserProfile sample1(65);
    sample1.addHealthIndicator("HighBloodPressure", true);
    sample1.addHealthIndicator("ChestPain", true);
    sample1.addHealthIndicator("Dizziness", true);
    sample1.setLabel(1);
    dataset.push_back(sample1);

    UserProfile sample2(45);
    sample2.addHealthIndicator("HighBloodPressure", false);
    sample2.addHealthIndicator("ChestPain", false);
    sample2.addHealthIndicator("Dizziness", false);
    sample2.setLabel(0);
    dataset.push_back(sample2);

    UserProfile sample3(55);
    sample3.addHealthIndicator("HighBloodPressure", true);
    sample3.addHealthIndicator("ChestPain", false);
    sample3.addHealthIndicator("Dizziness", true);
    sample3.setLabel(0);
    dataset.push_back(sample3);
}

bool KaggleData::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        UserProfile user;

        // Read age
        std::getline(ss, value, ',');
        user.age = std::stoi(value);

        // Read health indicators
        std::vector<std::string> indicators = {
            "HighBloodPressure", "ChestPain", "Dizziness",
            "Fatigue & Weakness", "Swelling (Edema)",
            "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating",
            "Chest Discomfort (Activity)", "Snoring/Sleep Apnea",
            "Anxiety/Feeling of Doom"
        };

        for (const auto& indicator : indicators) {
            std::getline(ss, value, ',');
            user.addHealthIndicator(indicator, value == "1");
        }

        // Read label
        std::getline(ss, value, ',');
        user.setLabel(std::stoi(value));

        dataset.push_back(user);
    }

    return true;
}

void KaggleData::saveToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // Write header
    file << "Age,HighBloodPressure,ChestPain,Dizziness,Fatigue & Weakness,"
         << "Swelling (Edema),Pain in Neck/Jaw/Shoulder/Back,Excessive Sweating,"
         << "Chest Discomfort (Activity),Snoring/Sleep Apnea,Anxiety/Feeling of Doom,Label\n";

    // Write data
    for (const auto& user : dataset) {
        file << user.age;
        for (const auto& indicator : {
            "HighBloodPressure", "ChestPain", "Dizziness",
            "Fatigue & Weakness", "Swelling (Edema)",
            "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating",
            "Chest Discomfort (Activity)", "Snoring/Sleep Apnea",
            "Anxiety/Feeling of Doom"
        }) {
            file << "," << (user.healthIndicators.at(indicator) ? "1" : "0");
        }
        file << "," << user.label << "\n";
    }
}

void KaggleData::appendUser(const UserProfile& user) {
    dataset.push_back(user);
}