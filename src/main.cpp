#include "file_loader.h"
#include "hash_table.h"
#include "track.h"
#include <iostream>
#include <ostream>

int main(int argc, char* argv[]) {
    std::string fileName;

    if (argc == 1) {
        std::cout << "You can insert the name of the file you wish to work with or you can specify"
                     " the path to that file and the name of the file." << std::endl;
        fileName = "tracks.txt";
    } else if (argc == 2) {
        fileName = argv[1];
    } else if (argc == 3) {
        std::string filePath = argv[1];
        std::string file = argv[2];

        if (filePath.back() != '/' && filePath.back() != '\\') {
            filePath += "/";
        }
        fileName = filePath + file;
    } else {
        std::cerr << "Usage: " << argv[0] << " [<path> <filename>]" << std::endl;
        return 1;
    }

    HashTable titleHashTable;
    HashTable artistHashTable;

    loadTracksFromFile(fileName, titleHashTable, artistHashTable);

    // Example: Search for a track by title
    std::string titleToSearch = "Some Track Title";
    Track* track = titleHashTable.searchTrack(titleToSearch, true);
    if (track) {
        std::cout << "Found track: " << track->title << " by " << track->artist << std::endl;
    } else {
        std::cout << "Track not found" << std::endl;
    }

    // Example: Remove a track by artist
    std::string artistToRemove = "Some Artist Name";
    if (artistHashTable.removeTrack(artistToRemove, false)) {
        std::cout << "Removed track by " << artistToRemove << std::endl;
    } else {
        std::cout << "No track found for artist " << artistToRemove << std::endl;
    }

    // Example: Add a new track
    Track newTrack("New Track Title", "New Track Artist", 180);
    titleHashTable.insertTrack(newTrack, true);
    artistHashTable.insertTrack(newTrack, false);
    std::cout << "Added new track: " << newTrack.title << " by " << newTrack.artist << std::endl;

    return 0;
}
