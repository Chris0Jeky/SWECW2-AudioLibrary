#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Audio Library Manager");
    app.setOrganizationName("AudioLib");
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}