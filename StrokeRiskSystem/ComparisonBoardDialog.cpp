#include "ComparisonBoardDialog.h"
#include <FL/fl_ask.H>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

ComparisonBoardDialog::ComparisonBoardDialog(const UserProfile& userProfile, DataProcessor& allData, RandomForest& model, ComparisonEngine& comparisonLogic)
    : Fl_Window(800, 600, "Comparison Dashboard"),
      m_userProfile(userProfile),
      m_allDataProcessor(allData),
      m_randomForestModel(model),
      m_comparisonEngineLogic(comparisonLogic)
{
    size_range(800, 600, 800, 600);

    begin();

    titleLabel = new Fl_Box(20, 20, w() - 40, 30, "Your Profile vs. Similar Individual with Opposite Risk");
    titleLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    titleLabel->labelsize(16);
    titleLabel->labelfont(FL_BOLD);

    yourProfileTitle = new Fl_Box(20, 70, (w() / 2) - 40, 25, "Your Profile");
    yourProfileTitle->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    yourProfileTitle->labelsize(14);
    yourProfileTitle->labelfont(FL_BOLD);

    yourProfileBrowser = new Fl_Browser(20, 100, (w() / 2) - 40, 350);
    yourProfileBrowser->type(FL_MULTI_BROWSER);
    yourProfileBrowser->has_scrollbar(FL_VERTICAL);

    yourRiskStatus = new Fl_Box(20, 460, (w() / 2) - 40, 30, "Your Risk:");
    yourRiskStatus->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    yourRiskStatus->labelsize(14);
    yourRiskStatus->labelfont(FL_BOLD);

    similarProfileTitle = new Fl_Box(w() / 2 + 20, 70, (w() / 2) - 40, 25, "Similar Individual");
    similarProfileTitle->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    similarProfileTitle->labelsize(14);
    similarProfileTitle->labelfont(FL_BOLD);

    similarProfileBrowser = new Fl_Browser(w() / 2 + 20, 100, (w() / 2) - 40, 350);
    similarProfileBrowser->type(FL_MULTI_BROWSER);
    similarProfileBrowser->has_scrollbar(FL_VERTICAL);

    similarRiskStatus = new Fl_Box(w() / 2 + 20, 460, (w() / 2) - 40, 30, "Their Risk:");
    similarRiskStatus->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    similarRiskStatus->labelsize(14);
    similarRiskStatus->labelfont(FL_BOLD);

    noMatchLabel = new Fl_Box(20, 500, w() - 40, 25, "");
    noMatchLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    noMatchLabel->labelsize(12);
    noMatchLabel->labelfont(FL_ITALIC);

    closeButton = new Fl_Button((w() - 100) / 2, h() - 50, 100, 30, "Close");
    closeButton->callback(close_callback, this);

    end();

    if (m_userProfile.getAge() == 0 && m_userProfile.getHighBloodPressure() == false && m_userProfile.getChestPain() == false) {
        noMatchLabel->label("No user profile available for comparison. Please go to 'Generate Stroke Risk Prediction' first.");
        yourProfileBrowser->clear();
        similarProfileBrowser->clear();
        yourRiskStatus->label("Your Risk: N/A");
        similarRiskStatus->label("Their Risk: N/A");
        return;
    }

    std::string yourRisk = (m_userProfile.getStrokePrediction() == 1) ? "At Risk" : "Not At Risk";

    UserProfile similarOppositeProfile;
    std::string similarRisk = "N/A";

    try {
        similarOppositeProfile = m_comparisonEngineLogic.findSimilarOppositeRisk(m_userProfile, m_allDataProcessor, m_randomForestModel);

        if (similarOppositeProfile.getAge() != 0 || similarOppositeProfile.getHighBloodPressure() == true || similarOppositeProfile.getChestPain() == true) { // A more robust check for non-default profile
            similarRisk = (similarOppositeProfile.getStrokePrediction() == 1) ? "At Risk" : "Not At Risk";
            populateBrowsers(m_userProfile, similarOppositeProfile, yourRisk, similarRisk);
            noMatchLabel->label("");
        } else {
            noMatchLabel->label("No sufficiently similar individual with opposite risk status was found in the dataset.");
            yourProfileBrowser->clear();
            similarProfileBrowser->clear();
            yourRiskStatus->label( ("Your Risk: " + yourRisk).c_str());
            similarRiskStatus->label("Their Risk: N/A");
        }

    } catch (const std::exception& e) {
        fl_alert("Comparison Error", "An error occurred during comparison:\n%s", e.what());
        noMatchLabel->label("An error occurred during comparison.");
        yourProfileBrowser->clear();
        similarProfileBrowser->clear();
        yourRiskStatus->label(("Your Risk: " + yourRisk).c_str());
        similarRiskStatus->label("Their Risk: Error");
    }
}

void ComparisonBoardDialog::populateBrowsers(const UserProfile& yourProfile, const UserProfile& similarProfile,
                                             const std::string& yourRisk, const std::string& similarRisk) {
    yourProfileBrowser->clear();
    similarProfileBrowser->clear();

    auto format_bool = [](bool val) { return val ? "Yes" : "No"; };

    std::vector<std::pair<std::string, std::function<std::string(const UserProfile&)>>> attributes = {
        {"Age", [](const UserProfile& p){ return std::to_string(p.getAge()); }},
        {"High Blood Pressure", [=](const UserProfile& p){ return format_bool(p.getHighBloodPressure()); }},
        {"Chest Pain", [=](const UserProfile& p){ return format_bool(p.getChestPain()); }},
        {"Dizziness", [=](const UserProfile& p){ return format_bool(p.getDizziness()); }},
        {"Fatigue & Weakness", [=](const UserProfile& p){ return format_bool(p.getFatigueWeakness()); }},
        {"Swelling (Edema)", [=](const UserProfile& p){ return format_bool(p.getSwellingEdema()); }},
        {"Pain (Neck/Jaw/Shoulder/Back)", [=](const UserProfile& p){ return format_bool(p.getPainNeckJawShoulderBack()); }},
        {"Excessive Sweating", [=](const UserProfile& p){ return format_bool(p.getExcessiveSweating()); }},
        {"Chest Discomfort (Activity)", [=](const UserProfile& p){ return format_bool(p.getChestDiscomfortActivity()); }},
        {"Snoring/Sleep Apnea", [=](const UserProfile& p){ return format_bool(p.getSnoringSleepApnea()); }},
        {"Anxiety/Feeling of Doom", [=](const UserProfile& p){ return format_bool(p.getAnxietyFeelingDoom()); }}
    };

    for (const auto& attr : attributes) {
        std::string your_val = attr.second(yourProfile);
        std::string similar_val = attr.second(similarProfile);

        yourProfileBrowser->add((attr.first + ": " + your_val).c_str());
        similarProfileBrowser->add((attr.first + ": " + similar_val).c_str());
    }

    yourRiskStatus->label(("Your Risk: " + yourRisk).c_str());
    similarRiskStatus->label(("Their Risk: " + similarRisk).c_str());
}

void ComparisonBoardDialog::close_callback(Fl_Widget* w, void* data) {
    Fl_Window* dialog = static_cast<Fl_Window*>(data);
    dialog->hide();
}