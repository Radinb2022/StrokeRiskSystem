#ifndef PREDICTIONDIALOG_H
#define PREDICTIONDIALOG_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Check_Button.H> // For binary attributes
#include <string>

#include "src/StrokeRiskPredictor.h"
#include "src/UserProfile.h"
#include "src/DataProcessor.h"

class PredictionDialog : public Fl_Window {
public:
    static constexpr int LABEL_WIDTH = 250;
    static constexpr int INPUT_WIDTH = 100; // For age
    static constexpr int CHECKBOX_WIDTH = 50; // For checkboxes
    static constexpr int ROW_HEIGHT = 25;
    static constexpr int SPACING = 10;
    static constexpr int MARGIN = 20;

    PredictionDialog(StrokeRiskPredictor& predictor, DataProcessor& dataProcessor);
    ~PredictionDialog();

    UserProfile getPredictedUserProfile() const;
    bool getPredictionSuccess() const { return m_predictionSuccess; }

private:
    Fl_Box* titleLabel;
    Fl_Group* inputGroup;
    Fl_Button* predictButton;
    Fl_Box* resultLabel;

    // Input widgets
    Fl_Int_Input* ageInput;
    Fl_Check_Button* highBloodPressureCheck;
    Fl_Check_Button* chestPainCheck;
    Fl_Check_Button* dizzinessCheck;
    Fl_Check_Button* fatigueWeaknessCheck;
    Fl_Check_Button* swellingEdemaCheck;
    Fl_Check_Button* painNeckJawShoulderBackCheck;
    Fl_Check_Button* excessiveSweatingCheck;
    Fl_Check_Button* chestDiscomfortActivityCheck;
    Fl_Check_Button* snoringSleepApneaCheck;
    Fl_Check_Button* anxietyFeelingDoomCheck;

    StrokeRiskPredictor& m_riskPredictor;
    DataProcessor& m_dataProcessor;
    UserProfile m_currentUserProfile;
    bool m_predictionSuccess;

    static void predict_callback(Fl_Widget* w, void* data);

    UserProfile getUserProfileFromInputs() const;
    void setupFormLayout();
};

#endif // PREDICTIONDIALOG_H