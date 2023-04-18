# Define the compiler and its flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Define the source files and object files
SRC = main.cpp track.cpp hash_table.cpp file_loader.cpp
OBJ = $(SRC:.cpp=.o)

# Define the executable output
EXECUTABLE = music_library

# Default target: build the executable
all: $(EXECUTABLE)

# Link object files to create the executable
$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target: remove object files and the executable
clean:
	rm -f $(OBJ) $(EXECUTABLE)
