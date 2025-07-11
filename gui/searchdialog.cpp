#include "searchdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFileDialog>
#include "audio_library/file_io.h"

SearchDialog::SearchDialog(SearchEngine* engine, MusicLibrary* lib, QWidget *parent)
    : QDialog(parent)
    , searchEngine(engine)
    , library(lib)
{
    setupUi();
    queryEdit->setFocus();
}

void SearchDialog::setupUi() {
    setWindowTitle("Advanced Search");
    setModal(true);
    resize(600, 500);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Search query
    QHBoxLayout* queryLayout = new QHBoxLayout();
    queryLayout->addWidget(new QLabel("Search Query:"));
    queryEdit = new QLineEdit();
    queryEdit->setPlaceholderText("Enter search terms...");
    queryLayout->addWidget(queryEdit);
    
    searchButton = new QPushButton("Search");
    searchButton->setDefault(true);
    connect(searchButton, &QPushButton::clicked, this, &SearchDialog::performSearch);
    queryLayout->addWidget(searchButton);
    
    mainLayout->addLayout(queryLayout);
    
    // Search options
    QGroupBox* optionsGroup = new QGroupBox("Search Options");
    QVBoxLayout* optionsLayout = new QVBoxLayout(optionsGroup);
    
    // Mode selection
    QHBoxLayout* modeLayout = new QHBoxLayout();
    modeLayout->addWidget(new QLabel("Search Mode:"));
    modeCombo = new QComboBox();
    modeCombo->addItems({"Substring", "Exact Match", "Prefix Match", "Fuzzy Match", "Regular Expression"});
    connect(modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SearchDialog::onModeChanged);
    modeLayout->addWidget(modeCombo);
    modeLayout->addStretch();
    optionsLayout->addLayout(modeLayout);
    
    // Field selection
    QHBoxLayout* fieldsLayout = new QHBoxLayout();
    fieldsLayout->addWidget(new QLabel("Search In:"));
    
    searchTitleCheck = new QCheckBox("Title");
    searchTitleCheck->setChecked(true);
    fieldsLayout->addWidget(searchTitleCheck);
    
    searchArtistCheck = new QCheckBox("Artist");
    searchArtistCheck->setChecked(true);
    fieldsLayout->addWidget(searchArtistCheck);
    
    searchAlbumCheck = new QCheckBox("Album");
    searchAlbumCheck->setChecked(true);
    fieldsLayout->addWidget(searchAlbumCheck);
    
    searchGenreCheck = new QCheckBox("Genre");
    searchGenreCheck->setChecked(true);
    fieldsLayout->addWidget(searchGenreCheck);
    
    fieldsLayout->addStretch();
    optionsLayout->addLayout(fieldsLayout);
    
    // Other options
    QHBoxLayout* otherLayout = new QHBoxLayout();
    
    caseSensitiveCheck = new QCheckBox("Case Sensitive");
    otherLayout->addWidget(caseSensitiveCheck);
    
    otherLayout->addWidget(new QLabel("Max Results:"));
    maxResultsSpin = new QSpinBox();
    maxResultsSpin->setRange(1, 1000);
    maxResultsSpin->setValue(100);
    otherLayout->addWidget(maxResultsSpin);
    
    otherLayout->addStretch();
    optionsLayout->addLayout(otherLayout);
    
    mainLayout->addWidget(optionsGroup);
    
    // Results list
    QLabel* resultsLabel = new QLabel("Search Results:");
    mainLayout->addWidget(resultsLabel);
    
    resultsList = new QListWidget();
    connect(resultsList, &QListWidget::itemDoubleClicked,
            this, &SearchDialog::onResultDoubleClicked);
    mainLayout->addWidget(resultsList);
    
    // Export button
    exportButton = new QPushButton("Export Results");
    exportButton->setEnabled(false);
    connect(exportButton, &QPushButton::clicked, [this]() {
        if (searchResults.empty()) return;
        
        QString fileName = QFileDialog::getSaveFileName(this, 
            "Export Search Results", "", "CSV Files (*.csv);;JSON Files (*.json)");
            
        if (!fileName.isEmpty()) {
            std::string error;
            bool success = false;
            
            if (fileName.endsWith(".csv")) {
                success = FileIO::export_to_csv(searchResults, fileName.toStdString(), error);
            } else if (fileName.endsWith(".json")) {
                success = FileIO::export_to_json(searchResults, fileName.toStdString(), error);
            }
            
            if (success) {
                QMessageBox::information(this, "Export Successful", 
                    QString("Exported %1 tracks successfully.").arg(searchResults.size()));
            } else {
                QMessageBox::critical(this, "Export Error", QString::fromStdString(error));
            }
        }
    });
    
    // Button box
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    buttonBox->addButton(exportButton, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
    
    // Connect enter key in query edit
    connect(queryEdit, &QLineEdit::returnPressed, this, &SearchDialog::performSearch);
}

void SearchDialog::performSearch() {
    QString query = queryEdit->text().trimmed();
    if (query.isEmpty()) {
        QMessageBox::warning(this, "Search", "Please enter a search query.");
        return;
    }
    
    // Build search options
    SearchOptions options;
    
    // Set search mode
    switch (modeCombo->currentIndex()) {
        case 0: options.mode = SearchEngine::SearchMode::SUBSTRING; break;
        case 1: options.mode = SearchEngine::SearchMode::EXACT; break;
        case 2: options.mode = SearchEngine::SearchMode::PREFIX; break;
        case 3: options.mode = SearchEngine::SearchMode::FUZZY; break;
        case 4: options.mode = SearchEngine::SearchMode::REGEX; break;
    }
    
    options.case_sensitive = caseSensitiveCheck->isChecked();
    options.search_in_title = searchTitleCheck->isChecked();
    options.search_in_artist = searchArtistCheck->isChecked();
    options.search_in_album = searchAlbumCheck->isChecked();
    options.search_in_genre = searchGenreCheck->isChecked();
    options.max_results = maxResultsSpin->value();
    
    // Perform search
    try {
        searchResults = searchEngine->search(query.toStdString(), options);
        displayResults();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Search Error", 
            QString("Search failed: %1").arg(e.what()));
    }
}

