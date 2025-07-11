#ifndef TRACKDIALOG_H
#define TRACKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <memory>
#include "audio_library/track.h"

class TrackDialog : public QDialog {
    Q_OBJECT

public:
    explicit TrackDialog(QWidget *parent = nullptr, std::shared_ptr<Track> track = nullptr);
    
    std::shared_ptr<Track> getTrack() const;

private slots:
    void validateInput();

private:
    void setupUi();
    
    // UI elements
    QLineEdit* titleEdit;
    QLineEdit* artistEdit;
    QSpinBox* durationEdit;
    QLineEdit* albumEdit;
    QLineEdit* genreEdit;
    QSpinBox* yearEdit;
    QSpinBox* playCountEdit;
    QDoubleSpinBox* ratingEdit;
    
    // Original track for editing
    std::shared_ptr<Track> originalTrack;
};

#endif // TRACKDIALOG_H