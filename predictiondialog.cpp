#include "PredictionDialog.h"
#include <FL/fl_ask.H> // For fl_alert
#include <iostream>
#include <string>
#include <sstream> // For string conversion

PredictionDialog::PredictionDialog(StrokeRiskPredictor& predictor, DataProcessor& dataProcessor)
    : Fl_Window(700, 650, "Stroke Risk Prediction Questionnaire"),
      m_riskPredictor(predictor),
      m_dataProcessor(dataProcessor),
      m_predictionSuccess(false)
{
    size_range(700, 650, 700, 650); // Fixed size

    begin(); // Start adding widgets

    titleLabel = new Fl_Box(MARGIN, MARGIN, w() - 2 * MARGIN, 30, "Please fill in your health indicators:");
    titleLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    titleLabel->labelsize(16);
    titleLabel->labelfont(FL_BOLD);

    // Setup input widgets
    setupFormLayout();

    // Predict Button
    predictButton = new Fl_Button((w() - INPUT_WIDTH) / 2, h() - MARGIN - 40 - SPACING - 30, INPUT_WIDTH, 40, "Predict Stroke Risk");
    predictButton->callback(predict_callback, this);

    // Result Label
    resultLabel = new Fl_Box(MARGIN, h() - MARGIN - 30, w() - 2 * MARGIN, 30, "Prediction:");
    resultLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    resultLabel->labelsize(14);
    resultLabel->labelfont(FL_BOLD);

    end(); // Stop adding widgets

    // Set default values for numerical inputs
    ageInput->value("30");
    avgGlucoseInput->value("90.0");
    bmiInput->value("25.0");
}

PredictionDialog::~PredictionDialog() {
    // FLTK widgets are usually managed by their parent Fl_Group/Fl_Window.
}

