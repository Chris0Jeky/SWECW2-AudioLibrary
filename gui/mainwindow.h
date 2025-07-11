#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <memory>
#include "audio_library/music_library.h"
#include "audio_library/search_engine.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTrack();
    void removeTrack();
    void editTrack();
    void searchTracks();
    void importFromFile();
    void exportToFile();
    void loadSampleData();
    void refreshTable();
    void showStatistics();
    void sortChanged(int index);
    void onTableDoubleClicked(int row, int column);
    void onSelectionChanged();
    void quickSearch();
    void about();

private:
    void setupUi();
    void createActions();
    void createMenus();
    void createToolBar();
    void populateTable();
    void updateStatusBar();
    std::string formatDuration(int seconds) const;
    std::string formatRating(double rating) const;

    // UI elements
    QTableWidget* trackTable;
    QLineEdit* quickSearchEdit;
    QComboBox* sortComboBox;
    QLabel* statusLabel;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* editButton;
    QPushButton* searchButton;

    // Actions
    QAction* addAction;
    QAction* removeAction;
    QAction* editAction;
    QAction* searchAction;
    QAction* importAction;
    QAction* exportCsvAction;
    QAction* exportJsonAction;
    QAction* loadSampleAction;
    QAction* refreshAction;
    QAction* statsAction;
    QAction* exitAction;
    QAction* aboutAction;
    QAction* aboutQtAction;

    // Core objects
    std::unique_ptr<MusicLibrary> library;
    std::unique_ptr<SearchEngine> searchEngine;
    std::vector<std::shared_ptr<Track>> currentTracks;
    std::string currentSortField;
};

#endif // MAINWINDOW_H