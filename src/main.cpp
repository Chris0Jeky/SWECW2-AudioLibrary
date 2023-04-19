#include "file_loader.h"
#include "hash_table.h"
#include "track.h"
#include <iostream>
#include <ostream>

int main() {
    HashTable titleHashTable;
    HashTable artistHashTable;

    loadTracksFromFile("tracks.txt", titleHashTable, artistHashTable);

    // Example: Search for a track by title
    std::string titleToSearch = "Some Track Title";
    Track* track = titleHashTable.searchTrack(titleToSearch, true);
    if (track) {
        std::cout << "Found track: " << track->getTitle() << " by " << track->getArtist() << std::endl;
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
    std::cout << "Added new track: " << newTrack.getTitle() << " by " << newTrack.getArtist() << std::endl;

    return 0;
}