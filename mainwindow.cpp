#include "MainWindow.h"
#include <FL/fl_ask.H> // For fl_alert, fl_message
#include <FL/Fl_File_Chooser.H> // For file dialogs (in TrainingDialog but often linked with main)
#include <iostream>
#include <stdexcept> // For std::runtime_error
#include <FL/platform.H> // For Fl_Native_File_Chooser for modern file dialogs (optional)

// Include dialog headers
#include "PredictionDialog.h"
#include "TrainingDialog.h"
#include "ComparisonBoardDialog.h"

MainWindow::MainWindow()
    : Fl_Window(600, 400, "Stroke Risk Prediction System"),
      m_riskPredictor(&m_randomForestModel) // Initialize risk predictor with model
{
    // Make window non-resizable
    size_range(600, 400, 600, 400);

    // Centralize widgets in the window
    begin(); // Start adding widgets to the window

    // Welcome Label
    welcomeLabel = new Fl_Box(MARGIN, MARGIN, w() - 2 * MARGIN, 50, "Welcome to the AI-powered Stroke Risk Prediction System!");
    welcomeLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    welcomeLabel->labelsize(20);
    welcomeLabel->labelfont(FL_BOLD);

    // Calculate vertical position for buttons
    int current_y = welcomeLabel->y() + welcomeLabel->h() + 2 * SPACING; // Below label, with extra space

    // Predict Button
    predictRiskButton = new Fl_Button((w() - BUTTON_WIDTH) / 2, current_y, BUTTON_WIDTH, BUTTON_HEIGHT, "1. Generate Stroke Risk Prediction");
    predictRiskButton->callback(predict_callback, this);
    current_y += BUTTON_HEIGHT + SPACING;

    // Train Button
    trainModelButton = new Fl_Button((w() - BUTTON_WIDTH) / 2, current_y, BUTTON_WIDTH, BUTTON_HEIGHT, "2. Train Model with New Data");
    trainModelButton->callback(train_callback, this);
    current_y += BUTTON_HEIGHT + SPACING;

    // Compare Button
    compareDashboardButton = new Fl_Button((w() - BUTTON_WIDTH) / 2, current_y, BUTTON_WIDTH, BUTTON_HEIGHT, "3. View Comparison Dashboard");
    compareDashboardButton->callback(compare_callback, this);

    end(); // Stop adding widgets to the window

    // Initialize data and train model on startup
    initializeModelAndData();
}

MainWindow::~MainWindow() {
    // FLTK widgets are usually managed by their parent Fl_Group/Fl_Window.
    // If you explicitly `new` widgets not part of the window, you'd `delete` them here.
}

void MainWindow::showMessage(const std::string& title, const std::string& message, bool isError) {
    if (isError) {
        fl_alert("%s\n%s", title.c_str(), message.c_str());
    } else {
        fl_message("%s\n%s", title.c_str(), message.c_str());
    }
}

void MainWindow::initializeModelAndData() {
    // Determine the path to stroke_data.csv
    // Assumes `data/stroke_data.csv` is relative to the executable
    std::string dataPath = "data/stroke_data.csv"; // On Linux/macOS, executable is often directly in build folder

#ifdef _WIN32
    // On Windows, the executable might be in Debug/Release subfolder
    // If running from VS, it might be $(OutDir)/data/stroke_data.csv
    // The CMake POST_BUILD step copies 'data' next to the executable.
    // So, relative path "data/stroke_data.csv" should work.
    // If not, you might need to adjust or use full path.
#endif

    std::cout << "Attempting to load data from: " << dataPath << std::endl;

    if (!m_dataProcessor.loadCSV(dataPath)) {
        showMessage("Data Load Error", "Could not load stroke_data.csv. Please ensure it's in the 'data' subdirectory next to the executable.", true);
        predictRiskButton->deactivate();
        trainModelButton->deactivate();
        compareDashboardButton->deactivate();
        return;
    }
    std::cout << "Data loaded successfully. Features size: " << m_dataProcessor.getProcessedFeatures().size() << std::endl;
    std::cout << "Labels size: " << m_dataProcessor.getLabels().size() << std::endl;

    try {
        m_randomForestModel.fit(m_dataProcessor.getProcessedFeatures(), m_dataProcessor.getLabels());
        std::cout << "RandomForest model trained successfully." << std::endl;
    } catch (const std::exception& e) {
        showMessage("Model Training Error", "Initial model training failed: " + std::string(e.what()), true);
        predictRiskButton->deactivate();
        trainModelButton->deactivate();
        compareDashboardButton->deactivate();
        return;
    }

    showMessage("System Ready", "Stroke risk prediction system initialized and model loaded. You can now use its features.");
}

void MainWindow::predict_callback(Fl_Widget* w, void* data) {
    MainWindow* self = static_cast<MainWindow*>(data);
    PredictionDialog dialog(self->m_riskPredictor, self->m_dataProcessor);
    dialog.set_modal(); // Make it a modal dialog
    dialog.show();
    // FLTK dialogs run their own loop until closed
    while (dialog.shown()) {
        Fl::wait();
    }

    // After dialog closes, check if prediction was successful and update last predicted user profile
    if (dialog.getPredictionSuccess()) {
        self->m_lastPredictedUserProfile = dialog.getPredictedUserProfile();
        std::cout << "Last predicted user profile stored for comparison." << std::endl;
    }
}

void MainWindow::train_callback(Fl_Widget* w, void* data) {
    MainWindow* self = static_cast<MainWindow*>(data);
    TrainingDialog dialog(self->m_dataProcessor, self->m_randomForestModel);
    dialog.set_modal();
    dialog.show();
    while (dialog.shown()) {
        Fl::wait();
    }
    // Model in self->m_randomForestModel is updated directly by TrainingDialog if successful
}

void MainWindow::compare_callback(Fl_Widget* w, void* data) {
    MainWindow* self = static_cast<MainWindow*>(data);

    if (self->m_lastPredictedUserProfile.getAge() == 0 && self->m_lastPredictedUserProfile.getGender().empty()) { // Simple check for empty profile
        self->showMessage("No User Profile", "Please generate a prediction first to create your profile for comparison.", true);
        return;
    }

    ComparisonBoardDialog dialog(self->m_lastPredictedUserProfile, self->m_dataProcessor, self->m_randomForestModel, self->m_comparisonEngine);
    dialog.set_modal();
    dialog.show();
    while (dialog.shown()) {
        Fl::wait();
    }
}
