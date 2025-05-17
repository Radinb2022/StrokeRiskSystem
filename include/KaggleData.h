#ifndef KAGGLE_DATA_H
#define KAGGLE_DATA_H

#include "UserProfile.h"
#include <vector>

class KaggleData {
public:
    std::vector<UserProfile> dataset;
    void loadSampleData();
    void appendUser(const UserProfile& user);
    void saveToCSV(const std::string& filename);
};

#endif