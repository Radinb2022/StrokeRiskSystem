#include "Tester.h"

void Tester::runTests() {
    StrokeRiskPredictor predictor;

    UserProfile tc1("TC1", 75);
    tc1.addHealthIndicator("ChestPain", true);
    tc1.addHealthIndicator("HighBloodPressure", true);
    float risk1 = predictor.calculateRisk(tc1);
    assert(risk1 >= 80.0);

    UserProfile tc2("TC2", 0);
    float risk2 = predictor.calculateRisk(tc2);
    assert(risk2 == 0.0);

    UserProfile tc3("TC3", 55);
    tc3.addHealthIndicator("ChestPain", true);
    tc3.addHealthIndicator("Dizziness", true);
    float risk3 = predictor.calculateRisk(tc3);
    assert(predictor.generateAlert(risk3) == "At Risk");

    UserProfile tc4("TC4", 25);
    float risk4 = predictor.calculateRisk(tc4);
    assert(risk4 == 0.0);

    std::cout << "\nAll test cases passed.\n";
}