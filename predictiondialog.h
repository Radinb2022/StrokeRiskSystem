#ifndef PREDICTIONDIALOG_H
#define PREDICTIONDIALOG_H

#include <QDialog>
#include "src/StrokeRiskPredictor.h" // For performing prediction
#include "src/UserProfile.h" // To capture user input

QT_BEGIN_NAMESPACE
namespace Ui { class PredictionDialog; }
QT_END_NAMESPACE

class PredictionDialog : public QDialog {
    Q_OBJECT

public:
    explicit PredictionDialog(StrokeRiskPredictor& predictor, QWidget *parent = nullptr);
    ~PredictionDialog();

private slots:
    void on_predictButton_clicked();

private:
    Ui::PredictionDialog *ui;
    StrokeRiskPredictor& m_riskPredictor; // Reference to the main window's predictor

    // Helper to collect user input into a UserProfile
    UserProfile getUserProfileFromInputs() const;
};

#endif // PREDICTIONDIALOG_H
