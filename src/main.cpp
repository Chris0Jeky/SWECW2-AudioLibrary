#include "file_loader.h"
#include "hash_table.h"
#include <iostream>
#include <ostream>

int main() {
    std::string fileName = "tracks.txt";
    int tableSize = 1000000; // Choose an appropriate size for the hash tables

    HashTable titleHashTable(tableSize);
    HashTable artistHashTable(tableSize);

    loadTracksFromFile(fileName, titleHashTable, artistHashTable);

    // Perform desired operations, such as searching by title or artist, removing tracks, etc.

    // Print the contents of the hash tables
    std::cout << "Title hash table:" << std::endl;
    titleHashTable.printHashTable();
    std::cout << "Artist hash table:" << std::endl;
    artistHashTable.printHashTable();

    return 0;
}
