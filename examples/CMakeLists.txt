# Example programs

# Basic usage example
add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage PRIVATE audio_library)

# Import/Export example
add_executable(import_export import_export.cpp)
target_link_libraries(import_export PRIVATE audio_library)

# Advanced search example
add_executable(advanced_search advanced_search.cpp)
target_link_libraries(advanced_search PRIVATE audio_library)

# Performance demo
add_executable(performance_demo performance_demo.cpp)
target_link_libraries(performance_demo PRIVATE audio_library)