#include "KaggleData.h"
#include <fstream>
#include <sstream>

void KaggleData::loadSampleData() {
    std::ifstream file("data/data.csv");
    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, ageStr;
        std::vector<std::string> fields(10);
        std::getline(ss, id, ',');
        std::getline(ss, ageStr, ',');
        for (int i = 0; i < 10; ++i) std::getline(ss, fields[i], ',');
        UserProfile user(id, std::stoi(ageStr));
        std::vector<std::string> keys = {"HighBloodPressure", "ChestPain", "Dizziness", "Fatigue & Weakness", "Swelling (Edema)", "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating", "Chest Discomfort (Activity)", "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom"};
        for (int i = 0; i < 10; ++i) user.addHealthIndicator(keys[i], fields[i] == "Yes");
        dataset.push_back(user);
    }
    file.close();
}

void KaggleData::appendUser(const UserProfile& user) {
    dataset.push_back(user);
    saveToCSV("data/data.csv");
}

void KaggleData::saveToCSV(const std::string& filename) {
    std::ofstream file(filename);
    // Write header
    file << "PatientId,Age,HighBloodPressure,ChestPain,Dizziness,Fatigue & Weakness,Swelling (Edema),Pain in Neck/Jaw/Shoulder/Back,Excessive Sweating,Chest Discomfort (Activity),Snoring/Sleep Apnea,Anxiety/Feeling of Doom\n";
    for (const auto& user : dataset) {
        file << user.patientId << "," << user.age;
        std::vector<std::string> keys = {"HighBloodPressure", "ChestPain", "Dizziness", "Fatigue & Weakness", "Swelling (Edema)", "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating", "Chest Discomfort (Activity)", "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom"};
        for (const auto& k : keys) file << "," << (user.healthIndicators.count(k) && user.healthIndicators.at(k) ? "Yes" : "No");
        file << "\n";
    }
    file.close();
}