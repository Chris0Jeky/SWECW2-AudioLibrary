#pragma once

#include <string>
#include <chrono>
#include <optional>
#include <vector>

namespace audio_library {

class Track {
public:
    // Constructors
    Track() = default;
    Track(std::string title, std::string artist, std::chrono::seconds duration);
    Track(std::string title, std::string artist, int duration_seconds);
    
    // Rule of 5
    Track(const Track&) = default;
    Track(Track&&) = default;
    Track& operator=(const Track&) = default;
    Track& operator=(Track&&) = default;
    ~Track() = default;
    
    // Getters
    [[nodiscard]] const std::string& title() const noexcept { return title_; }
    [[nodiscard]] const std::string& artist() const noexcept { return artist_; }
    [[nodiscard]] std::chrono::seconds duration() const noexcept { return duration_; }
    [[nodiscard]] int duration_seconds() const noexcept { return static_cast<int>(duration_.count()); }
    
    // Additional metadata (for future expansion)
    [[nodiscard]] const std::string& album() const noexcept { return album_; }
    [[nodiscard]] const std::string& genre() const noexcept { return genre_; }
    [[nodiscard]] int year() const noexcept { return year_; }
    [[nodiscard]] uint64_t play_count() const noexcept { return play_count_; }
    [[nodiscard]] double rating() const noexcept { return rating_; }
    
    // Setters for optional metadata
    void set_album(std::string album) { album_ = std::move(album); }
    void set_genre(std::string genre) { genre_ = std::move(genre); }
    void set_year(int year) { year_ = year; }
    void increment_play_count() { ++play_count_; }
    void set_rating(double rating);
    
    // Utility methods
    [[nodiscard]] std::string format_duration() const;
    [[nodiscard]] bool matches_query(const std::string& query) const;
    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] std::string to_csv() const;
    [[nodiscard]] std::string to_json() const;
    
    // Comparison operators
    bool operator==(const Track& other) const noexcept;
    bool operator!=(const Track& other) const noexcept { return !(*this == other); }
    bool operator<(const Track& other) const noexcept;
    
    // Static factory methods
    static std::optional<Track> from_csv_line(const std::string& line);
    static std::optional<Track> from_json(const std::string& json);
    
private:
    std::string title_;
    std::string artist_;
    std::chrono::seconds duration_{0};
    
    // Additional metadata
    std::string album_;
    std::string genre_;
    int year_{0};
    uint64_t play_count_{0};
    double rating_{0.0}; // 0.0 to 5.0
};

// Hash function for use in unordered containers
struct TrackHash {
    std::size_t operator()(const Track& track) const noexcept;
};

} // namespace audio_library