void PredictionDialog::setupFormLayout() {
    int current_y = titleLabel->y() + titleLabel->h() + SPACING;
    int col1_x = MARGIN;
    int col2_x = col1_x + LABEL_WIDTH + SPACING;

    // Use a group to contain the form widgets for scrollability if needed (basic version now)
    inputGroup = new Fl_Group(MARGIN, current_y, w() - 2 * MARGIN, h() - current_y - MARGIN - 40 - SPACING - 30); // Leave space for predict button and result
    inputGroup->begin();

    // Gender
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Gender:");
    genderChoice = new Fl_Choice(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    genderChoice->add("Male");
    genderChoice->add("Female");
    genderChoice->add("Other");
    genderChoice->value(0); // Default to Male
    current_y += ROW_HEIGHT + SPACING;

    // Age
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Age:");
    ageInput = new Fl_Int_Input(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    ageInput->value("30");
    current_y += ROW_HEIGHT + SPACING;

    // Hypertension
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Hypertension (High Blood Pressure):");
    hypertensionChoice = new Fl_Choice(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    hypertensionChoice->add("No");
    hypertensionChoice->add("Yes");
    hypertensionChoice->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Heart Disease
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Heart Disease:");
    heartDiseaseChoice = new Fl_Choice(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    heartDiseaseChoice->add("No");
    heartDiseaseChoice->add("Yes");
    heartDiseaseChoice->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Ever Married
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Ever Married:");
    everMarriedChoice = new Fl_Choice(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    everMarriedChoice->add("No");
    everMarriedChoice->add("Yes");
    everMarriedChoice->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Work Type
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Work Type:");
    workTypeChoice = new Fl_Choice(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    workTypeChoice->add("Private");
    workTypeChoice->add("Self-employed");
    workTypeChoice->add("Govt_job");
    workTypeChoice->add("children");
    workTypeChoice->add("Never_worked");
    workTypeChoice->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Residence Type
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Residence Type:");
    residenceTypeChoice = new Fl_Choice(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    residenceTypeChoice->add("Urban");
    residenceTypeChoice->add("Rural");
    residenceTypeChoice->value(0);
    current_y += ROW_HEIGHT + SPACING;

    // Avg Glucose Level
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Avg Glucose Level:");
    avgGlucoseInput = new Fl_Float_Input(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    avgGlucoseInput->value("90.0");
    current_y += ROW_HEIGHT + SPACING;

    // BMI
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "BMI:");
    bmiInput = new Fl_Float_Input(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    bmiInput->value("25.0");
    current_y += ROW_HEIGHT + SPACING;

    // Smoking Status
    new Fl_Box(col1_x, current_y, LABEL_WIDTH, ROW_HEIGHT, "Smoking Status:");
    smokingStatusChoice = new Fl_Choice(col2_x, current_y, INPUT_WIDTH, ROW_HEIGHT);
    smokingStatusChoice->add("formerly smoked");
    smokingStatusChoice->add("never smoked");
    smokingStatusChoice->add("smokes");
    smokingStatusChoice->add("Unknown"); // Corresponds to the Kaggle dataset
    smokingStatusChoice->value(0);
    current_y += ROW_HEIGHT + SPACING;

    inputGroup->end();
    // If the content is taller than the group, you'd make the window scrollable,
    // or manually add a scrollbar. For now, assuming fixed window size fits content.
}


UserProfile PredictionDialog::getUserProfileFromInputs() const {
    UserProfile userProfile;

    userProfile.setGender(genderChoice->text());
    userProfile.setAge(atoi(ageInput->value())); // Convert char* to int
    userProfile.setHypertension(strcmp(hypertensionChoice->text(), "Yes") == 0);
    userProfile.setHeartDisease(strcmp(heartDiseaseChoice->text(), "Yes") == 0);
    userProfile.setEverMarried(strcmp(everMarriedChoice->text(), "Yes") == 0);
    userProfile.setWorkType(workTypeChoice->text());
    userProfile.setResidenceType(residenceTypeChoice->text());

    userProfile.setAvgGlucoseLevel(atof(avgGlucoseInput->value())); // Convert char* to double
    userProfile.setBmi(atof(bmiInput->value()));

    userProfile.setSmokingStatus(smokingStatusChoice->text());

    return userProfile;
}

UserProfile PredictionDialog::getPredictedUserProfile() const {
    return m_currentUserProfile;
}

void PredictionDialog::predict_callback(Fl_Widget* w, void* data) {
    PredictionDialog* self = static_cast<PredictionDialog*>(data);
    UserProfile userProfile = self->getUserProfileFromInputs();

    // Basic input validation
    bool isValid = true;
    if (strlen(self->avgGlucoseInput->value()) == 0 || atof(self->avgGlucoseInput->value()) <= 0) {
        fl_alert("Please enter a valid positive number for Average Glucose Level.");
        isValid = false;
    }
    if (strlen(self->bmiInput->value()) == 0 || atof(self->bmiInput->value()) <= 0) {
        fl_alert("Please enter a valid positive number for BMI.");
        isValid = false;
    }
    if (!isValid) {
        return;
    }

    try {
        std::string prediction_str = self->m_riskPredictor.predict(userProfile, self->m_dataProcessor);

        std::string display_text;
        if (prediction_str == "1") { // Assuming "1" means At Risk from your model
            display_text = "Prediction: At Risk";
            self->m_currentUserProfile.setStrokePrediction(1);
        } else if (prediction_str == "0") { // Assuming "0" means Not At Risk
            display_text = "Prediction: Not At Risk";
            self->m_currentUserProfile.setStrokePrediction(0);
        } else {
            display_text = "Prediction: " + prediction_str; // Raw output if not 0/1
            self->m_currentUserProfile.setStrokePrediction(-1); // Unknown/Error
        }
        self->resultLabel->label(display_text.c_str());

        // Store the current user profile including its prediction for later use (e.g., comparison)
        self->m_currentUserProfile = userProfile;
        self->m_currentUserProfile.setStrokePrediction(prediction_str == "1" ? 1 : 0);
        self->m_predictionSuccess = true;

    } catch (const std::exception& e) {
        fl_alert("Prediction Error:\n%s", e.what());
        self->resultLabel->label("Prediction: Error");
        self->m_predictionSuccess = false;
    }
    Fl::check(); // Update UI to show result
}
