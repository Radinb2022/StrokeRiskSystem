#include <FL/Fl.H> // FLTK core
#include "MainWindow.h" // Our main window class

int main(int argc, char **argv) {
    // Initialize FLTK, process command line arguments
    Fl::args(argc, argv);

    // Create the main application window
    MainWindow mainWindow;

    // Show the window
    mainWindow.show();

    // Start the FLTK event loop
    return Fl::run();
}