#include "TrainingDialog.h"
#include <FL/Fl_File_Chooser.H> // For native file dialog
#include <FL/fl_ask.H> // For fl_alert
#include <iostream>
#include <string>

TrainingDialog::TrainingDialog(DataProcessor& dataProcessor, RandomForest& model)
    : Fl_Window(500, 300, "Train Model with New Data"),
      m_dataProcessor(dataProcessor),
      m_randomForestModel(model)
{
    size_range(500, 300, 500, 300); // Fixed size

    begin(); // Start adding widgets

    titleLabel = new Fl_Box(20, 20, w() - 40, 30, "Upload and Train Model with Custom Dataset");
    titleLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    titleLabel->labelsize(16);
    titleLabel->labelfont(FL_BOLD);

    filePathInput = new Fl_Input(20, 70, w() - 120, 30, "File Path:");
    filePathInput->readonly(1); // Read-only
    filePathInput->value("No file selected");

    browseButton = new Fl_Button(w() - 90, 70, 70, 30, "Browse...");
    browseButton->callback(browse_callback, this);

    trainButton = new Fl_Button((w() - 150) / 2, 120, 150, 40, "Start Training");
    trainButton->callback(train_callback, this);
    trainButton->deactivate(); // Initially disabled

    progressBar = new Fl_Progress(20, 180, w() - 40, 25);
    progressBar->value(0);

    statusLabel = new Fl_Box(20, 220, w() - 40, 25, "Status: Ready");
    statusLabel->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

    end(); // Stop adding widgets
}

TrainingDialog::~TrainingDialog() {
    // FLTK widgets are usually managed by their parent Fl_Group/Fl_Window.
}

void TrainingDialog::browse_callback(Fl_Widget* w, void* data) {
    TrainingDialog* self = static_cast<TrainingDialog*>(data);

    // Using Fl_Native_File_Chooser for a more modern file dialog
    Fl_Native_File_Chooser fileChooser;
    fileChooser.title("Upload New Dataset");
    fileChooser.filter("CSV Files (*.csv)");
    fileChooser.type(Fl_Native_File_Chooser::BROWSE_FILE);

    if (fileChooser.show() == 0) { // 0 means OK (file selected)
        self->m_selectedFilePath = fileChooser.filename();
        self->filePathInput->value(self->m_selectedFilePath.c_str());
        self->trainButton->activate();
        self->statusLabel->label("Status: File selected. Ready to train.");
    } else {
        self->m_selectedFilePath = "";
        self->filePathInput->value("No file selected");
        self->trainButton->deactivate();
        self->statusLabel->label("Status: No file selected.");
    }
    Fl::check(); // Update UI
}

void TrainingDialog::train_callback(Fl_Widget* w, void* data) {
    TrainingDialog* self = static_cast<TrainingDialog*>(data);

    if (self->m_selectedFilePath.empty()) {
        fl_alert("Please select a CSV file to train the model.");
        return;
    }

    self->statusLabel->label("Status: Loading new data...");
    self->progressBar->value(10);
    Fl::check(); // Update UI

    DataProcessor newFileProcessor; // Temporary DataProcessor for the new file
    if (!newFileProcessor.loadCSV(self->m_selectedFilePath)) {
        fl_alert("Load Error", "Failed to load the selected CSV file. Please check its format and attributes.");
        self->statusLabel->label("Status: Training failed (Data Load Error)");
        self->progressBar->value(0);
        Fl::check();
        return;
    }

    self->statusLabel->label("Status: Training model...");
    self->progressBar->value(50);
    Fl::check();

    try {
        self->m_randomForestModel.fit(newFileProcessor.getProcessedFeatures(), newFileProcessor.getLabels());
        self->statusLabel->label("Status: Training complete!");
        self->progressBar->value(100);
        fl_message("Training Success", "Model successfully trained with the new dataset.");
    } catch (const std::exception& e) {
        fl_alert("Training Error", "An error occurred during training:\n%s", e.what());
        self->statusLabel->label("Status: Training failed.");
        self->progressBar->value(0);
    }
    Fl::check();
}
