#include "file_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

void loadTracksFromFile(const std::string& fileName, HashTable& titleHashTable, HashTable& artistHashTable) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << fileName << std::endl;
        return;
    }

    std::string line;
    // Read tracks from the file and insert them into the hash tables
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string title, artist, durationStr;
        std::getline(ss, title, '\t');
        std::getline(ss, artist, '\t');
        std::getline(ss, durationStr, '\t');

        int duration = std::stoi(durationStr);

        Track track(title, artist, duration);
        titleHashTable.insertTrack(track, true);
        artistHashTable.insertTrack(track, false);
    }
    file.close();
}
