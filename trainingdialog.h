#ifndef TRAININGDIALOG_H
#define TRAININGDIALOG_H

#include <QDialog>
#include "src/KaggleData.h" // For loading new data
#include "src/RandomForest.h" // For model training

QT_BEGIN_NAMESPACE
namespace Ui { class TrainingDialog; }
QT_END_NAMESPACE

class TrainingDialog : public QDialog {
    Q_OBJECT

public:
    explicit TrainingDialog(KaggleData& data, RandomForest& model, QWidget *parent = nullptr);
    ~TrainingDialog();

private slots:
    void on_browseButton_clicked();
    void on_trainButton_clicked();

private:
    Ui::TrainingDialog *ui;
    KaggleData& m_kaggleData; // Reference to the main window's data handler
    RandomForest& m_randomForestModel; // Reference to the main window's model
    QString m_selectedFilePath;
};

#endif // TRAININGDIALOG_H
