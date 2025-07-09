#include "audio_library/music_library.h"
#include "audio_library/file_io.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <sstream>

namespace fs = std::filesystem;
using namespace audio_library;

class MusicLibraryApp {
public:
    MusicLibraryApp() = default;
    
    void run(int argc, char* argv[]) {
        print_welcome();
        
        // Load initial file if provided
        if (argc > 1) {
            std::string filename = argv[1];
            if (argc > 2) {
                // Handle path and filename separately
                fs::path dir(argv[1]);
                fs::path file(argv[2]);
                filename = (dir / file).string();
            }
            
            std::cout << "Loading tracks from: " << filename << "\n";
            if (library_.import_from_csv(filename)) {
                std::cout << "Successfully loaded " << library_.size() << " tracks.\n\n";
            } else {
                std::cerr << "Failed to load tracks from file.\n\n";
            }
        }
        
        // Main menu loop
        while (running_) {
            display_menu();
            process_command();
        }
        
        print_goodbye();
    }
    
private:
    MusicLibrary library_;
    bool running_ = true;
    
    void print_welcome() {
        std::cout << R"(
╔════════════════════════════════════════════════════╗
║           Audio Library Management System          ║
║                    Version 2.0                     ║
╚════════════════════════════════════════════════════╝
)" << "\n";
    }
    
    void print_goodbye() {
        std::cout << "\nThank you for using Audio Library. Goodbye!\n";
    }
    
    void display_menu() {
        auto stats = library_.get_statistics();
        
        std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        std::cout << "Library Statistics: " 
                  << stats.total_tracks << " tracks, "
                  << stats.total_artists << " artists, "
                  << format_duration(stats.total_duration) << " total\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
        
        std::cout << "MAIN MENU:\n";
        std::cout << "  1. Search tracks\n";
        std::cout << "  2. List all tracks\n";
        std::cout << "  3. Add track manually\n";
        std::cout << "  4. Remove track\n";
        std::cout << "  5. Import tracks from file\n";
        std::cout << "  6. Export tracks to file\n";
        std::cout << "  7. Browse by category\n";
        std::cout << "  8. Top tracks\n";
        std::cout << "  9. Advanced search\n";
        std::cout << "  0. Exit\n\n";
        std::cout << "Enter your choice: ";
    }
    
    void process_command() {
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) return;
        
        try {
            int choice = std::stoi(input);
            
            switch (choice) {
                case 1: search_tracks(); break;
                case 2: list_all_tracks(); break;
                case 3: add_track_manually(); break;
                case 4: remove_track(); break;
                case 5: import_tracks(); break;
                case 6: export_tracks(); break;
                case 7: browse_by_category(); break;
                case 8: show_top_tracks(); break;
                case 9: advanced_search(); break;
                case 0: running_ = false; break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a number.\n";
        }
    }
    
    void search_tracks() {
        std::cout << "\nSEARCH TRACKS\n";
        std::cout << "Enter search query: ";
        
        std::string query;
        std::getline(std::cin, query);
        
        if (query.empty()) return;
        
        auto results = library_.search(query);
        display_tracks(results, "Search Results");
    }
    
    void list_all_tracks() {
        std::cout << "\nSort by:\n";
        std::cout << "  1. Title\n";
        std::cout << "  2. Artist\n";
        std::cout << "  3. Duration\n";
        std::cout << "  4. Year\n";
        std::cout << "  5. Rating\n";
        std::cout << "  6. Play count\n";
        std::cout << "  0. No sorting\n";
        std::cout << "Choice: ";
        
        std::string input;
        std::getline(std::cin, input);
        
        std::vector<MusicLibrary::TrackPtr> tracks;
        
        try {
            int choice = input.empty() ? 0 : std::stoi(input);
            
            switch (choice) {
                case 1:
                    tracks = library_.get_tracks_sorted(
                        [](const Track& a, const Track& b) { return a.title() < b.title(); });
                    break;
                case 2:
                    tracks = library_.get_tracks_sorted(
                        [](const Track& a, const Track& b) { return a.artist() < b.artist(); });
                    break;
                case 3:
                    tracks = library_.get_tracks_sorted(
                        [](const Track& a, const Track& b) { return a.duration() < b.duration(); });
                    break;
                case 4:
                    tracks = library_.get_tracks_sorted(
                        [](const Track& a, const Track& b) { return a.year() < b.year(); });
                    break;
                case 5:
                    tracks = library_.get_tracks_sorted(
                        [](const Track& a, const Track& b) { return a.rating() > b.rating(); });
                    break;
                case 6:
                    tracks = library_.get_tracks_sorted(
                        [](const Track& a, const Track& b) { return a.play_count() > b.play_count(); });
                    break;
                default:
                    tracks = library_.get_all_tracks();
            }
        } catch (...) {
            tracks = library_.get_all_tracks();
        }
        
        display_tracks(tracks, "All Tracks");
    }
    
    void add_track_manually() {
        std::cout << "\nADD NEW TRACK\n";
        
        std::string title, artist, duration_str;
        
        std::cout << "Title: ";
        std::getline(std::cin, title);
        
        std::cout << "Artist: ";
        std::getline(std::cin, artist);
        
        std::cout << "Duration (seconds): ";
        std::getline(std::cin, duration_str);
        
        try {
            int duration = std::stoi(duration_str);
            Track track(title, artist, duration);
            
            // Optional fields
            std::cout << "Album (optional): ";
            std::string album;
            std::getline(std::cin, album);
            if (!album.empty()) track.set_album(album);
            
            std::cout << "Genre (optional): ";
            std::string genre;
            std::getline(std::cin, genre);
            if (!genre.empty()) track.set_genre(genre);
            
            std::cout << "Year (optional): ";
            std::string year_str;
            std::getline(std::cin, year_str);
            if (!year_str.empty()) {
                try {
                    track.set_year(std::stoi(year_str));
                } catch (...) {}
            }
            
            if (library_.add_track(std::move(track))) {
                std::cout << "\nTrack added successfully!\n";
            } else {
                std::cout << "\nFailed to add track (duplicate?).\n";
            }
        } catch (const std::exception& e) {
            std::cout << "\nInvalid input: " << e.what() << "\n";
        }
    }
    
    void remove_track() {
        std::cout << "\nREMOVE TRACK\n";
        std::cout << "Enter exact title: ";
        
        std::string title;
        std::getline(std::cin, title);
        
        std::cout << "Enter exact artist: ";
        std::string artist;
        std::getline(std::cin, artist);
        
        if (library_.remove_track(title, artist)) {
            std::cout << "\nTrack removed successfully!\n";
        } else {
            std::cout << "\nTrack not found.\n";
            
            // Offer to search for similar tracks
            auto results = library_.search(title + " " + artist);
            if (!results.empty()) {
                std::cout << "\nDid you mean one of these?\n";
                display_tracks(results, "Similar Tracks", 5);
            }
        }
    }
    
    void import_tracks() {
        std::cout << "\nIMPORT TRACKS\n";
        std::cout << "Enter filename: ";
        
        std::string filename;
        std::getline(std::cin, filename);
        
        if (filename.empty()) return;
        
        // Auto-detect format
        auto format = FileIO::detect_file_format(filename);
        
        bool success = false;
        size_t prev_size = library_.size();
        
        if (format == "json") {
            success = library_.import_from_json(filename);
        } else {
            success = library_.import_from_csv(filename);
        }
        
        if (success) {
            size_t added = library_.size() - prev_size;
            std::cout << "\nSuccessfully imported " << added << " tracks.\n";
        } else {
            std::cout << "\nFailed to import tracks.\n";
        }
    }
    
    void export_tracks() {
        std::cout << "\nEXPORT TRACKS\n";
        std::cout << "Export format:\n";
        std::cout << "  1. CSV\n";
        std::cout << "  2. JSON\n";
        std::cout << "Choice: ";
        
        std::string format_choice;
        std::getline(std::cin, format_choice);
        
        std::cout << "Enter filename: ";
        std::string filename;
        std::getline(std::cin, filename);
        
        if (filename.empty()) return;
        
        bool success = false;
        
        try {
            int choice = std::stoi(format_choice);
            if (choice == 2) {
                success = library_.export_to_json(filename);
            } else {
                success = library_.export_to_csv(filename);
            }
        } catch (...) {
            success = library_.export_to_csv(filename);
        }
        
        if (success) {
            std::cout << "\nSuccessfully exported " << library_.size() << " tracks.\n";
        } else {
            std::cout << "\nFailed to export tracks.\n";
        }
    }
    
    void browse_by_category() {
        std::cout << "\nBROWSE BY:\n";
        std::cout << "  1. Artist\n";
        std::cout << "  2. Album\n";
        std::cout << "  3. Genre\n";
        std::cout << "  4. Year\n";
        std::cout << "Choice: ";
        
        std::string choice_str;
        std::getline(std::cin, choice_str);
        
        try {
            int choice = std::stoi(choice_str);
            
            switch (choice) {
                case 1: browse_artists(); break;
                case 2: browse_albums(); break;
                case 3: browse_genres(); break;
                case 4: browse_years(); break;
                default:
                    std::cout << "Invalid choice.\n";
            }
        } catch (...) {
            std::cout << "Invalid input.\n";
        }
    }
    
    void browse_artists() {
        auto artists = library_.get_all_artists();
        std::sort(artists.begin(), artists.end());
        
        std::cout << "\nARTISTS (" << artists.size() << "):\n";
        for (size_t i = 0; i < artists.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << artists[i] << "\n";
        }
        
        std::cout << "\nEnter artist number to view tracks (0 to cancel): ";
        std::string input;
        std::getline(std::cin, input);
        
        try {
            size_t index = std::stoul(input);
            if (index > 0 && index <= artists.size()) {
                auto tracks = library_.find_by_artist(artists[index - 1]);
                display_tracks(tracks, "Tracks by " + artists[index - 1]);
            }
        } catch (...) {}
    }
    
    void browse_albums() {
        auto albums = library_.get_all_albums();
        std::sort(albums.begin(), albums.end());
        
        std::cout << "\nALBUMS (" << albums.size() << "):\n";
        for (size_t i = 0; i < albums.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << albums[i] << "\n";
        }
        
        std::cout << "\nEnter album number to view tracks (0 to cancel): ";
        std::string input;
        std::getline(std::cin, input);
        
        try {
            size_t index = std::stoul(input);
            if (index > 0 && index <= albums.size()) {
                auto tracks = library_.find_by_album(albums[index - 1]);
                display_tracks(tracks, "Tracks from " + albums[index - 1]);
            }
        } catch (...) {}
    }
    
    void browse_genres() {
        auto genres = library_.get_all_genres();
        std::sort(genres.begin(), genres.end());
        
        std::cout << "\nGENRES (" << genres.size() << "):\n";
        for (size_t i = 0; i < genres.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << genres[i] << "\n";
        }
        
        std::cout << "\nEnter genre number to view tracks (0 to cancel): ";
        std::string input;
        std::getline(std::cin, input);
        
        try {
            size_t index = std::stoul(input);
            if (index > 0 && index <= genres.size()) {
                auto tracks = library_.find_by_genre(genres[index - 1]);
                display_tracks(tracks, genres[index - 1] + " Tracks");
            }
        } catch (...) {}
    }
    
    void browse_years() {
        std::cout << "\nEnter year range (e.g., '2010 2020'): ";
        std::string input;
        std::getline(std::cin, input);
        
        std::istringstream iss(input);
        int start_year, end_year;
        
        if (iss >> start_year >> end_year) {
            auto tracks = library_.get_tracks_by_year_range(start_year, end_year);
            display_tracks(tracks, "Tracks from " + std::to_string(start_year) + 
                                  " to " + std::to_string(end_year));
        } else {
            std::cout << "Invalid input format.\n";
        }
    }
    
    void show_top_tracks() {
        std::cout << "\nTOP TRACKS BY:\n";
        std::cout << "  1. Play count\n";
        std::cout << "  2. Rating\n";
        std::cout << "Choice: ";
        
        std::string choice_str;
        std::getline(std::cin, choice_str);
        
        try {
            int choice = std::stoi(choice_str);
            
            if (choice == 1) {
                auto tracks = library_.get_most_played_tracks(20);
                display_tracks(tracks, "Most Played Tracks");
            } else if (choice == 2) {
                auto tracks = library_.get_top_rated_tracks(20);
                display_tracks(tracks, "Top Rated Tracks");
            }
        } catch (...) {
            std::cout << "Invalid input.\n";
        }
    }
    
    void advanced_search() {
        std::cout << "\nADVANCED SEARCH\n";
        std::cout << "Search query: ";
        
        std::string query;
        std::getline(std::cin, query);
        
        if (query.empty()) return;
        
        SearchEngine::SearchOptions options;
        
        std::cout << "Search mode:\n";
        std::cout << "  1. Substring (default)\n";
        std::cout << "  2. Exact match\n";
        std::cout << "  3. Prefix match\n";
        std::cout << "  4. Fuzzy match\n";
        std::cout << "  5. Regular expression\n";
        std::cout << "Choice: ";
        
        std::string mode_str;
        std::getline(std::cin, mode_str);
        
        try {
            int mode = mode_str.empty() ? 1 : std::stoi(mode_str);
            switch (mode) {
                case 2: options.mode = SearchEngine::SearchMode::EXACT; break;
                case 3: options.mode = SearchEngine::SearchMode::PREFIX; break;
                case 4: options.mode = SearchEngine::SearchMode::FUZZY; break;
                case 5: options.mode = SearchEngine::SearchMode::REGEX; break;
                default: options.mode = SearchEngine::SearchMode::SUBSTRING;
            }
        } catch (...) {}
        
        std::cout << "Case sensitive? (y/N): ";
        std::string case_input;
        std::getline(std::cin, case_input);
        options.case_sensitive = !case_input.empty() && 
                                (case_input[0] == 'y' || case_input[0] == 'Y');
        
        auto results = library_.search_advanced(query, nullptr);
        display_tracks(results, "Advanced Search Results");
    }
    
    void display_tracks(const std::vector<MusicLibrary::TrackPtr>& tracks,
                       const std::string& title,
                       size_t limit = 0) {
        if (tracks.empty()) {
            std::cout << "\nNo tracks found.\n";
            return;
        }
        
        size_t count = limit > 0 ? std::min(tracks.size(), limit) : tracks.size();
        
        std::cout << "\n" << title << " (" << count;
        if (count < tracks.size()) {
            std::cout << " of " << tracks.size();
        }
        std::cout << "):\n";
        std::cout << "────────────────────────────────────────────────────────────────\n";
        
        // Table header
        std::cout << std::left
                  << std::setw(3) << "#"
                  << std::setw(30) << "Title"
                  << std::setw(25) << "Artist"
                  << std::setw(10) << "Duration"
                  << "Rating\n";
        std::cout << "────────────────────────────────────────────────────────────────\n";
        
        // Display tracks
        for (size_t i = 0; i < count; ++i) {
            if (!tracks[i]) continue;
            
            const auto& track = *tracks[i];
            
            std::cout << std::left
                      << std::setw(3) << (i + 1)
                      << std::setw(30) << truncate(track.title(), 29)
                      << std::setw(25) << truncate(track.artist(), 24)
                      << std::setw(10) << track.format_duration();
            
            if (track.rating() > 0) {
                std::cout << std::fixed << std::setprecision(1) 
                          << track.rating() << "/5.0";
            }
            
            std::cout << "\n";
        }
        
        if (count < tracks.size()) {
            std::cout << "... and " << (tracks.size() - count) << " more tracks.\n";
        }
    }
    
    static std::string format_duration(std::chrono::seconds duration) {
        auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
        duration -= hours;
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
        
        std::ostringstream oss;
        if (hours.count() > 0) {
            oss << hours.count() << "h " << minutes.count() << "m";
        } else {
            oss << minutes.count() << " minutes";
        }
        return oss.str();
    }
    
    static std::string truncate(const std::string& str, size_t max_length) {
        if (str.length() <= max_length) {
            return str;
        }
        return str.substr(0, max_length - 3) + "...";
    }
};

int main(int argc, char* argv[]) {
    try {
        MusicLibraryApp app;
        app.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}