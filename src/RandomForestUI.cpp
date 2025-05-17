#include "RandomForestUI.h"
#include "UserProfile.h"
#include "StrokeRiskPredictor.h"
#include "KaggleData.h"
#include "Comparison.h"
#include <iostream>
#include <chrono>
#include <limits>
#include <set>
#include <iomanip>

void printHelp() {
    std::cout << "\n--- Help: Input Fields ---\n";
    std::cout << "Age: Patient's age (integer)\n";
    std::cout << "HighBloodPressure: Yes/No\n";
    std::cout << "ChestPain: Yes/No\n";
    std::cout << "Dizziness: Yes/No\n";
    std::cout << "Fatigue & Weakness: Yes/No\n";
    std::cout << "Swelling (Edema): Yes/No\n";
    std::cout << "Pain in Neck/Jaw/Shoulder/Back: Yes/No\n";
    std::cout << "Excessive Sweating: Yes/No\n";
    std::cout << "Chest Discomfort (Activity): Yes/No\n";
    std::cout << "Snoring/Sleep Apnea: Yes/No\n";
    std::cout << "Anxiety/Feeling of Doom: Yes/No\n";
    std::cout << "-------------------------\n";
}

UserProfile inputUserProfile(const std::vector<UserProfile>& dataset) {
    std::string id;
    std::set<std::string> existing_ids;
    for (const auto& u : dataset) existing_ids.insert(u.patientId);
    while (true) {
        std::cout << "Create Your Patient ID: ";
        std::getline(std::cin, id);
        if (existing_ids.count(id)) {
            std::cout << "Patient ID already exists. Use a different ID or type 'y' to update: ";
            std::string resp;
            std::getline(std::cin, resp);
            if (resp == "y" || resp == "Y") break;
        } else {
            break;
        }
    }
    int age;
    std::string input;
    UserProfile user;
    user.patientId = id;
    std::cout << "Enter Age: ";
    std::cin >> age;
    user.age = age;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::vector<std::pair<std::string, std::string>> questions = {
        {"HighBloodPressure", "High Blood Pressure (Yes/No): "},
        {"ChestPain", "Chest Pain (Yes/No): "},
        {"Dizziness", "Dizziness (Yes/No): "},
        {"Fatigue & Weakness", "Fatigue & Weakness (Yes/No): "},
        {"Swelling (Edema)", "Swelling (Edema) (Yes/No): "},
        {"Pain in Neck/Jaw/Shoulder/Back", "Pain in Neck/Jaw/Shoulder/Back (Yes/No): "},
        {"Excessive Sweating", "Excessive Sweating (Yes/No): "},
        {"Chest Discomfort (Activity)", "Chest Discomfort (Activity) (Yes/No): "},
        {"Snoring/Sleep Apnea", "Snoring/Sleep Apnea (Yes/No): "},
        {"Anxiety/Feeling of Doom", "Anxiety/Feeling of Doom (Yes/No): "}
    };
    for (const auto& q : questions) {
        while (true) {
            std::cout << q.second;
            std::getline(std::cin, input);
            std::string lower = input;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            if (lower == "yes" || lower == "no") {
                user.addHealthIndicator(q.first, lower == "yes");
                break;
            } else {
                std::cout << "Please enter 'Yes' or 'No'.\n";
            }
        }
    }
    return user;
}

void printDatabase(const std::vector<UserProfile>& dataset) {
    std::cout << "\n--- Patient Database ---\n";
    for (const auto& u : dataset) {
        std::cout << "ID: " << u.patientId << ", Age: " << u.age << ", Indicators: ";
        for (const auto& kv : u.healthIndicators) {
            std::cout << kv.first << "=" << (kv.second ? "Yes" : "No") << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "-----------------------\n";
    std::cout << "Enter a Patient ID to view details or press Enter to return: ";
    std::string sel;
    std::getline(std::cin, sel);
    if (!sel.empty()) {
        auto it = std::find_if(dataset.begin(), dataset.end(), [&](const UserProfile& u){ return u.patientId == sel; });
        if (it != dataset.end()) {
            std::cout << "\n--- Patient Profile ---\n";
            std::cout << "ID: " << it->patientId << "\nAge: " << it->age << "\n";
            for (const auto& kv : it->healthIndicators) {
                std::cout << kv.first << ": " << (kv.second ? "Yes" : "No") << "\n";
            }
            std::cout << "----------------------\n";
        } else {
            std::cout << "Patient ID not found.\n";
        }
    }
}

void RandomForestUI::run() {
    using namespace std::chrono;
    KaggleData data;
    data.loadSampleData();
    StrokeRiskPredictor predictor;
    predictor.train(data.dataset);
    Comparison cmp;
    while (true) {
        std::cout << "\n--- Stroke Risk Prediction System ---\n";
        std::cout << "1. Predict Stroke Risk (Random Forest)\n";
        std::cout << "2. Add New User Data\n";
        std::cout << "3. Compare with Opposite Risk Profile\n";
        std::cout << "4. Help\n";
        std::cout << "5. Exit\n";
        std::cout << "6. Access Database\n";
        std::cout << "Select an option: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 1) {
            UserProfile user = inputUserProfile(data.dataset);
            auto start = high_resolution_clock::now();
            int pred = predictor.predict(user);
            float proba = predictor.predict_proba(user);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);
            std::cout << "Prediction: " << (pred ? "At Risk" : "Not At Risk") << "\n";
            std::cout << "Probability (fraction of trees voting At Risk): " << std::fixed << std::setprecision(2) << proba << "\n";
            if (proba == 0.0f || proba == 1.0f) {
                std::cout << "[Warning] The model is very certain. This may indicate underfitting or imbalanced data.\n";
            }
            std::cout << "Prediction Time: " << duration.count() << "ms\n";
        } else if (choice == 2) {
            UserProfile user = inputUserProfile(data.dataset);
            predictor.addTrainingData(user, data);
            std::cout << "User data added and model updated.\n";
        } else if (choice == 3) {
            UserProfile user = inputUserProfile(data.dataset);
            UserProfile opposite = cmp.findOppositeRiskProfile(user, data.dataset, predictor);
            if (opposite.patientId != "None") {
                std::cout << "\nSimilar Opposite Risk Profile Found: " << opposite.patientId << ", Age: " << opposite.age << "\n";
            } else {
                std::cout << "No opposite risk profile found.\n";
            }
        } else if (choice == 4) {
            printHelp();
        } else if (choice == 5) {
            break;
        } else if (choice == 6) {
            printDatabase(data.dataset);
        } else {
            std::cout << "Invalid option. Try again.\n";
        }
    }
}