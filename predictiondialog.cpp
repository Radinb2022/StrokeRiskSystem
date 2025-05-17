#include "predictiondialog.h"
#include "ui_predictiondialog.h"
#include <QMessageBox>
#include <QDebug>
#include "src/UserProfile.h" // Ensure UserProfile definition is available
#include "src/QuestionnaireResponses.h" // For capturing questionnaire data

PredictionDialog::PredictionDialog(StrokeRiskPredictor& predictor, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::PredictionDialog),
      m_riskPredictor(predictor) // Initialize the reference
{
    ui->setupUi(this);
    setWindowTitle("Stroke Risk Prediction Questionnaire");
    setFixedSize(700, 650); // Fixed size

    connect(ui->predictButton, &QPushButton::clicked, this, &PredictionDialog::on_predictButton_clicked);

    ui->resultLabel->setText("Prediction:"); // Clear previous results
}

PredictionDialog::~PredictionDialog() {
    delete ui;
}

UserProfile PredictionDialog::getUserProfileFromInputs() const {
    UserProfile userProfile;

    // Assuming UserProfile has setters for these attributes
    // And that your backend uses string or integer representations consistent with these inputs

    userProfile.setGender(ui->genderComboBox->currentText().toStdString()); // "Male", "Female", "Other"
    userProfile.setAge(ui->ageSpinBox->value());
    userProfile.setHypertension(ui->hypertensionComboBox->currentText() == "Yes"); // 0 or 1
    userProfile.setHeartDisease(ui->heartDiseaseComboBox->currentText() == "Yes"); // 0 or 1
    userProfile.setEverMarried(ui->everMarriedComboBox->currentText() == "Yes"); // "Yes" or "No"
    userProfile.setWorkType(ui->workTypeComboBox->currentText().toStdString());
    userProfile.setResidenceType(ui->residenceTypeComboBox->currentText().toStdString());

    bool ok;
    double avgGlucose = ui->avgGlucoseLineEdit->text().toDouble(&ok);
    if (ok) userProfile.setAvgGlucoseLevel(avgGlucose);
    else QMessageBox::warning(this, "Input Error", "Please enter a valid number for Average Glucose Level.");

    double bmi = ui->bmiLineEdit->text().toDouble(&ok);
    if (ok) userProfile.setBmi(bmi);
    else QMessageBox::warning(this, "Input Error", "Please enter a valid number for BMI.");

    userProfile.setSmokingStatus(ui->smokingStatusComboBox->currentText().toStdString());

    // You might need to map these to numerical values if your backend ML model expects them
    // Example: gender ("Male": 1, "Female": 0), ever_married ("Yes": 1, "No": 0)
    // Your UserProfile class or a data preprocessor in your backend should handle this.

    return userProfile;
}

void PredictionDialog::on_predictButton_clicked() {
    UserProfile userProfile = getUserProfileFromInputs();

    // Check if input conversion was successful enough
    if (ui->avgGlucoseLineEdit->text().isEmpty() || ui->bmiLineEdit->text().isEmpty() ||
        ui->avgGlucoseLineEdit->text().toDouble() <= 0 || ui->bmiLineEdit->text().toDouble() <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter valid numerical values for Average Glucose Level and BMI.");
        return;
    }

    // Call the backend prediction logic
    // This assumes StrokeRiskPredictor takes a UserProfile and returns a string prediction.
    try {
        std::string prediction = m_riskPredictor.predict(userProfile); // Assuming predict takes UserProfile

        QString resultText = "Prediction: <span style='font-weight:bold; color:";
        if (prediction == "At Risk") {
            resultText += "red;'>At Risk</span>";
        } else if (prediction == "Not At Risk") {
            resultText += "green;'>Not At Risk</span>";
        } else {
            resultText += "black;'>" + QString::fromStdString(prediction) + "</span>";
        }
        ui->resultLabel->setText(resultText);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Prediction Error", QString("An error occurred during prediction: ") + e.what());
        ui->resultLabel->setText("Prediction: Error");
    }
}
