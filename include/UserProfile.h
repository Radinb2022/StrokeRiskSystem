#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include <string>
#include <map>

class UserProfile {
public:
    std::string patientId;
    int age;
    std::map<std::string, bool> healthIndicators;

    UserProfile(std::string id, int a);
    UserProfile();

    void addHealthIndicator(const std::string& indicator, bool value);
};

#endif