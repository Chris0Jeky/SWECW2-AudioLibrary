#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include "hash_table.h"
#include "track.h"
#include "file_loader.h"
#include "hash_table.h"

HashTable setup_test_hash_table() {
    HashTable titleHashTable;
    HashTable artistHashTable;

    loadTracksFromFile("tracks.txt", titleHashTable, artistHashTable);

    return titleHashTable;
}

TEST_CASE("Inserting a track") {
    HashTable titleHashTable = setup_test_hash_table();

    Track newTrack("Test Track", "Test Artist", 180);
    titleHashTable.insertTrack(newTrack, true);

    Track* track = titleHashTable.searchTrack("Test Track", true);
    REQUIRE(track != nullptr);
    REQUIRE(track->title == "Test Track");
    REQUIRE(track->artist == "Test Artist");
    REQUIRE(track->duration == 180);
}

TEST_CASE("Searching for a track by title") {
    HashTable titleHashTable = setup_test_hash_table();

    Track* track = titleHashTable.searchTrack("Jump For Joy", true);
    REQUIRE(track != nullptr);
    REQUIRE(track->title == "Jump For Joy");
    REQUIRE(track->artist == "New York Trio");
    REQUIRE(track->duration == 286);
}

TEST_CASE("Removing a track by title") {
    HashTable titleHashTable = setup_test_hash_table();

    bool removed = titleHashTable.removeTrack("Jump For Joy", true);
    REQUIRE(removed == true);

    Track* track = titleHashTable.searchTrack("Jump For Joy", true);
    REQUIRE(track == nullptr);
}


TEST_CASE("Inserting and searching tracks", "[hash_table]") {
    HashTable titleHashTable;

    Track track1("Title 1", "Artist 1", 100);
    Track track2("Title 2", "Artist 2", 200);

    titleHashTable.insertTrack(track1, true);
    titleHashTable.insertTrack(track2, true);

    SECTION("Searching existing tracks") {
        Track* result1 = titleHashTable.searchTrack("Title 1", true);
        REQUIRE(result1 != nullptr);
        REQUIRE(result1->title == "Title 1");
        REQUIRE(result1->artist == "Artist 1");
        REQUIRE(result1->duration == 100);

        Track* result2 = titleHashTable.searchTrack("Title 2", true);
        REQUIRE(result2 != nullptr);
        REQUIRE(result2->title == "Title 2");
        REQUIRE(result2->artist == "Artist 2");
        REQUIRE(result2->duration == 200);
    }

    SECTION("Searching non-existent track") {
        Track* result3 = titleHashTable.searchTrack("Non-existent title", true);
        REQUIRE(result3 == nullptr);
    }
}


