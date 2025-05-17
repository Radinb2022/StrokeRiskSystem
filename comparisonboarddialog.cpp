#ifndef COMPARISONBOARDDIALOG_H
#define COMPARISONBOARDDIALOG_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Browser.H> // For displaying lists/tables of data
#include <string>
#include <vector>
#include <functional> // For std::function

#include "src/UserProfile.h"
#include "src/DataProcessor.h"
#include "src/RandomForest.h"
#include "src/ComparisonEngine.h"

class ComparisonBoardDialog : public Fl_Window {
public:
    ComparisonBoardDialog(const UserProfile& userProfile, DataProcessor& allData, RandomForest& model, ComparisonEngine& comparisonLogic);
    ~ComparisonBoardDialog();

private:
    Fl_Box* titleLabel;
    Fl_Box* yourProfileTitle;
    Fl_Browser* yourProfileBrowser; // Using Fl_Browser for simplicity as a table
    Fl_Box* yourRiskStatus;

    Fl_Box* similarProfileTitle;
    Fl_Browser* similarProfileBrowser; // Using Fl_Browser for simplicity as a table
    Fl_Box* similarRiskStatus;

    Fl_Box* noMatchLabel;
    Fl_Button* closeButton;

    const UserProfile& m_userProfile;
    DataProcessor& m_allDataProcessor;
    RandomForest& m_randomForestModel;
    ComparisonEngine& m_comparisonEngineLogic;

    // Callbacks
    static void close_callback(Fl_Widget* w, void* data);

    void populateBrowsers(const UserProfile& yourProfile, const UserProfile& similarProfile,
                          const std::string& yourRisk, const std::string& similarRisk);
};

#endif // COMPARISONBOARDDIALOG_H
