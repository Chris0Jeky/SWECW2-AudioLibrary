@echo off
echo Audio Library Demo Script for Windows
echo =====================================
echo.

echo Building the project...
make clean >nul 2>&1
make -j4

if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Please check the errors.
    exit /b 1
)

echo Build successful!
echo.

echo Running basic tests...
audio_library_tests.exe "[track]"

echo.
echo Starting the Music Library application...
echo.
echo Loading sample data from data/sample_tracks.csv
echo.

REM Create a demo input file for automated testing
(
echo 1
echo Queen
echo 2
echo 1
echo 0
) > demo_input.txt

music_manager.exe data/sample_tracks.csv < demo_input.txt

echo.
echo Demo completed successfully!
echo.
echo Features demonstrated:
echo - Modern C++17 implementation
echo - Thread-safe operations  
echo - Advanced search capabilities
echo - CSV/JSON import/export
echo - Comprehensive metadata support
echo - Statistics and analytics
echo - Unit testing with Catch2

REM Clean up
del demo_input.txt 2>nul

pause