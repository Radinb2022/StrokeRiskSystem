#include "../include/RandomForestUI.h"
#include "../include/UserProfile.h"
#include "../include/StrokeRiskPredictor.h"
#include "../include/KaggleData.h"
#include "../include/Comparison.h"
#include <iostream>
#include <chrono>
#include <limits>
#include <set>
#include <iomanip>
#include <algorithm>

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

UserProfile inputUserProfile() {
    UserProfile user;
    int age;
    std::string input;
    
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
    for (size_t i = 0; i < dataset.size(); ++i) {
        const auto& u = dataset[i];
        std::cout << "Patient #" << (i + 1) << ", Age: " << u.age << ", Risk: " 
                  << (u.label ? "At Risk" : "Not At Risk") << "\n";
        std::cout << "Health Indicators: ";
        for (const auto& kv : u.healthIndicators) {
            std::cout << kv.first << "=" << (kv.second ? "Yes" : "No") << ", ";
        }
        std::cout << "\n";
    }
    std::cout << "-----------------------\n";
}

void RandomForestUI::run() {
    StrokeRiskPredictor predictor;
    KaggleData data;
    
    // Load training data
    if (!data.loadFromCSV("data/stroke_data.csv")) {
        std::cerr << "Failed to load training data!" << std::endl;
        return;
    }

    // Train the model
    predictor.TrainModel(data.dataset);
    std::cout << "Model trained successfully!" << std::endl;

    while (true) {
        std::cout << "\n=== Stroke Risk Assessment ===" << std::endl;
        std::cout << "1. Assess new patient" << std::endl;
        std::cout << "2. View database" << std::endl;
        std::cout << "3. Help" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 4) break;

        switch (choice) {
            case 1: {
                UserProfile user = inputUserProfile();
                
                // Get prediction
                int pred = predictor.PredictRisk(user);
                double proba = predictor.RiskProbability(user);
                std::string message = predictor.OutcomeMessage(user);

                std::cout << "\n=== Assessment Results ===" << std::endl;
                std::cout << message << std::endl;
                std::cout << "Risk Level: " << (pred ? "At Risk" : "Not At Risk") << std::endl;
                std::cout << "Risk Probability: " << std::fixed << std::setprecision(2) << (proba * 100) << "%" << std::endl;
                std::cout << "Detailed Analysis:" << std::endl;
                std::cout << "- Age: " << user.age << std::endl;
                for (const auto& kv : user.healthIndicators) {
                    std::cout << "- " << kv.first << ": " << (kv.second ? "Yes" : "No") << std::endl;
                }
                break;
            }
            case 2:
                printDatabase(data.dataset);
                break;
            case 3:
                printHelp();
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}