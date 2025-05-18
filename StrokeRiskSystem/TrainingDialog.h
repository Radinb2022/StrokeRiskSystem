#ifndef TRAININGDIALOG_H
#define TRAININGDIALOG_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Progress.H>
#include <string>

#include "src/DataProcessor.h"
#include "src/RandomForest.h"

class TrainingDialog : public Fl_Window {
public:
    TrainingDialog(DataProcessor& dataProcessor, RandomForest& model);
    ~TrainingDialog();

private:
    Fl_Box* titleLabel;
    Fl_Input* filePathInput;
    Fl_Button* browseButton;
    Fl_Button* trainButton;
    Fl_Progress* progressBar;
    Fl_Box* statusLabel;

    DataProcessor& m_dataProcessor;
    RandomForest& m_randomForestModel;
    std::string m_selectedFilePath;

    static void browse_callback(Fl_Widget* w, void* data);
    static void train_callback(Fl_Widget* w, void* data);
};

#endif // TRAININGDIALOG_H