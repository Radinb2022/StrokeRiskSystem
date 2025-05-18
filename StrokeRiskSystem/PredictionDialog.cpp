#include "PredictionDialog.h"
#include <FL/fl_ask.H>
#include <iostream>
#include <string>
#include <sstream>

PredictionDialog::PredictionDialog(StrokeRiskPredictor& predictor, DataProcessor& dataProcessor)
    : Fl_Window(700, 550, "Stroke Risk Prediction Questionnaire"), // Adjusted window height
      m_riskPredictor(predictor),
      m_dataProcessor(dataProcessor),
      m_predictionSuccess(false)
{
    size_range(700, 550, 700, 550); // Fixed size

    begin();

    titleLabel = new Fl_Box(MARGIN, MARGIN, w() - 2 * MARGIN, 30, "Please fill in your health indicators:");
    titleLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    titleLabel->labelsize(16);
    titleLabel->labelfont(FL_BOLD);

    setupFormLayout();

    predictButton = new Fl_Button((w() - INPUT_WIDTH) / 2, h() - MARGIN - 40 - SPACING - 30, INPUT_WIDTH, 40, "Predict Stroke Risk");
    predictButton->callback(predict_callback, this);

    resultLabel = new Fl_Box(MARGIN, h() - MARGIN - 30, w() - 2 * MARGIN, 30, "Prediction:");
    resultLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    resultLabel->labelsize(14);
    resultLabel->labelfont(FL_BOLD);

    end();

    ageInput->value("30");
}

PredictionDialog::~PredictionDialog() {
}

void PredictionDialog::setupFormLayout() {
    int current_y = titleLabel->y() + titleLabel->h() + SPACING;
    int col1_x = MARGIN;
    int col2_x = col1_x + LABEL_WIDTH + SPACING;

    inputGroup = new Fl_Group(MARGIN, current_y, w() - 2 * MARGIN, h() - current_y - MARGIN - 40 - SPACING - 30);
    inputGroup->begin();

    // Age
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Age:");
    ageInput = new Fl_Int_Input(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    ageInput->value("30");
    current_y += ROW_HEIGHT + SPACING;

    // High Blood Pressure
    highBloodPressureCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "High Blood Pressure");
    highBloodPressureCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Chest Pain
    chestPainCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Chest Pain");
    chestPainCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Dizziness
    dizzinessCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Dizziness");
    dizzinessCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Fatigue & Weakness
    fatigueWeaknessCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Fatigue & Weakness");
    fatigueWeaknessCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Swelling (Edema)
    swellingEdemaCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Swelling (Edema)");
    swellingEdemaCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Pain in Neck/Jaw/Shoulder/Back
    painNeckJawShoulderBackCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Pain in Neck/Jaw/Shoulder/Back");
    painNeckJawShoulderBackCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Excessive Sweating
    excessiveSweatingCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Excessive Sweating");
    excessiveSweatingCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Chest Discomfort (Activity)
    chestDiscomfortActivityCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Chest Discomfort (Activity)");
    chestDiscomfortActivityCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Snoring/Sleep Apnea
    snoringSleepApneaCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Snoring/Sleep Apnea");
    snoringSleepApneaCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Anxiety/Feeling of Doom
    anxietyFeelingDoomCheck = new Fl_Check_Button(col1_x, current_y, LABEL_WIDTH + CHECKBOX_WIDTH, ROW_HEIGHT, "Anxiety/Feeling of Doom");
    anxietyFeelingDoomCheck->value(0);
    current_y += ROW_HEIGHT + SPACING;


    inputGroup->end();
}

UserProfile PredictionDialog::getUserProfileFromInputs() const {
    UserProfile userProfile;

    userProfile.setAge(atoi(ageInput->value()));
    userProfile.setHighBloodPressure(highBloodPressureCheck->value());
    userProfile.setChestPain(chestPainCheck->value());
    userProfile.setDizziness(dizzinessCheck->value());
    userProfile.setFatigueWeakness(fatigueWeaknessCheck->value());
    userProfile.setSwellingEdema(swellingEdemaCheck->value());
    userProfile.setPainNeckJawShoulderBack(painNeckJawShoulderBackCheck->value());
    userProfile.setExcessiveSweating(excessiveSweatingCheck->value());
    userProfile.setChestDiscomfortActivity(chestDiscomfortActivityCheck->value());
    userProfile.setSnoringSleepApnea(snoringSleepApneaCheck->value());
    userProfile.setAnxietyFeelingDoom(anxietyFeelingDoomCheck->value());

    return userProfile;
}

UserProfile PredictionDialog::getPredictedUserProfile() const {
    return m_currentUserProfile;
}

void PredictionDialog::predict_callback(Fl_Widget* w, void* data) {
    PredictionDialog* self = static_cast<PredictionDialog*>(data);
    UserProfile userProfile = self->getUserProfileFromInputs();

    bool isValid = true;
    if (userProfile.getAge() <= 0 || strlen(self->ageInput->value()) == 0) {
        fl_alert("Please enter a valid positive number for Age.");
        isValid = false;
    }
    if (!isValid) {
        return;
    }

    try {
        std::string prediction_str = self->m_riskPredictor.predict(userProfile, self->m_dataProcessor);

        std::string display_text;
        if (prediction_str == "1") {
            display_text = "Prediction: At Risk";
            self->m_currentUserProfile.setStrokePrediction(1);
        } else if (prediction_str == "0") {
            display_text = "Prediction: Not At Risk";
            self->m_currentUserProfile.setStrokePrediction(0);
        } else {
            display_text = "Prediction: " + prediction_str;
            self->m_currentUserProfile.setStrokePrediction(-1);
        }
        self->resultLabel->label(display_text.c_str());

        self->m_currentUserProfile = userProfile;
        self->m_currentUserProfile.setStrokePrediction(prediction_str == "1" ? 1 : 0);
        self->m_predictionSuccess = true;

    } catch (const std::exception& e) {
        fl_alert("Prediction Error:\n%s", e.what());
        self->resultLabel->label("Prediction: Error");
        self->m_predictionSuccess = false;
    }
    Fl::check();
}