#include "trackdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

TrackDialog::TrackDialog(QWidget *parent, std::shared_ptr<Track> track)
    : QDialog(parent)
    , originalTrack(track)
{
    setupUi();
    
    if (track) {
        // Populate fields for editing
        titleEdit->setText(QString::fromStdString(track->get_title()));
        artistEdit->setText(QString::fromStdString(track->get_artist()));
        durationEdit->setValue(track->get_duration());
        albumEdit->setText(QString::fromStdString(track->get_album()));
        genreEdit->setText(QString::fromStdString(track->get_genre()));
        yearEdit->setValue(track->get_year());
        playCountEdit->setValue(track->get_play_count());
        ratingEdit->setValue(track->get_rating());
        
        setWindowTitle("Edit Track");
    }
}

void TrackDialog::setupUi() {
    setWindowTitle("Add Track");
    setModal(true);
    setMinimumWidth(400);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Form layout
    QFormLayout* formLayout = new QFormLayout();
    
    // Title (required)
    titleEdit = new QLineEdit();
    titleEdit->setPlaceholderText("Required");
    formLayout->addRow(new QLabel("Title*:"), titleEdit);
    
    // Artist (required)
    artistEdit = new QLineEdit();
    artistEdit->setPlaceholderText("Required");
    formLayout->addRow(new QLabel("Artist*:"), artistEdit);
    
    // Duration (required)
    durationEdit = new QSpinBox();
    durationEdit->setRange(1, 36000);
    durationEdit->setSuffix(" seconds");
    durationEdit->setValue(180); // Default 3 minutes
    formLayout->addRow(new QLabel("Duration*:"), durationEdit);
    
    // Album (optional)
    albumEdit = new QLineEdit();
    albumEdit->setPlaceholderText("Optional");
    formLayout->addRow("Album:", albumEdit);
    
    // Genre (optional)
    genreEdit = new QLineEdit();
    genreEdit->setPlaceholderText("Optional");
    formLayout->addRow("Genre:", genreEdit);
    
    // Year (optional)
    yearEdit = new QSpinBox();
    yearEdit->setRange(0, 2100);
    yearEdit->setSpecialValueText("Not set");
    yearEdit->setValue(0);
    formLayout->addRow("Year:", yearEdit);
    
    // Play count (optional)
    playCountEdit = new QSpinBox();
    playCountEdit->setRange(0, 999999);
    playCountEdit->setValue(0);
    formLayout->addRow("Play Count:", playCountEdit);
    
    // Rating (optional)
    ratingEdit = new QDoubleSpinBox();
    ratingEdit->setRange(0.0, 5.0);
    ratingEdit->setSingleStep(0.5);
    ratingEdit->setDecimals(1);
    ratingEdit->setValue(0.0);
    formLayout->addRow("Rating:", ratingEdit);
    
    mainLayout->addLayout(formLayout);
    
    // Required fields note
    QLabel* noteLabel = new QLabel("* Required fields");
    noteLabel->setStyleSheet("color: gray; font-size: 10pt;");
    mainLayout->addWidget(noteLabel);
    
    // Button box
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TrackDialog::validateInput);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    mainLayout->addWidget(buttonBox);
    
    // Set focus
    titleEdit->setFocus();
}

void TrackDialog::validateInput() {
    // Check required fields
    if (titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Title is required.");
        titleEdit->setFocus();
        return;
    }
    
    if (artistEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Artist is required.");
        artistEdit->setFocus();
        return;
    }
    
    accept();
}

std::shared_ptr<Track> TrackDialog::getTrack() const {
    return std::make_shared<Track>(
        titleEdit->text().trimmed().toStdString(),
        artistEdit->text().trimmed().toStdString(),
        durationEdit->value(),
        albumEdit->text().trimmed().toStdString(),
        genreEdit->text().trimmed().toStdString(),
        yearEdit->value(),
        playCountEdit->value(),
        ratingEdit->value()
    );
}