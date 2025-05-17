#include "mainwindow.h"
#include "ui_mainwindow.h" // Essential for using UI elements from mainwindow.ui
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog> // For file selection in training

// Include your actual backend C++ headers here
#include "src/RandomForest.h"
#include "src/StrokeRiskPredictor.h"
#include "src/UserProfile.h"
#include "src/KaggleData.h" // Assuming a class to manage Kaggle data loading

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) // Initialize the UI object from mainwindow.ui
{
    ui->setupUi(this); // This loads the UI defined in mainwindow.ui

    // Set fixed size for the main window
    setFixedSize(600, 400);
    setWindowTitle("Stroke Risk Prediction System");

    // Connect signals to slots
    connect(ui->predictRiskButton, &QPushButton::clicked, this, &MainWindow::on_predictRiskButton_clicked);
    connect(ui->trainModelButton, &QPushButton::clicked, this, &MainWindow::on_trainModelButton_clicked);
    connect(ui->compareDashboardButton, &QPushButton::clicked, this, &MainWindow::on_compareDashboardButton_clicked);

    // Initialize backend components here
    m_kaggleData = new KaggleData();
    m_randomForestModel = new RandomForest();
    m_riskPredictor = new StrokeRiskPredictor(m_randomForestModel);
    m_userProfile = new UserProfile();
    //
}

MainWindow::~MainWindow() {
    delete ui; // Clean up the UI object
    // Clean up backend objects if dynamically allocated and without parents
    delete m_kaggleData;
    delete m_randomForestModel;
    delete m_riskPredictor;
    delete m_userProfile;
}

void MainWindow::on_predictRiskButton_clicked() {
    // TODO: Open the "Stroke Risk Prediction Questionnaire" dialog here.
    // For now, a placeholder message.
    QMessageBox::information(this, "Prediction", "Opening Stroke Risk Prediction Questionnaire...");
}

void MainWindow::on_trainModelButton_clicked() {
    // TODO: Open the "Model Training Interface" dialog here.
    // For now, a placeholder message.
    QMessageBox::information(this, "Training", "Opening Model Training Interface...");

    QString filePath = QFileDialog::getOpenFileName(this, "Upload New Dataset", "", "CSV Files (*.csv)");
    if (!filePath.isEmpty()) {
        QMessageBox::information(this, "File Selected", "Selected file: " + filePath);
       // TODO: Pass filePath to your backend's training function
        m_kaggleData->loadAndTrainNewData(filePath);
    // }
}

void MainWindow::on_compareDashboardButton_clicked() {
    // TODO: Open the "Comparison Dashboard" dialog here.
    // For now, a placeholder message.
    QMessageBox::information(this, "Comparison", "Opening Comparison Dashboard...");
}
