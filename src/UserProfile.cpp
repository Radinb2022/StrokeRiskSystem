#include "UserProfile.h"

UserProfile::UserProfile(std::string id, int a) : patientId(id), age(a) {}

UserProfile::UserProfile() : patientId("None"), age(0) {}

void UserProfile::addHealthIndicator(const std::string& indicator, bool value) {
    healthIndicators[indicator] = value;
}