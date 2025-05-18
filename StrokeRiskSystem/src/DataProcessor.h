#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include "UserProfile.h" // Now depends on the updated UserProfile

class DataProcessor {
public:
    DataProcessor();

    bool loadCSV(const std::string& filename);

    const std::vector<std::vector<double>>& getProcessedFeatures() const;
    const std::vector<std::string>& getLabels() const;
    const std::vector<UserProfile>& getRawData() const;

    // Preprocessing for a single UserProfile (for prediction)
    std::vector<double> preprocessUserProfile(const UserProfile& userProfile) const;

    // Min/Max for Age normalization
    std::pair<double, double> getAgeMinMax() const { return ageMinMax; }

private:
    std::vector<std::vector<double>> processedFeatures;
    std::vector<std::string> labels;
    std::vector<UserProfile> rawData;

    std::vector<std::string> header;
    std::map<std::string, int> featureColumnMap;

    std::pair<double, double> ageMinMax; // Min/Max for 'age' feature

    void parseCSVLine(const std::string& line, UserProfile& userProfile, std::string& label);
    double normalize(double value, double min_val, double max_val) const;
};

#endif // DATA_PROCESSOR_H