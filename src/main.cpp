// main.cpp
#include "hash_table.h"
#include "file_loader.h"

int main() {
    HashTable titleHashTable(100);
    HashTable artistHashTable(100);

    std::string fileName = "tracks.txt";
    loadTracksFromFile(fileName, titleHashTable, artistHashTable);

    return 0;
}
