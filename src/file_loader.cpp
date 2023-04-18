// file_loader.cpp
#include "file_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

void loadTracksFromFile(const std::string& fileName, HashTable& titleHashTable, HashTable& artistHashTable) {
    std::ifstream inputFile(fileName);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream lineStream(line);
            std::string title, artist;
            int duration;
            std::getline(lineStream, title, '\t');
            std::getline(lineStream, artist, '\t');
            lineStream >> duration;

            Track track(title, artist, duration);
            titleHashTable.insertTrack(track, true);
            artistHashTable.insertTrack(track, false);
        }
        inputFile.close();
    } else {
        std::cerr << "Unable to open file: " << fileName << std::endl;
    }
}
