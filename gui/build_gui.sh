#!/bin/bash

# Build script for Audio Library GUI

echo "Building Audio Library GUI..."

# Create build directory
mkdir -p build_gui
cd build_gui

# Configure with CMake
echo "Configuring with CMake..."
cmake ../gui

# Build
echo "Building..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run ./AudioLibraryGUI to start the application"
else
    echo "Build failed!"
    exit 1
fi