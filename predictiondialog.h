#ifndef PREDICTIONDIALOG_H
#define PREDICTIONDIALOG_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H> // For grouping widgets and layouts
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H> // For age
#include <FL/Fl_Float_Input.H> // For numerical inputs like glucose, BMI
#include <FL/Fl_Choice.H> // For combo boxes
#include <string>

#include "src/StrokeRiskPredictor.h"
#include "src/UserProfile.h"
#include "src/DataProcessor.h"

class PredictionDialog : public Fl_Window {
public:
    // Constants for layout
    static constexpr int LABEL_WIDTH = 180;
    static constexpr int INPUT_WIDTH = 150;
    static constexpr int ROW_HEIGHT = 25;
    static constexpr int SPACING = 10;
    static constexpr int MARGIN = 20;

    PredictionDialog(StrokeRiskPredictor& predictor, DataProcessor& dataProcessor);
    ~PredictionDialog();

    UserProfile getPredictedUserProfile() const;
    bool getPredictionSuccess() const { return m_predictionSuccess; }

private:
    Fl_Box* titleLabel;
    Fl_Group* inputGroup; // Group for form layout
    Fl_Button* predictButton;
    Fl_Box* resultLabel;

    // Input widgets
    Fl_Choice* genderChoice;
    Fl_Int_Input* ageInput;
    Fl_Choice* hypertensionChoice;
    Fl_Choice* heartDiseaseChoice;
    Fl_Choice* everMarriedChoice;
    Fl_Choice* workTypeChoice;
    Fl_Choice* residenceTypeChoice;
    Fl_Float_Input* avgGlucoseInput;
    Fl_Float_Input* bmiInput;
    Fl_Choice* smokingStatusChoice;

    StrokeRiskPredictor& m_riskPredictor;
    DataProcessor& m_dataProcessor;
    UserProfile m_currentUserProfile;
    bool m_predictionSuccess;

    // Callbacks
    static void predict_callback(Fl_Widget* w, void* data);

    // Helper to collect user input
    UserProfile getUserProfileFromInputs() const;
    void setupFormLayout();
};

#endif // PREDICTIONDIALOG_H
