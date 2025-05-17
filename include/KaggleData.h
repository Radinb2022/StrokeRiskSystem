#ifndef KAGGLE_DATA_H
#define KAGGLE_DATA_H

#include "UserProfile.h"
#include <vector>
#include <string>

class KaggleData {
public:
    std::vector<UserProfile> dataset;
    void loadSampleData();
    bool loadFromCSV(const std::string& filename);
    void appendUser(const UserProfile& user);
    void saveToCSV(const std::string& filename);
};

#endif