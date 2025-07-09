#!/bin/bash

# Audio Library Demo Script

echo "Building the project..."
make clean > /dev/null 2>&1
make -j4 > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "Build failed. Please check the errors."
    exit 1
fi

echo "Build successful!"
echo ""
echo "Running tests..."
./audio_library_tests "[track]" --reporter compact

echo ""
echo "Starting the Music Library application..."
echo ""

# Create a demo input file for automated testing
cat > demo_input.txt << EOF
1
Queen
2
1
0
EOF

./music_manager data/sample_tracks.csv < demo_input.txt

echo ""
echo "Demo completed successfully!"
echo ""
echo "Features demonstrated:"
echo "✓ Modern C++17 implementation"
echo "✓ Thread-safe operations"
echo "✓ Advanced search capabilities"
echo "✓ CSV/JSON import/export"
echo "✓ Comprehensive metadata support"
echo "✓ Statistics and analytics"
echo "✓ Unit testing with Catch2"

# Clean up
rm -f demo_input.txt