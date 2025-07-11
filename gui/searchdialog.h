#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QListWidget>
#include <vector>
#include <memory>
#include "audio_library/search_engine.h"
#include "audio_library/music_library.h"
#include "audio_library/track.h"

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    SearchDialog(SearchEngine* searchEngine, MusicLibrary* library, QWidget *parent = nullptr);
    
    std::vector<std::shared_ptr<Track>> getResults() const { return searchResults; }

private slots:
    void performSearch();
    void onModeChanged(int index);
    void onResultDoubleClicked();

private:
    void setupUi();
    void displayResults();
    
    // Core objects
    SearchEngine* searchEngine;
    MusicLibrary* library;
    std::vector<std::shared_ptr<Track>> searchResults;
    
    // UI elements
    QLineEdit* queryEdit;
    QComboBox* modeCombo;
    QCheckBox* caseSensitiveCheck;
    QCheckBox* searchTitleCheck;
    QCheckBox* searchArtistCheck;
    QCheckBox* searchAlbumCheck;
    QCheckBox* searchGenreCheck;
    QSpinBox* maxResultsSpin;
    QListWidget* resultsList;
    QPushButton* searchButton;
    QPushButton* exportButton;
};

#endif // SEARCHDIALOG_H