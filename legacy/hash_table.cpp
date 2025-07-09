// HashTable class implementation
#include "hash_table.h"
#include <string>
#include <iostream>
#include <ostream>

HashTable::HashTable() : size(TABLE_SIZE) {
    buckets.resize(TABLE_SIZE);
}

HashTable::~HashTable() {}

HashTable::HashTable(int size) : size(size) {
    buckets.resize(size);
}

unsigned int HashTable::testHashFunction(const std::string& key) const {
    return hashFunction(key);
}


const std::vector<std::list<Track>>& HashTable::getBuckets() const {
    return buckets;
}


void HashTable::insertTrack(const Track& track, bool useTitleAsKey) {
    std::string key = useTitleAsKey ? track.title : track.artist;
    unsigned int index = hashFunction(key);

    for (const Track& t : buckets[index]) {
        if (t.title == track.title && t.artist == track.artist) {
            return; // Track already exists, do not insert.
        }
    }

    buckets[index].push_back(track);
}

bool HashTable::removeTrack(const std::string& key, bool useTitleAsKey) {
    unsigned int index = hashFunction(key);

    for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it) {
        if ((useTitleAsKey && it->title == key) || (!useTitleAsKey && it->artist == key)) {
            buckets[index].erase(it);
            return true;
        }
    }

    return false;
}

void HashTable::printHashTable() const {
    for (int i = 0; i < size; i++) {
        std::cout << "Bucket " << i << ": ";
        for (const Track& track : buckets[i]) {
            std::cout << "[" << track.title << ", " << track.artist << ", " << track.duration << "] -> ";
        }
        std::cout << "nullptr" << std::endl;
    }
}

unsigned int HashTable::hashFunction(const std::string& key) const {
    unsigned int hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return hash % size;
}

std::vector<Track*> HashTable::searchTrack(const std::string& key, bool useTitleAsKey) {
    unsigned int index = hashFunction(key);
    std::vector<Track*> matchingTracks;

    for (Track& track : buckets[index]) {
        if ((useTitleAsKey && track.title == key) || (!useTitleAsKey && track.artist == key)) {
            matchingTracks.push_back(&track);
        }
    }

    return matchingTracks;
}

