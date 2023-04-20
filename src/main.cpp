#include "file_loader.h"
#include "hash_table.h"
#include "track.h"
#include <iostream>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <limits>

void saveTracksToFile(const std::string& fileName, const HashTable& titleHashTable) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << fileName << std::endl;
        return;
    }

    for (int i = 0; i < titleHashTable.size; ++i) {
        for (const Track& track : titleHashTable.buckets[i]) {
            file << track.title << '\t' << track.artist << '\t' << track.duration << '\n';
        }
    }

    file.close();
}

std::string toLowerCase(const std::string& input) {
    std::string result(input);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

int main(int argc, char* argv[]) {
    std::string fileName;

    if (argc == 1) {
        std::cout << "You can insert the name of the file you wish to work with or you can specify"
                     " the path to that file and the name of the file." << std::endl;
        fileName = "tracks.txt";
    } else if (argc == 2) {
        fileName = argv[1];
    } else if (argc == 3) {
        std::string filePath = argv[1];
        std::string file = argv[2];

        if (filePath.back() != '/' && filePath.back() != '\\') {
            filePath += "/";
        }
        fileName = filePath + file;
    } else {
        std::cerr << "Usage: " << argv[0] << " [<path> <filename>]" << std::endl;
        return 1;
    }

    HashTable titleHashTable;
    HashTable artistHashTable;

    loadTracksFromFile(fileName, titleHashTable, artistHashTable);

    std::string command;

    while (true) {
        std::cout << "Enter command (search, import, export, remove, quit): ";
        std::getline(std::cin, command);
        command = toLowerCase(command);

        if (command == "search") {
            std::string type, key;
            while (true) {
                std::cout << "Enter search type (track or artist): ";
                std::cin >> type;
                type = toLowerCase(type);

                if (type == "track" || type == "artist") {
                    break;
                } else {
                    std::cout << "Invalid search type. Please enter 'track' or 'artist'." << std::endl;
                }
            }

            std::cout << "Enter the title or artist name: ";
            std::cin.ignore();
            std::getline(std::cin, key);


            bool searchByTitle = type == "track";
            HashTable& searchHashTable = searchByTitle ? titleHashTable : artistHashTable;
            Track* track = searchHashTable.searchTrack(key, searchByTitle);


            if (track) {
                std::cout << "Found track: " << track->title << " by " << track->artist << std::endl;
            } else {
                std::cout << "Track not found" << std::endl;
            }
        } else if (command == "import") {
            std::string file;
            std::cout << "Enter file name: ";
            std::cin >> file;
            std::cin.ignore();

            loadTracksFromFile(file, titleHashTable, artistHashTable);
        } else if (command == "export") {
            std::string file;
            std::cout << "Enter file name: ";
            std::cin >> file;
            std::cin.ignore();

            saveTracksToFile(file, titleHashTable);
        } else if (command == "remove") {
            std::string title;
            std::cout << "Enter the title of the track to remove: ";
            std::cin.ignore();
            std::getline(std::cin, title);

            if (titleHashTable.removeTrack(title, true)) {
                std::cout << "Track removed" << std::endl;
            } else {
                std::cout << "Track not found" << std::endl;
            }
        } else if (command == "quit") {
            break;
        } else {
            std::cout << "Invalid command. Press 'enter' to continue." << std::endl;
            std::string exitOrRetry;
            std::getline(std::cin, exitOrRetry);
            if (toLowerCase(exitOrRetry) == "exit") {
                continue;
            }
        }
    }
    return 0;
}
