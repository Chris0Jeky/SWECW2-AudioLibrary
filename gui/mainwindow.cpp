#include "mainwindow.h"
#include "trackdialog.h"
#include "searchdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QProgressDialog>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QGroupBox>
#include <QSplitter>
#include <QTextEdit>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "audio_library/file_io.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , library(std::make_unique<MusicLibrary>())
    , searchEngine(std::make_unique<SearchEngine>())
    , currentSortField("title")
{
    setupUi();
    createActions();
    createMenus();
    createToolBar();
    
    // Initialize with empty library
    refreshTable();
    updateStatusBar();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
    // Set window properties
    setWindowTitle("Audio Library Manager");
    setMinimumSize(1000, 600);
    
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Search bar
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* searchLabel = new QLabel("Quick Search:");
    quickSearchEdit = new QLineEdit();
    quickSearchEdit->setPlaceholderText("Search by title, artist, album...");
    connect(quickSearchEdit, &QLineEdit::textChanged, this, &MainWindow::quickSearch);
    
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(quickSearchEdit);
    searchLayout->addStretch();
    
    // Sort combo box
    QLabel* sortLabel = new QLabel("Sort by:");
    sortComboBox = new QComboBox();
    sortComboBox->addItems({"Title", "Artist", "Album", "Year", "Rating", "Play Count"});
    connect(sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::sortChanged);
    
    searchLayout->addWidget(sortLabel);
    searchLayout->addWidget(sortComboBox);
    
    mainLayout->addLayout(searchLayout);
    
    // Track table
    trackTable = new QTableWidget(0, 8);
    QStringList headers = {"Title", "Artist", "Album", "Duration", "Genre", "Year", "Rating", "Plays"};
    trackTable->setHorizontalHeaderLabels(headers);
    trackTable->setAlternatingRowColors(true);
    trackTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    trackTable->setSortingEnabled(false);
    trackTable->horizontalHeader()->setStretchLastSection(true);
    trackTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    connect(trackTable, &QTableWidget::cellDoubleClicked,
            this, &MainWindow::onTableDoubleClicked);
    connect(trackTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);
    
    mainLayout->addWidget(trackTable);
    
    // Button panel
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    addButton = new QPushButton("Add Track");
    addButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTrack);
    
    editButton = new QPushButton("Edit Track");
    editButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    editButton->setEnabled(false);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editTrack);
    
    removeButton = new QPushButton("Remove Track");
    removeButton->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    removeButton->setEnabled(false);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeTrack);
    
    searchButton = new QPushButton("Advanced Search");
    searchButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchTracks);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Status bar
    statusLabel = new QLabel("Ready");
    statusBar()->addWidget(statusLabel);
}

