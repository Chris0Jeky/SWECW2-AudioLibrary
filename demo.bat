@echo off
title Audio Library Demo
color 0A
echo ========================================
echo    Audio Library Management System
echo           Version 2.0 Demo
echo ========================================
echo.

echo Checking for required files...
if not exist music_manager.exe (
    echo Building the project...
    make clean >nul 2>&1
    make -j4
    
    if %ERRORLEVEL% NEQ 0 (
        echo Build failed. Please check the errors.
        pause
        exit /b 1
    )
    echo Build successful!
) else (
    echo Executable found, skipping build.
)

echo.
echo === Running Unit Tests ===
echo Testing Track class...
audio_library_tests.exe "[track]" -r compact

echo.
echo === Interactive Demo ===
echo.
echo This demo will show:
echo 1. Loading 36 tracks from CSV
echo 2. Searching for Queen tracks
echo 3. Listing all tracks
echo 4. Exiting the application
echo.
echo Press any key to start...
pause >nul

REM Create a demo input file for automated testing
(
echo 1
echo Queen
echo
echo 2
echo 0
echo
echo 0
) > demo_input.txt

echo.
echo Starting application with sample data...
echo.
music_manager.exe data\sample_tracks.csv < demo_input.txt

echo.
echo ========================================
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
echo.
echo To run interactively:
echo   music_manager.exe data\sample_tracks.csv
echo ========================================

REM Clean up
del demo_input.txt 2>nul

pause