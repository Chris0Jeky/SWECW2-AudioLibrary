cmake_minimum_required(VERSION 3.16)
project(AudioLibraryGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Source files
set(SOURCES
    main_gui.cpp
    mainwindow.cpp
    trackdialog.cpp
    searchdialog.cpp
    ../src/audio_library/track.cpp
    ../src/audio_library/music_library.cpp
    ../src/audio_library/search_engine.cpp
    ../src/audio_library/file_io.cpp
)

# Header files
set(HEADERS
    mainwindow.h
    trackdialog.h
    searchdialog.h
)

# Create executable
add_executable(AudioLibraryGUI ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(AudioLibraryGUI
    Qt5::Core
    Qt5::Widgets
    pthread
)

# Set output directory
set_target_properties(AudioLibraryGUI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)