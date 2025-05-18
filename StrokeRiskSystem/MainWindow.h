#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <string>
#include <memory>

#include "src/DataProcessor.h"
#include "src/RandomForest.h"
#include "src/StrokeRiskPredictor.h"
#include "src/ComparisonEngine.h"
#include "src/UserProfile.h"

class PredictionDialog;
class TrainingDialog;
class ComparisonBoardDialog;

class MainWindow : public Fl_Window {
public:
    static constexpr int BUTTON_HEIGHT = 40;
    static constexpr int BUTTON_WIDTH = 300;
    static constexpr int SPACING = 10;
    static constexpr int MARGIN = 20;

    MainWindow();
    ~MainWindow();

private:
    Fl_Box* welcomeLabel;
    Fl_Button* predictRiskButton;
    Fl_Button* trainModelButton;
    Fl_Button* compareDashboardButton;

    DataProcessor m_dataProcessor;
    RandomForest m_randomForestModel;
    StrokeRiskPredictor m_riskPredictor;
    ComparisonEngine m_comparisonEngine;

    UserProfile m_lastPredictedUserProfile;

    static void predict_callback(Fl_Widget* w, void* data);
    static void train_callback(Fl_Widget* w, void* data);
    static void compare_callback(Fl_Widget* w, void* data);

    void initializeModelAndData();
    void showMessage(const std::string& title, const std::string& message, bool isError = false);

    // New: Central place to define the data file name for consistency
    const std::string dataFileName = "stroke_data_new.csv";
};

#endif // MAINWINDOW_H