#include "RandomForestUI.h"
#include "UserProfile.h"
#include "StrokeRiskPredictor.h"
#include "KaggleData.h"
#include "Comparison.h"
#include <iostream>
#include <chrono>

void RandomForestUI::run() {
    using namespace std::chrono;

    UserProfile user("U001", 70);
    user.addHealthIndicator("HighBloodPressure", true);
    user.addHealthIndicator("ChestPain", true);

    StrokeRiskPredictor predictor;

    auto start = high_resolution_clock::now();
    float risk = predictor.calculateRisk(user);
    std::string label = predictor.generateAlert(risk);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Risk Score: " << risk << "%\n";
    std::cout << "Prediction: " << label << "\n";
    std::cout << "Prediction Time: " << duration.count() << "ms\n";

    KaggleData data;
    data.loadSampleData();

    Comparison cmp;
    UserProfile opposite = cmp.findOppositeRiskProfile(user, data.dataset, predictor);
    std::cout << "\nSimilar Opposite Risk Profile Found: " << opposite.patientId << ", Age: " << opposite.age << "\n";
}