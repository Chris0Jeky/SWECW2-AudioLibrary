// HashTable class definition
#pragma once
#include <vector>
#include <list>
#include "track.h"
#include <string>

class HashTable {

public:
    unsigned int testHashFunction(const std::string& key) const;

public:
    int size;
    std::vector<Track*> searchTrack(const std::string& key, bool useTitleAsKey);

    // Constructors and destructor for HashTable class
    HashTable(int size);

    HashTable();
    ~HashTable();

    // Methods to insert and remove tracks in the hash table
    void insertTrack(const Track& track, bool useTitleAsKey);
    bool removeTrack(const std::string& key, bool useTitleAsKey);
    void printHashTable() const;
    const std::vector<std::list<Track>>& getBuckets() const;


private:
    std::vector<std::list<Track>> buckets;
    unsigned int hashFunction(const std::string& key) const;
    static const unsigned int TABLE_SIZE = 100000;
};
