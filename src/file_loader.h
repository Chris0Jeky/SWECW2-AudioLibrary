// File loader functions
#pragma once
#include <string>
#include "hash_table.h"

// Function to load tracks from a file and insert them into hash tables
void loadTracksFromFile(const std::string& fileName, HashTable& titleHashTable, HashTable& artistHashTable);
