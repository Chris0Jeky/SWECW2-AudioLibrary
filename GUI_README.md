# Audio Library GUI Documentation

This project now includes two GUI implementations for the Audio Library management system:

## 1. Web-based GUI (GitHub Pages Demo)

A modern web interface built with HTML5, CSS3, and vanilla JavaScript.

### Features
- **Library Management**: View, add, edit, and delete tracks
- **Advanced Search**: Multiple search modes (substring, exact, prefix, fuzzy, regex)
- **Import/Export**: Support for CSV and JSON formats
- **Statistics Dashboard**: Visual overview of your library
- **Responsive Design**: Works on desktop and mobile devices
- **Dark Theme**: Modern dark UI for comfortable viewing
- **Local Storage**: Persists data in browser

### Running Locally
Simply open `docs/index.html` in a web browser, or serve it with a local server:
```bash
cd docs
python -m http.server 8000
# Visit http://localhost:8000
```

### GitHub Pages Deployment
The web GUI is automatically deployed to GitHub Pages when you push to the main branch.
Access it at: `https://[your-username].github.io/SWECW2-AudioLibrary/`

### Usage
1. **Add Tracks**: Click "Add Track" in the sidebar or use the button
2. **Search**: Use quick search or advanced search with multiple filters
3. **Import Data**: Import CSV/JSON files or load sample data
4. **Export**: Export your library or search results
5. **Statistics**: View library insights in the Statistics tab

## 2. Desktop GUI (Qt Application)

A native desktop application built with Qt5 and C++17.

### Prerequisites
- Qt5 (Core and Widgets modules)
- CMake 3.16+
- C++17 compatible compiler
- pthread library

### Building the Desktop GUI

#### Linux/macOS:
```bash
cd gui
chmod +x build_gui.sh
./build_gui.sh
```

#### Windows:
```bash
cd gui
mkdir build_gui
cd build_gui
cmake ..
cmake --build . --config Release
```

#### Manual Build:
```bash
mkdir build_gui
cd build_gui
cmake ../gui
make -j$(nproc)
./AudioLibraryGUI
```

### Desktop GUI Features
- **Native Performance**: Fast, responsive native application
- **Familiar Interface**: Standard desktop UI patterns
- **Keyboard Shortcuts**: 
  - Ctrl+N: Add new track
  - Ctrl+E: Edit selected track
  - Delete: Remove selected track
  - Ctrl+F: Search
  - Ctrl+O: Import file
  - F5: Refresh
- **Double-click Editing**: Double-click any track to edit
- **Sort Options**: Click column headers or use dropdown
- **Progress Indicators**: Visual feedback for long operations
- **Multi-selection**: Select multiple tracks for batch operations

### Desktop GUI Usage
1. **Launch**: Run `./AudioLibraryGUI` (Linux/macOS) or `AudioLibraryGUI.exe` (Windows)
2. **Add Tracks**: Use File menu, toolbar, or Ctrl+N
3. **Import**: File → Import to load CSV/JSON files
4. **Search**: Use quick search bar or Advanced Search (Ctrl+F)
5. **Export**: File → Export to save your library
6. **Statistics**: View → Statistics for library overview

## Technical Details

### Web GUI Architecture
- **Pure JavaScript**: No frameworks required
- **LocalStorage API**: For data persistence
- **Responsive CSS Grid/Flexbox**: Modern layout
- **Font Awesome Icons**: Professional iconography
- **CSV/JSON Parser**: Built-in format support

### Desktop GUI Architecture
- **Qt5 Framework**: Cross-platform native UI
- **Model-View Pattern**: Separation of concerns
- **Shared Pointers**: Memory-safe track management
- **Thread-Safe Operations**: Uses existing library's thread safety
- **Signal/Slot System**: Qt's event handling

## Development

### Extending the Web GUI
The web GUI is modular and easy to extend:
- `app.js`: Main application logic
- `styles.css`: All styling (uses CSS variables)
- `index.html`: Structure and layout

### Extending the Desktop GUI
- `mainwindow.cpp/h`: Main application window
- `trackdialog.cpp/h`: Add/Edit track dialog
- `searchdialog.cpp/h`: Advanced search dialog

## Troubleshooting

### Web GUI Issues
- **Data not persisting**: Check browser's local storage settings
- **Import fails**: Ensure CSV/JSON format matches expected structure
- **Search not working**: Check console for JavaScript errors

### Desktop GUI Issues
- **Build fails**: Ensure Qt5 and CMake are properly installed
- **Missing libraries**: Install Qt5 development packages
- **Crashes on startup**: Check for missing Qt runtime libraries

### Qt Installation

#### Ubuntu/Debian:
```bash
sudo apt-get install qt5-default qtbase5-dev
```

#### macOS:
```bash
brew install qt@5
```

#### Windows:
Download Qt installer from https://www.qt.io/download

## License
Both GUIs use the same license as the main Audio Library project.