void SearchDialog::onModeChanged(int index) {
    // Update UI hints based on mode
    switch (index) {
        case 0: // Substring
            queryEdit->setPlaceholderText("Enter search terms...");
            break;
        case 1: // Exact
            queryEdit->setPlaceholderText("Enter exact text to match...");
            break;
        case 2: // Prefix
            queryEdit->setPlaceholderText("Enter prefix to match...");
            break;
        case 3: // Fuzzy
            queryEdit->setPlaceholderText("Enter approximate text...");
            break;
        case 4: // Regex
            queryEdit->setPlaceholderText("Enter regular expression...");
            break;
    }
}

void SearchDialog::onResultDoubleClicked() {
    accept();
}

void SearchDialog::displayResults() {
    resultsList->clear();
    
    if (searchResults.empty()) {
        resultsList->addItem("No results found.");
        exportButton->setEnabled(false);
        return;
    }
    
    for (const auto& track : searchResults) {
        QString item = QString("%1 - %2")
            .arg(QString::fromStdString(track->get_title()))
            .arg(QString::fromStdString(track->get_artist()));
            
        if (!track->get_album().empty()) {
            item += QString(" (%1)").arg(QString::fromStdString(track->get_album()));
        }
        
        resultsList->addItem(item);
    }
    
    exportButton->setEnabled(true);
    
    // Update status
    QString status = QString("Found %1 track%2")
        .arg(searchResults.size())
        .arg(searchResults.size() != 1 ? "s" : "");
    setWindowTitle(QString("Advanced Search - %1").arg(status));
}