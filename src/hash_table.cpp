// hash_table.cpp
#include "hash_table.h"

HashTable::HashTable(int size) : size(size) {
    buckets.resize(size);
}

void HashTable::insertTrack(const Track& track, bool useTitleAsKey) {
    unsigned int index = useTitleAsKey ? hashFunction(track.title) : hashFunction(track.artist);
    index %= size;

    for (const Track& existingTrack : buckets[index]) {
        if ((useTitleAsKey && existingTrack.title == track.title) ||
            (!useTitleAsKey && existingTrack.artist == track.artist && existingTrack.title == track.title)) {
            return; // track already exists in the library
        }
    }
    buckets[index].push_back(track);
}

unsigned int HashTable::hashFunction(const std::string& key) {
    unsigned int hashFunction(const std::string& key) {
        unsigned int hash = 0;
        for (char c : key) {
            hash = 31 * hash + c;
        }
        return hash;
    }
}
