// hash_table.h
#pragma once
#include <vector>
#include <list>
#include "track.h"

class HashTable {
public:
    int size;
    std::vector<std::list<Track>> buckets;

    HashTable(int size);

    void insertTrack(const Track& track, bool useTitleAsKey);

    void printHashTable();

private:
    unsigned int hashFunction(const std::string& key);
};