void MainWindow::createActions() {
    // File actions
    addAction = new QAction("&Add Track...", this);
    addAction->setShortcut(QKeySequence::New);
    addAction->setStatusTip("Add a new track to the library");
    connect(addAction, &QAction::triggered, this, &MainWindow::addTrack);
    
    removeAction = new QAction("&Remove Track", this);
    removeAction->setShortcut(QKeySequence::Delete);
    removeAction->setStatusTip("Remove selected track from the library");
    removeAction->setEnabled(false);
    connect(removeAction, &QAction::triggered, this, &MainWindow::removeTrack);
    
    editAction = new QAction("&Edit Track...", this);
    editAction->setShortcut(QKeySequence("Ctrl+E"));
    editAction->setStatusTip("Edit selected track");
    editAction->setEnabled(false);
    connect(editAction, &QAction::triggered, this, &MainWindow::editTrack);
    
    searchAction = new QAction("&Search...", this);
    searchAction->setShortcut(QKeySequence::Find);
    searchAction->setStatusTip("Advanced search");
    connect(searchAction, &QAction::triggered, this, &MainWindow::searchTracks);
    
    importAction = new QAction("&Import...", this);
    importAction->setShortcut(QKeySequence::Open);
    importAction->setStatusTip("Import tracks from file");
    connect(importAction, &QAction::triggered, this, &MainWindow::importFromFile);
    
    exportCsvAction = new QAction("Export to &CSV...", this);
    exportCsvAction->setStatusTip("Export library to CSV file");
    connect(exportCsvAction, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Export to CSV", "", "CSV Files (*.csv)");
        if (!fileName.isEmpty()) {
            auto tracks = library->get_all_tracks();
            std::string error;
            if (FileIO::export_to_csv(tracks, fileName.toStdString(), error)) {
                QMessageBox::information(this, "Success", "Library exported successfully!");
            } else {
                QMessageBox::critical(this, "Error", QString::fromStdString(error));
            }
        }
    });
    
    exportJsonAction = new QAction("Export to &JSON...", this);
    exportJsonAction->setStatusTip("Export library to JSON file");
    connect(exportJsonAction, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Export to JSON", "", "JSON Files (*.json)");
        if (!fileName.isEmpty()) {
            auto tracks = library->get_all_tracks();
            std::string error;
            if (FileIO::export_to_json(tracks, fileName.toStdString(), error)) {
                QMessageBox::information(this, "Success", "Library exported successfully!");
            } else {
                QMessageBox::critical(this, "Error", QString::fromStdString(error));
            }
        }
    });
    
    loadSampleAction = new QAction("Load &Sample Data", this);
    loadSampleAction->setStatusTip("Load sample tracks");
    connect(loadSampleAction, &QAction::triggered, this, &MainWindow::loadSampleData);
    
    refreshAction = new QAction("&Refresh", this);
    refreshAction->setShortcut(QKeySequence::Refresh);
    refreshAction->setStatusTip("Refresh the track list");
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refreshTable);
    
    statsAction = new QAction("&Statistics...", this);
    statsAction->setStatusTip("Show library statistics");
    connect(statsAction, &QAction::triggered, this, &MainWindow::showStatistics);
    
    exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip("Exit the application");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Help actions
    aboutAction = new QAction("&About", this);
    aboutAction->setStatusTip("About this application");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
    
    aboutQtAction = new QAction("About &Qt", this);
    aboutQtAction->setStatusTip("About Qt");
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus() {
    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(addAction);
    fileMenu->addAction(editAction);
    fileMenu->addAction(removeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(importAction);
    fileMenu->addAction(exportCsvAction);
    fileMenu->addAction(exportJsonAction);
    fileMenu->addSeparator();
    fileMenu->addAction(loadSampleAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    
    // Edit menu
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(searchAction);
    editMenu->addSeparator();
    editMenu->addAction(refreshAction);
    
    // View menu
    QMenu* viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction(statsAction);
    
    // Help menu
    QMenu* helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createToolBar() {
    QToolBar* toolBar = addToolBar("Main");
    toolBar->setMovable(false);
    
    toolBar->addAction(addAction);
    toolBar->addAction(editAction);
    toolBar->addAction(removeAction);
    toolBar->addSeparator();
    toolBar->addAction(searchAction);
    toolBar->addAction(refreshAction);
}

void MainWindow::addTrack() {
    TrackDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        auto track = dialog.getTrack();
        std::string error;
        if (library->add_track(track, error)) {
            refreshTable();
            updateStatusBar();
            statusBar()->showMessage("Track added successfully", 2000);
        } else {
            QMessageBox::warning(this, "Error", QString::fromStdString(error));
        }
    }
}

void MainWindow::removeTrack() {
    int row = trackTable->currentRow();
    if (row >= 0 && row < currentTracks.size()) {
        auto track = currentTracks[row];
        
        int ret = QMessageBox::question(this, "Confirm Deletion",
            QString("Are you sure you want to remove \"%1\" by %2?")
                .arg(QString::fromStdString(track->get_title()))
                .arg(QString::fromStdString(track->get_artist())),
            QMessageBox::Yes | QMessageBox::No);
            
        if (ret == QMessageBox::Yes) {
            std::string error;
            if (library->remove_track(track->get_title(), track->get_artist(), error)) {
                refreshTable();
                updateStatusBar();
                statusBar()->showMessage("Track removed successfully", 2000);
            } else {
                QMessageBox::warning(this, "Error", QString::fromStdString(error));
            }
        }
    }
}

void MainWindow::editTrack() {
    int row = trackTable->currentRow();
    if (row >= 0 && row < currentTracks.size()) {
        auto track = currentTracks[row];
        
        TrackDialog dialog(this, track);
        if (dialog.exec() == QDialog::Accepted) {
            auto newTrack = dialog.getTrack();
            std::string error;
            
            // Remove old track and add new one
            if (library->remove_track(track->get_title(), track->get_artist(), error) &&
                library->add_track(newTrack, error)) {
                refreshTable();
                updateStatusBar();
                statusBar()->showMessage("Track updated successfully", 2000);
            } else {
                QMessageBox::warning(this, "Error", QString::fromStdString(error));
            }
        }
    }
}

void MainWindow::searchTracks() {
    SearchDialog dialog(searchEngine.get(), library.get(), this);
    if (dialog.exec() == QDialog::Accepted) {
        currentTracks = dialog.getResults();
        populateTable();
        statusBar()->showMessage(QString("Found %1 tracks").arg(currentTracks.size()), 2000);
    }
}

void MainWindow::importFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, 
        "Import Tracks", "", "CSV Files (*.csv);;JSON Files (*.json)");
        
    if (!fileName.isEmpty()) {
        QProgressDialog progress("Importing tracks...", "Cancel", 0, 100, this);
        progress.setWindowModality(Qt::WindowModal);
        
        std::vector<std::shared_ptr<Track>> tracks;
        std::string error;
        bool success = false;
        
        if (fileName.endsWith(".csv")) {
            success = FileIO::import_from_csv(fileName.toStdString(), tracks, error,
                [&progress](size_t current, size_t total) {
                    progress.setValue(static_cast<int>(current * 100 / total));
                    return !progress.wasCanceled();
                });
        } else if (fileName.endsWith(".json")) {
            success = FileIO::import_from_json(fileName.toStdString(), tracks, error);
            progress.setValue(50);
        }
        
        if (success) {
            int added = 0;
            for (const auto& track : tracks) {
                std::string addError;
                if (library->add_track(track, addError)) {
                    added++;
                }
            }
            
            progress.setValue(100);
            refreshTable();
            updateStatusBar();
            QMessageBox::information(this, "Import Complete",
                QString("Successfully imported %1 out of %2 tracks.")
                    .arg(added).arg(tracks.size()));
        } else {
            QMessageBox::critical(this, "Import Error", QString::fromStdString(error));
        }
    }
}

