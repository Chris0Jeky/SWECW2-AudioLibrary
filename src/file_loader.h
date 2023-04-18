// file_loader.h
#pragma once
#include <string>
#include "hash_table.h"

void loadTracksFromFile(const std::string& fileName, HashTable& titleHashTable, HashTable& artistHashTable);
