#include "../include/Tester.h"
#include "../include/UserProfile.h"
#include "../include/StrokeRiskPredictor.h"
#include <cassert>

void Tester::runTests() {
    StrokeRiskPredictor predictor;

    // Test case 1: Low risk profile
    UserProfile tc1(35);
    tc1.addHealthIndicator("HighBloodPressure", false);
    tc1.addHealthIndicator("ChestPain", false);
    tc1.addHealthIndicator("Dizziness", false);
    tc1.setLabel(0);

    // Test case 2: Medium risk profile
    UserProfile tc2(55);
    tc2.addHealthIndicator("HighBloodPressure", true);
    tc2.addHealthIndicator("ChestPain", false);
    tc2.addHealthIndicator("Dizziness", true);
    tc2.setLabel(0);

    // Test case 3: High risk profile
    UserProfile tc3(70);
    tc3.addHealthIndicator("HighBloodPressure", true);
    tc3.addHealthIndicator("ChestPain", true);
    tc3.addHealthIndicator("Dizziness", true);
    tc3.setLabel(1);

    // Test case 4: Young but with symptoms
    UserProfile tc4(25);
    tc4.addHealthIndicator("HighBloodPressure", true);
    tc4.addHealthIndicator("ChestPain", true);
    tc4.addHealthIndicator("Dizziness", true);
    tc4.setLabel(1);

    // Run predictions
    float risk1 = predictor.calculateRisk(tc1);
    assert(risk1 < 30.0f);

    float risk2 = predictor.calculateRisk(tc2);
    assert(risk2 >= 30.0f && risk2 < 60.0f);

    float risk3 = predictor.calculateRisk(tc3);
    assert(predictor.generateAlert(risk3) == "At Risk");

    float risk4 = predictor.calculateRisk(tc4);
    assert(risk4 >= 50.0f);
}