void MainWindow::exportToFile() {
    exportCsvAction->trigger();
}

void MainWindow::loadSampleData() {
    std::vector<std::shared_ptr<Track>> sampleTracks = {
        std::make_shared<Track>("Bohemian Rhapsody", "Queen", 354, "A Night at the Opera", "Rock", 1975, 150, 5.0),
        std::make_shared<Track>("Imagine", "John Lennon", 183, "Imagine", "Rock", 1971, 120, 5.0),
        std::make_shared<Track>("Hotel California", "Eagles", 391, "Hotel California", "Rock", 1976, 98, 4.5),
        std::make_shared<Track>("Stairway to Heaven", "Led Zeppelin", 482, "Led Zeppelin IV", "Rock", 1971, 142, 5.0),
        std::make_shared<Track>("Billie Jean", "Michael Jackson", 294, "Thriller", "Pop", 1982, 89, 4.5),
        std::make_shared<Track>("Like a Rolling Stone", "Bob Dylan", 369, "Highway 61 Revisited", "Rock", 1965, 76, 4.0),
        std::make_shared<Track>("Smells Like Teen Spirit", "Nirvana", 301, "Nevermind", "Grunge", 1991, 110, 4.5),
        std::make_shared<Track>("Hey Jude", "The Beatles", 431, "Hey Jude", "Rock", 1968, 134, 5.0),
        std::make_shared<Track>("One", "U2", 276, "Achtung Baby", "Rock", 1991, 65, 4.0),
        std::make_shared<Track>("Sweet Child O' Mine", "Guns N' Roses", 356, "Appetite for Destruction", "Rock", 1987, 92, 4.5)
    };
    
    int added = 0;
    for (const auto& track : sampleTracks) {
        std::string error;
        if (library->add_track(track, error)) {
            added++;
        }
    }
    
    refreshTable();
    updateStatusBar();
    QMessageBox::information(this, "Sample Data Loaded",
        QString("Successfully added %1 sample tracks.").arg(added));
}

void MainWindow::refreshTable() {
    currentTracks = library->get_all_tracks();
    searchEngine->build_index(currentTracks);
    populateTable();
}

void MainWindow::showStatistics() {
    auto stats = library->get_statistics();
    
    QString statsText = QString(
        "Library Statistics\n\n"
        "Total Tracks: %1\n"
        "Total Artists: %2\n"
        "Total Albums: %3\n"
        "Total Duration: %4\n"
        "Average Rating: %5\n"
        "Total Play Count: %6")
        .arg(stats.total_tracks)
        .arg(stats.unique_artists)
        .arg(stats.unique_albums)
        .arg(QString::fromStdString(formatDuration(stats.total_duration)))
        .arg(stats.average_rating, 0, 'f', 1)
        .arg(stats.total_play_count);
    
    QMessageBox::information(this, "Library Statistics", statsText);
}

