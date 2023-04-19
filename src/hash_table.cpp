#include "hash_table.h"
#include <string>
#include <iostream>
#include <ostream>

HashTable::HashTable(int size) : size(size) {
    buckets.resize(size);
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

unsigned int HashTable::hashFunction(const std::string& key) {
    unsigned int hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return hash % size;
}

void HashTable::printHashTable() {
    for (int i = 0; i < size; i++) {
        std::cout << "Bucket " << i << ": ";
        for (const Track& track : buckets[i]) {
            std::cout << "[" << track.title << ", " << track.artist << ", " << track.duration << "] -> ";
        }
        std::cout << "nullptr" << std::endl;
    }
}


