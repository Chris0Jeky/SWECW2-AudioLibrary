# Define the compiler and its flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Define the source files and object files
SRC = main.cpp track.cpp hash_table.cpp file_loader.cpp
OBJ = $(SRC:.cpp=.o)

# Define the test source files and object files
TEST_SRC = tests.cpp track.cpp hash_table.cpp file_loader.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Define the executable output
EXECUTABLE = music_library
TEST_EXECUTABLE = tests

# Default target: build the executable
all: $(EXECUTABLE)

# Link object files to create the executable
$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run tests
tests: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Clean target: remove object files and the executable
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(EXECUTABLE) $(TEST_EXECUTABLE)
