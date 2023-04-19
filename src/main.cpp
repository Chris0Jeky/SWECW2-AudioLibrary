// main.cpp
#include "hash_table.h"
#include "file_loader.h"

int main() {
    std::string fileName = "tracks.txt";
    int tableSize = 1000;

    HashTable titleHashTable(tableSize);
    HashTable artistHashTable(tableSize);

    loadTracksFromFile(fileName, titleHashTable, artistHashTable);

    // Print the contents of the hash tables
    std::cout << "Title hash table:" << std::endl;
    titleHashTable.printHashTable();
    std::cout << "Artist hash table:" << std::endl;
    artistHashTable.printHashTable();

    return 0;
}
