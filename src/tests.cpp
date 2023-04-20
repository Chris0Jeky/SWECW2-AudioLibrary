#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include "hash_table.h"
#include "track.h"
#include "file_loader.h"

HashTable setup_test_hash_table() {
    HashTable titleHashTable;
    HashTable artistHashTable;

    loadTracksFromFile("tracks.txt", titleHashTable, artistHashTable);

    return titleHashTable;
}


// Test the hash function with different strings and expect different hash values
TEST_CASE("Test hashFunction", "[HashTable]") {
    HashTable table;

    REQUIRE(table.testHashFunction("Track1") != table.testHashFunction("Track2"));
    REQUIRE(table.testHashFunction("Artist1") != table.testHashFunction("Artist2"));
}

// Test inserting a track and then searching for it by title
TEST_CASE("Inserting a track") {
    HashTable titleHashTable = setup_test_hash_table();

    Track newTrack("Test Track", "Test Artist", 180);
    titleHashTable.insertTrack(newTrack, true);

    std::vector<Track*> tracks= titleHashTable.searchTrack("Test Track", true);
    REQUIRE(!tracks.empty());
    REQUIRE(tracks[0]->title == "Test Track");
    REQUIRE(tracks[0]->artist == "Test Artist");
    REQUIRE(tracks[0]->duration == 180);
}


// Test inserting and searching for tracks by title and artist
TEST_CASE("Test insertTrack and searchTrack", "[HashTable]") {
    HashTable titleTable;
    HashTable artistTable;

    Track track1("Title1", "Artist1", 180);
    Track track2("Title2", "Artist2", 240);

    titleTable.insertTrack(track1, true);
    artistTable.insertTrack(track1, false);

    titleTable.insertTrack(track2, true);
    artistTable.insertTrack(track2, false);

    // Test searching for a track by its title
    SECTION("Test search by title") {
        std::vector<Track*> foundTracks = titleTable.searchTrack("Title1", true);
        REQUIRE(foundTracks.size() == 1);
        REQUIRE(foundTracks[0]->title == "Title1");
        REQUIRE(foundTracks[0]->artist == "Artist1");
        REQUIRE(foundTracks[0]->duration == 180);
    }

    // Test searching for a track by its artist
    SECTION("Test search by artist") {
        std::vector<Track*> foundTracks = artistTable.searchTrack("Artist2", false);
        REQUIRE(foundTracks.size() == 1);
        REQUIRE(foundTracks[0]->title == "Title2");
        REQUIRE(foundTracks[0]->artist == "Artist2");
        REQUIRE(foundTracks[0]->duration == 240);
    }
}

// Test searching for a track by title
TEST_CASE("Searching for a track by title") {
    HashTable titleHashTable = setup_test_hash_table();

    std::vector<Track*> tracks = titleHashTable.searchTrack("Jump For Joy", true);
    REQUIRE(!tracks.empty());
    REQUIRE(tracks[0]->title == "Jump For Joy");
    REQUIRE(tracks[0]->artist == "New York Trio");
    REQUIRE(tracks[0]->duration == 286);
}

// Test removing a track by title and then verifying it is not in the hash table
TEST_CASE("Removing a track by title") {
    HashTable titleHashTable = setup_test_hash_table();

    bool removed = titleHashTable.removeTrack("Jump For Joy", true);
    REQUIRE(removed == true);

    std::vector<Track*> tracks = titleHashTable.searchTrack("Jump For Joy", true);
    REQUIRE(tracks.empty());
}


// Test inserting, searching, and removing tracks
TEST_CASE("Inserting and searching tracks", "[hash_table]") {
    HashTable titleHashTable;

    Track track1("Title 1", "Artist 1", 100);
    Track track2("Title 2", "Artist 2", 200);

    titleHashTable.insertTrack(track1, true);
    titleHashTable.insertTrack(track2, true);

    // Test searching for existing tracks by their titles
    SECTION("Searching existing tracks") {
        std::vector<Track*> result1 = titleHashTable.searchTrack("Title 1", true);
        REQUIRE(result1.size() == 1);
        REQUIRE(result1[0]->title == "Title 1");
        REQUIRE(result1[0]->artist == "Artist 1");
        REQUIRE(result1[0]->duration == 100);

        std::vector<Track*> result2 = titleHashTable.searchTrack("Title 2", true);
        REQUIRE(result2.size() == 1);
        REQUIRE(result2[0]->title == "Title 2");
        REQUIRE(result2[0]->artist == "Artist 2");
        REQUIRE(result2[0]->duration == 200);
    }

    // Test searching for a non-existent track by title
    SECTION("Searching non-existent track") {
        std::vector<Track*> result3 = titleHashTable.searchTrack("Non-existent title", true);
        REQUIRE(result3.empty());
    }
}

// Test removing tracks by title and artist
TEST_CASE("Test removeTrack", "[HashTable]") {
    HashTable titleTable;
    HashTable artistTable;

    Track track1("Title1", "Artist1", 180);
    Track track2("Title2", "Artist2", 240);

    titleTable.insertTrack(track1, true);
    artistTable.insertTrack(track1, false);

    titleTable.insertTrack(track2, true);
    artistTable.insertTrack(track2, false);

    // Test removing a track by its title
    SECTION("Test remove by title") {
        REQUIRE(titleTable.removeTrack("Title1", true));
        std::vector<Track*> foundTracks = titleTable.searchTrack("Title1", true);
        REQUIRE(foundTracks.empty());
    }

    // Test removing a track by its artist
    SECTION("Test remove by artist") {
        REQUIRE(artistTable.removeTrack("Artist2", false));
        std::vector<Track*> foundTracks = artistTable.searchTrack("Artist2", false);
        REQUIRE(foundTracks.empty());
    }
}


