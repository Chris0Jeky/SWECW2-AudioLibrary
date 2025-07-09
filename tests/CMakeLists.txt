# Testing configuration
include(CTest)

# Test executable
add_executable(audio_library_tests
    test_main.cpp
    test_track.cpp
    test_music_library.cpp
    test_search_engine.cpp
    test_file_io.cpp
)

target_link_libraries(audio_library_tests
    PRIVATE audio_library
)

# Add tests
add_test(NAME audio_library_tests COMMAND audio_library_tests)

# Enable verbose output for tests
set(CMAKE_CTEST_ARGUMENTS "--verbose")