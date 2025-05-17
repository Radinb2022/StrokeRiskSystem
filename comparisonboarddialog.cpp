#include "comparisonboarddialog.h"
#include "ui_comparisonboarddialog.h"
#include <QDebug>
#include <QTableWidgetItem>
#include <QMessageBox>

ComparisonBoardDialog::ComparisonBoardDialog(const UserProfile& userProfile, KaggleData& allData, Comparison& comparisonLogic, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ComparisonBoardDialog),
      m_userProfile(userProfile),
      m_allData(allData),
      m_comparisonLogic(comparisonLogic)
{
    ui->setupUi(this);
    setWindowTitle("Comparison Dashboard");
    setFixedSize(800, 600);

    connect(ui->closeButton, &QPushButton::clicked, this, &ComparisonBoardDialog::on_closeButton_clicked);

    // Initial check for a valid user profile to compare against
    if (m_userProfile.getAge() == 0 && m_userProfile.getGender().empty()) { // Simple check if profile is "empty"
        QMessageBox::warning(this, "No User Profile", "Please generate a prediction first to create your profile for comparison.");
        ui->noMatchLabel->setText("No user profile available for comparison. Please go to 'Generate Stroke Risk Prediction' first.");
        ui->yourProfileTable->setRowCount(0);
        ui->similarProfileTable->setRowCount(0);
        ui->yourRiskStatus->setText("Your Risk: N/A");
        ui->similarRiskStatus->setText("Their Risk: N/A");
        return;
    }

    // Determine user's current risk (assuming the model has been trained)
    // You might need a more robust way to get the *current* model.
    // Assuming MainWindow's m_riskPredictor can predict for the current user profile.
    std::string yourRisk = "Unknown";
    try {
        // Need access to the RandomForest model from StrokeRiskPredictor.
        // Assuming StrokeRiskPredictor can predict based on a UserProfile object
        // and you can get the model from the MainWindow or pass it down.
        // For simplicity, let's just make a dummy prediction if the userProfile is not tied to a real prediction yet.
        // In reality, this dialog should receive the *actual* predicted risk for the user.
        // Let's assume m_riskPredictor.predict(m_userProfile) works.
        // For now, let's fake it if no real prediction was done.
        // If this dialog is opened *after* a prediction, the UserProfile passed will be accurate.
        // For demonstration, let's just use "Not At Risk" or "At Risk" based on a dummy condition
        // or assume m_userProfile already has its 'risk_status' set after prediction.

        // THIS IS A PLACEHOLDER - YOU NEED THE ACTUAL RISK STATUS
        // For example, if the UserProfile saves the last predicted risk:
        // yourRisk = m_userProfile.getRiskStatus();
        // Or re-predict it:
        // yourRisk = m_riskPredictor.predict(m_userProfile);
        yourRisk = "Not At Risk"; // Placeholder if you haven't implemented getting the last prediction.

    } catch (const std::exception& e) {
        qDebug() << "Error predicting user risk for comparison:" << e.what();
        yourRisk = "Error";
    }

    // Find similar individual with opposite risk
    // This calls your backend comparison logic.
    // Assuming Comparison::findSimilarOppositeRisk takes current user profile and all data.
    // It should return a UserProfile object or similar.
    UserProfile similarOppositeProfile;
    std::string similarRisk = "Unknown";

    try {
        // Assuming your Comparison class has a method to find and return the profile.
        // The return type should be a UserProfile or a struct representing the found data.
        // You'll need to decide how your Comparison::findSimilarOppositeRisk works.
        // Example: similarOppositeProfile = m_comparisonLogic.findSimilarOppositeRisk(m_userProfile, m_allData);
        // And then get its risk: similarRisk = m_comparisonLogic.getRiskForProfile(similarOppositeProfile);

        // Dummy similar profile and risk for demonstration
        // In a real scenario, this comes from your backend.
        similarOppositeProfile = m_userProfile; // Start with user's profile
        // Modify some attributes to make it "opposite risk" and slightly different
        similarOppositeProfile.setHeartDisease(!m_userProfile.getHeartDisease());
        similarOppositeProfile.setAge(m_userProfile.getAge() + 5);
        similarRisk = (yourRisk == "At Risk") ? "Not At Risk" : "At Risk"; // Ensure opposite risk

        if (similarOppositeProfile.getGender().empty()) { // If the backend didn't find a match
            ui->noMatchLabel->setText("No sufficiently similar individual with opposite risk status was found in the dataset.");
            ui->yourProfileTable->setRowCount(0);
            ui->similarProfileTable->setRowCount(0);
            ui->yourRiskStatus->setText("Your Risk: " + QString::fromStdString(yourRisk));
            ui->similarRiskStatus->setText("Their Risk: N/A");
            return;
        }

        populateTables(m_userProfile, similarOppositeProfile, yourRisk, similarRisk);

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Comparison Error", QString("An error occurred during comparison: ") + e.what());
        ui->noMatchLabel->setText("An error occurred during comparison.");
        ui->yourProfileTable->setRowCount(0);
        ui->similarProfileTable->setRowCount(0);
        ui->yourRiskStatus->setText("Your Risk: " + QString::fromStdString(yourRisk));
        ui->similarRiskStatus->setText("Their Risk: Error");
    }
}

