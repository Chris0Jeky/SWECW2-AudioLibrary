#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include "hash_table.h"

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
