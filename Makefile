# Audio Library Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -pthread
INCLUDES = -Iinclude -Ilegacy
LDFLAGS = -pthread

# Directories
SRC_DIR = src/audio_library
OBJ_DIR = obj
BIN_DIR = .

# Source files
LIBRARY_SOURCES = $(SRC_DIR)/track.cpp \
                  $(SRC_DIR)/music_library.cpp \
                  $(SRC_DIR)/search_engine.cpp \
                  $(SRC_DIR)/file_io.cpp

MAIN_SOURCE = src/main.cpp

# Object files
LIBRARY_OBJECTS = $(LIBRARY_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
MAIN_OBJECT = $(OBJ_DIR)/main.o

# Targets
TARGET = $(BIN_DIR)/music_manager

# Test files
TEST_DIR = tests
TEST_SOURCES = $(TEST_DIR)/test_main.cpp \
               $(TEST_DIR)/test_track.cpp \
               $(TEST_DIR)/test_music_library.cpp \
               $(TEST_DIR)/test_search_engine.cpp \
               $(TEST_DIR)/test_file_io.cpp

TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/tests/%.o)
TEST_TARGET = $(BIN_DIR)/audio_library_tests

# Default target
all: $(TARGET)

# Main executable
$(TARGET): $(LIBRARY_OBJECTS) $(MAIN_OBJECT)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Built: $@"

# Test executable
tests: $(TEST_TARGET)

$(TEST_TARGET): $(LIBRARY_OBJECTS) $(TEST_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Built: $@"

# Object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJ_DIR)/main.o: src/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJ_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/tests
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

# Run tests
test: tests
	./$(TEST_TARGET)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)
	@echo "Cleaned build files"

# Install
install: $(TARGET)
	@echo "Installing to /usr/local/bin"
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete"

# Phony targets
.PHONY: all clean tests test install

# Dependencies
-include $(LIBRARY_OBJECTS:.o=.d)
-include $(MAIN_OBJECT:.o=.d)
-include $(TEST_OBJECTS:.o=.d)