void ComparisonBoardDialog::populateTables(const UserProfile& yourProfile, const UserProfile& similarProfile,
                                           const std::string& yourRisk, const std::string& similarRisk) {
    // Attributes to display - map to your UserProfile getters
    std::vector<std::pair<std::string, std::function<std::string(const UserProfile&)>>> attributes = {
        {"Gender", [](const UserProfile& p){ return p.getGender(); }},
        {"Age", [](const UserProfile& p){ return std::to_string(p.getAge()); }},
        {"Hypertension", [](const UserProfile& p){ return p.getHypertension() ? "Yes" : "No"; }},
        {"Heart Disease", [](const UserProfile& p){ return p.getHeartDisease() ? "Yes" : "No"; }},
        {"Ever Married", [](const UserProfile& p){ return p.getEverMarried() ? "Yes" : "No"; }},
        {"Work Type", [](const UserProfile& p){ return p.getWorkType(); }},
        {"Residence Type", [](const UserProfile& p){ return p.getResidenceType(); }},
        {"Avg Glucose Level", [](const UserProfile& p){ return std::to_string(p.getAvgGlucoseLevel()); }},
        {"BMI", [](const UserProfile& p){ return std::to_string(p.getBmi()); }},
        {"Smoking Status", [](const UserProfile& p){ return p.getSmokingStatus(); }}
    };

    // Populate "Your Profile" table
    ui->yourProfileTable->setRowCount(attributes.size());
    for (int i = 0; i < attributes.size(); ++i) {
        ui->yourProfileTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(attributes[i].first)));
        ui->yourProfileTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(attributes[i].second(yourProfile))));
    }
    ui->yourProfileTable->resizeColumnsToContents();
    ui->yourProfileTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Populate "Similar Individual" table
    ui->similarProfileTable->setRowCount(attributes.size());
    for (int i = 0; i < attributes.size(); ++i) {
        ui->similarProfileTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(attributes[i].first)));
        ui->similarProfileTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(attributes[i].second(similarProfile))));
    }
    ui->similarProfileTable->resizeColumnsToContents();
    ui->similarProfileTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set risk statuses
    QString yourRiskHtml = "<span style='font-weight:bold; color:";
    yourRiskHtml += (yourRisk == "At Risk" ? "red" : "green") + ";'>" + QString::fromStdString(yourRisk) + "</span>";
    ui->yourRiskStatus->setText("Your Risk: " + yourRiskHtml);

    QString similarRiskHtml = "<span style='font-weight:bold; color:";
    similarRiskHtml += (similarRisk == "At Risk" ? "red" : "green") + ";'>" + QString::fromStdString(similarRisk) + "</span>";
    ui->similarRiskStatus->setText("Their Risk: " + similarRiskHtml);

    ui->noMatchLabel->clear(); // Clear the "no match" message if a match was found
}

void ComparisonBoardDialog::on_closeButton_clicked() {
    accept(); // Closes the dialog
}
