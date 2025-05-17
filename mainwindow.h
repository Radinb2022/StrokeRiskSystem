#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Forward declarations or includes for backend classes if needed
#include "src/RandomForest.h"
#include "src/StrokeRiskPredictor.h"
#include "src/UserProfile.h"
#include "src/KaggleData.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_predictRiskButton_clicked();
    void on_trainModelButton_clicked();
    void on_compareDashboardButton_clicked();

private:
    Ui::MainWindow *ui; // This will hold the UI elements from mainwindow.ui

    RandomForest *m_randomForestModel;
     StrokeRiskPredictor *m_riskPredictor;
    UserProfile *m_userProfile;
    KaggleData *m_kaggleData;
};

#endif // MAINWINDOW_H
