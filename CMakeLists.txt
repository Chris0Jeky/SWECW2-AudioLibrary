cmake_minimum_required(VERSION 3.14)
project(AudioLibrary VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(BUILD_EXAMPLES "Build examples" ON)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Find packages
find_package(Threads REQUIRED)

# Include directories
include_directories(include)

# Library sources
set(LIBRARY_SOURCES
    src/audio_library/track.cpp
    src/audio_library/music_library.cpp
    src/audio_library/file_io.cpp
    src/audio_library/search_engine.cpp
)

# Create library
add_library(audio_library ${LIBRARY_SOURCES})
target_link_libraries(audio_library PUBLIC Threads::Threads)

# Main executable
add_executable(music_manager src/main.cpp)
target_link_libraries(music_manager PRIVATE audio_library)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Benchmarks
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Install rules
install(TARGETS audio_library music_manager
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/audio_library
    DESTINATION include
)