void MainWindow::sortChanged(int index) {
    switch (index) {
        case 0: currentSortField = "title"; break;
        case 1: currentSortField = "artist"; break;
        case 2: currentSortField = "album"; break;
        case 3: currentSortField = "year"; break;
        case 4: currentSortField = "rating"; break;
        case 5: currentSortField = "playcount"; break;
    }
    populateTable();
}

void MainWindow::onTableDoubleClicked(int row, int column) {
    Q_UNUSED(column)
    if (row >= 0 && row < currentTracks.size()) {
        editTrack();
    }
}

void MainWindow::onSelectionChanged() {
    bool hasSelection = trackTable->currentRow() >= 0;
    removeButton->setEnabled(hasSelection);
    editButton->setEnabled(hasSelection);
    removeAction->setEnabled(hasSelection);
    editAction->setEnabled(hasSelection);
}

void MainWindow::quickSearch() {
    QString searchText = quickSearchEdit->text();
    
    if (searchText.isEmpty()) {
        refreshTable();
        return;
    }
    
    SearchOptions options;
    options.mode = SearchEngine::SearchMode::SUBSTRING;
    options.case_sensitive = false;
    options.search_in_title = true;
    options.search_in_artist = true;
    options.search_in_album = true;
    options.search_in_genre = true;
    
    currentTracks = searchEngine->search(searchText.toStdString(), options);
    populateTable();
    
    statusBar()->showMessage(QString("Found %1 tracks").arg(currentTracks.size()), 2000);
}

void MainWindow::about() {
    QMessageBox::about(this, "About Audio Library Manager",
        "<h3>Audio Library Manager</h3>"
        "<p>A modern music library management application with advanced search capabilities.</p>"
        "<p>Features:</p>"
        "<ul>"
        "<li>Add, edit, and remove tracks</li>"
        "<li>Import/export CSV and JSON</li>"
        "<li>Advanced search with multiple modes</li>"
        "<li>Sort by various fields</li>"
        "<li>Track statistics</li>"
        "</ul>"
        "<p>Built with Qt5 and C++17</p>");
}

void MainWindow::populateTable() {
    // Sort tracks
    std::sort(currentTracks.begin(), currentTracks.end(),
        [this](const std::shared_ptr<Track>& a, const std::shared_ptr<Track>& b) {
            if (currentSortField == "title") return a->get_title() < b->get_title();
            else if (currentSortField == "artist") return a->get_artist() < b->get_artist();
            else if (currentSortField == "album") return a->get_album() < b->get_album();
            else if (currentSortField == "year") return a->get_year() > b->get_year();
            else if (currentSortField == "rating") return a->get_rating() > b->get_rating();
            else if (currentSortField == "playcount") return a->get_play_count() > b->get_play_count();
            return false;
        });
    
    // Clear and populate table
    trackTable->setRowCount(0);
    trackTable->setRowCount(currentTracks.size());
    
    for (size_t i = 0; i < currentTracks.size(); ++i) {
        const auto& track = currentTracks[i];
        
        trackTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(track->get_title())));
        trackTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(track->get_artist())));
        trackTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(track->get_album())));
        trackTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(formatDuration(track->get_duration()))));
        trackTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(track->get_genre())));
        trackTable->setItem(i, 5, new QTableWidgetItem(track->get_year() > 0 ? QString::number(track->get_year()) : "-"));
        trackTable->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(formatRating(track->get_rating()))));
        trackTable->setItem(i, 7, new QTableWidgetItem(QString::number(track->get_play_count())));
    }
    
    trackTable->resizeColumnsToContents();
}

void MainWindow::updateStatusBar() {
    auto stats = library->get_statistics();
    statusLabel->setText(QString("%1 tracks | %2 artists | Total duration: %3")
        .arg(stats.total_tracks)
        .arg(stats.unique_artists)
        .arg(QString::fromStdString(formatDuration(stats.total_duration))));
}

std::string MainWindow::formatDuration(int seconds) const {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    
    std::stringstream ss;
    if (hours > 0) {
        ss << hours << ":" << std::setfill('0') << std::setw(2) << minutes 
           << ":" << std::setw(2) << secs;
    } else {
        ss << minutes << ":" << std::setfill('0') << std::setw(2) << secs;
    }
    return ss.str();
}

std::string MainWindow::formatRating(double rating) const {
    if (rating <= 0) return "-";
    
    int stars = static_cast<int>(std::round(rating));
    std::string result;
    for (int i = 0; i < stars; ++i) {
        result += "★";
    }
    for (int i = stars; i < 5; ++i) {
        result += "☆";
    }
    return result;
}