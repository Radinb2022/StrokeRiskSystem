#include "trainingdialog.h"
#include "ui_trainingdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

TrainingDialog::TrainingDialog(KaggleData& data, RandomForest& model, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TrainingDialog),
      m_kaggleData(data),
      m_randomForestModel(model)
{
    ui->setupUi(this);
    setWindowTitle("Train Model with New Data");
    setFixedSize(500, 300);

    connect(ui->browseButton, &QPushButton::clicked, this, &TrainingDialog::on_browseButton_clicked);
    connect(ui->trainButton, &QPushButton::clicked, this, &TrainingDialog::on_trainButton_clicked);

    ui->progressBar->setValue(0);
    ui->statusLabel->setText("Status: Ready");
    ui->trainButton->setEnabled(false); // Disable until a file is selected
}

TrainingDialog::~TrainingDialog() {
    delete ui;
}

void TrainingDialog::on_browseButton_clicked() {
    m_selectedFilePath = QFileDialog::getOpenFileName(this, "Upload New Dataset", "", "CSV Files (*.csv)");
    if (!m_selectedFilePath.isEmpty()) {
        ui->filePathLineEdit->setText(m_selectedFilePath);
        ui->trainButton->setEnabled(true);
        ui->statusLabel->setText("Status: File selected. Ready to train.");
    } else {
        ui->filePathLineEdit->clear();
        ui->trainButton->setEnabled(false);
        ui->statusLabel->setText("Status: No file selected.");
    }
}

void TrainingDialog::on_trainButton_clicked() {
    if (m_selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "No File", "Please select a CSV file to train the model.");
        return;
    }

    ui->statusLabel->setText("Status: Loading new data...");
    ui->progressBar->setValue(10);
    qApp->processEvents(); // Process UI events to update status

    // Attempt to load the new data
    KaggleData newDataHandler; // Use a temporary handler to load new data
    if (!newDataHandler.loadData(m_selectedFilePath.toStdString())) {
        QMessageBox::critical(this, "Load Error", "Failed to load the selected CSV file. Please check its format.");
        ui->statusLabel->setText("Status: Training failed (Data Load Error)");
        ui->progressBar->setValue(0);
        return;
    }

    ui->statusLabel->setText("Status: Training model...");
    ui->progressBar->setValue(50);
    qApp->processEvents();

    try {
       m_randomForestModel.fit(newDataHandler.getFeatures(), newDataHandler.getLabels());
        m_randomForestModel.trainModelWithNewData(newDataHandler); // Assuming such a method exists

        ui->statusLabel->setText("Status: Training complete!");
        ui->progressBar->setValue(100);
        QMessageBox::information(this, "Training Success", "Model successfully trained with the new dataset.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Training Error", QString("An error occurred during training: ") + e.what());
        ui->statusLabel->setText("Status: Training failed.");
        ui->progressBar->setValue(0);
    }
}
