#include "DataProcessor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <cmath> // For isnan

DataProcessor::DataProcessor()
    : ageMinMax({std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest()}) {}

bool DataProcessor::loadCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open CSV file: " << filename << std::endl;
        return false;
    }

    processedFeatures.clear();
    labels.clear();
    rawData.clear();
    header.clear();
    featureColumnMap.clear();
    ageMinMax = {std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest()}; // Reset min/max

    std::string line;
    std::getline(file, line); // Read header
    std::stringstream ss_header(line);
    std::string cell;

    int col_idx = 0;
    while (std::getline(ss_header, cell, ',')) {
        header.push_back(cell);
        featureColumnMap[cell] = col_idx++;
    }

    std::vector<std::vector<std::string>> rawRows;
    while (std::getline(file, line)) {
        rawRows.push_back({});
        std::stringstream ss_line(line);
        while (std::getline(ss_line, cell, ',')) {
            rawRows.back().push_back(cell);
        }
        if (rawRows.back().size() != header.size()) {
            std::cerr << "Warning: Mismatched column count in row: " << line << std::endl;
            rawRows.pop_back();
        }
    }
    file.close();

    // Pass 1: Calculate Min/Max for Age
    int age_col_idx = getColumnIndex("age");
    if (age_col_idx != -1) {
        for (const auto& row : rawRows) {
            if (age_col_idx < row.size()) {
                try {
                    double val = std::stod(row[age_col_idx]);
                    if (val < ageMinMax.first) ageMinMax.first = val;
                    if (val > ageMinMax.second) ageMinMax.second = val;
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Warning: Invalid age value in CSV: " << row[age_col_idx] << std::endl;
                }
            }
        }
    }

    // Pass 2: Process data into features and labels
    for (const auto& row : rawRows) {
        UserProfile userProfile;
        std::string label_str;

        for (size_t i = 0; i < header.size(); ++i) {
            const std::string& col_name = header[i];
            const std::string& value = row[i];

            if (col_name == "age") userProfile.setAge(std::stoi(value));
            else if (col_name == "HighBloodPressure") userProfile.setHighBloodPressure(value == "1");
            else if (col_name == "ChestPain") userProfile.setChestPain(value == "1");
            else if (col_name == "Dizziness") userProfile.setDizziness(value == "1");
            else if (col_name == "Fatigue_Weakness") userProfile.setFatigueWeakness(value == "1");
            else if (col_name == "Swelling_Edema") userProfile.setSwellingEdema(value == "1");
            else if (col_name == "Pain_NeckJawShoulderBack") userProfile.setPainNeckJawShoulderBack(value == "1");
            else if (col_name == "ExcessiveSweating") userProfile.setExcessiveSweating(value == "1");
            else if (col_name == "Chest_Discomfort_Activity") userProfile.setChestDiscomfortActivity(value == "1");
            else if (col_name == "Snoring_SleepApnea") userProfile.setSnoringSleepApnea(value == "1");
            else if (col_name == "Anxiety_FeelingDoom") userProfile.setAnxietyFeelingDoom(value == "1");
            else if (col_name == "stroke") label_str = value;
            // 'id' is ignored
        }
        labels.push_back(label_str);
        rawData.push_back(userProfile);
        processedFeatures.push_back(preprocessUserProfile(userProfile)); // Use its own preprocessor
    }

    return true;
}

std::vector<double> DataProcessor::preprocessUserProfile(const UserProfile& userProfile) const {
    std::vector<double> features;

    // Order of features must match the order in which they are processed in loadCSV
    // age (normalized)
    if (ageMinMax.first != std::numeric_limits<double>::max() && ageMinMax.second != std::numeric_limits<double>::lowest()) {
        features.push_back(normalize(userProfile.getAge(), ageMinMax.first, ageMinMax.second));
    } else {
        features.push_back(0.0); // Fallback if age min/max not determined (e.g., empty dataset)
    }

    // Binary attributes (0.0 or 1.0)
    features.push_back(userProfile.getHighBloodPressure() ? 1.0 : 0.0);
    features.push_back(userProfile.getChestPain() ? 1.0 : 0.0);
    features.push_back(userProfile.getDizziness() ? 1.0 : 0.0);
    features.push_back(userProfile.getFatigueWeakness() ? 1.0 : 0.0);
    features.push_back(userProfile.getSwellingEdema() ? 1.0 : 0.0);
    features.push_back(userProfile.getPainNeckJawShoulderBack() ? 1.0 : 0.0);
    features.push_back(userProfile.getExcessiveSweating() ? 1.0 : 0.0);
    features.push_back(userProfile.getChestDiscomfortActivity() ? 1.0 : 0.0);
    features.push_back(userProfile.getSnoringSleepApnea() ? 1.0 : 0.0);
    features.push_back(userProfile.getAnxietyFeelingDoom() ? 1.0 : 0.0);

    return features;
}

double DataProcessor::normalize(double value, double min_val, double max_val) const {
    if (max_val == min_val) return 0.0;
    return (value - min_val) / (max_val - min_val);
}

int DataProcessor::getColumnIndex(const std::string& featureName) const {
    auto it = featureColumnMap.find(featureName);
    if (it != featureColumnMap.end()) {
        return it->second;
    }
    return -1; // Not found
}

const std::vector<std::vector<double>>& DataProcessor::getProcessedFeatures() const {
    return processedFeatures;
}

const std::vector<std::string>& DataProcessor::getLabels() const {
    return labels;
}

const std::vector<UserProfile>& DataProcessor::getRawData() const {
    return rawData;
}