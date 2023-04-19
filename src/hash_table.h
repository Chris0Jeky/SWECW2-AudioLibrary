// hash_table.h
#pragma once
#include <vector>
#include <list>
#include "track.h"
#include <string>

class HashTable {
public:
    int size;
    std::vector<std::list<Track>> buckets;

    HashTable(int size);

    HashTable();
    ~HashTable();


    void insertTrack(const Track& track, bool useTitleAsKey);
    Track* searchTrack(const std::string& key, bool useTitleAsKey);
    bool removeTrack(const std::string& key, bool useTitleAsKey);
    void printHashTable() const;

    void printHashTable();


private:
    struct HashNode {
        Track track;
        HashNode* next;

        HashNode(const Track& track) : track(track), next(nullptr) {}
    };

    unsigned int hashFunction(const std::string& key) const;
    static const unsigned int TABLE_SIZE = 1000;
    HashNode* table[TABLE_SIZE];
};
