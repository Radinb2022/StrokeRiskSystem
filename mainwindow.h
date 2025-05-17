#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H> // For general text display/input
#include <string>
#include <memory> // For std::unique_ptr

// Include your backend classes
#include "src/DataProcessor.h"
#include "src/RandomForest.h"
#include "src/StrokeRiskPredictor.h"
#include "src/ComparisonEngine.h"
#include "src/UserProfile.h"

// Forward declarations for dialogs to avoid circular includes
// (although FLTK often makes this less of an issue than Qt for simple dialogs)
class PredictionDialog;
class TrainingDialog;
class ComparisonBoardDialog;

class MainWindow : public Fl_Window {
public:
    // Constants for layout
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

    // Backend class instances
    DataProcessor m_dataProcessor;
    RandomForest m_randomForestModel;
    StrokeRiskPredictor m_riskPredictor;
    ComparisonEngine m_comparisonEngine;

    UserProfile m_lastPredictedUserProfile; // Store the user profile from the last prediction

    // Callbacks for buttons
    static void predict_callback(Fl_Widget* w, void* data);
    static void train_callback(Fl_Widget* w, void* data);
    static void compare_callback(Fl_Widget* w, void* data);

    // Initializer method
    void initializeModelAndData();
    void showMessage(const std::string& title, const std::string& message, bool isError = false);
};

#endif // MAINWINDOW_H
