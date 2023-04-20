#pragma once
#include <string>

class Track {
public:
    std::string title;
    std::string artist;
    int duration;

    Track(const std::string& title, const std::string& artist, int duration);
};
