#include "KaggleData.h"
#include <fstream>
#include <sstream>

void KaggleData::loadSampleData() {
    std::ifstream file("data/data.csv");
    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, ageStr, bp, chest, dizzy;
        std::getline(ss, id, ',');
        std::getline(ss, ageStr, ',');
        std::getline(ss, bp, ',');
        std::getline(ss, chest, ',');
        std::getline(ss, dizzy, ',');

        UserProfile user(id, std::stoi(ageStr));
        user.addHealthIndicator("HighBloodPressure", bp == "Yes");
        user.addHealthIndicator("ChestPain", chest == "Yes");
        user.addHealthIndicator("Dizziness", dizzy == "Yes");

        dataset.push_back(user);
    }
    file.close();
}