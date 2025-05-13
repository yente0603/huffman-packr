# Huffman-packr

A tool for compressing and decompressing files using the Huffman coding algorithm. This project provides:
1.  A **Command-Line Interface (CLI)** primarily built with `make` for Linux-based systems.
2.  A **Graphical User Interface (GUI)** built with Qt6 and CMake for cross-platform use (Windows, macOS, Linux).

## Features

*   **File Compression & Decompression:** Losslessly compress any file into a smaller `.huff` file and restore `.huff` files back to their original form.
*   **Command-Line Interface (CLI):**
    *   Built using a traditional `Makefile`, optimized for Linux environments.
    *   **Custom Output Path:** Specify the name and location for the output file.
    *   **Automatic Naming:** If no output path is specified, the program automatically names the output file based on the input file and saves it to the `output/` directory by default.
    *   **Compression Ratio Display:** Show the compression ratio after a compression operation (`-r` option).
*   **Graphical User Interface (GUI - Qt):**
    *   Built using CMake for cross-platform compatibility.
    *   Provides an intuitive interface for selecting files to compress or decompress.
    *   Allows specifying output locations through file dialogs.
    *   Displays operation progress and results.
*   **C++ Standard Compatibility:** Core logic supports compilation with C++11 and later standards. The Qt GUI component typically requires C++17 or newer.

## Prerequisites

### For CLI

*   A C++ compiler supporting C++11 or later (e.g., g++).
*   `make` build tool.

On Debian/Ubuntu-based Linux distributions, you can install the necessary tools using:
```bash
sudo apt-get update
sudo apt-get install build-essential make
```

### For GUI

*   A C++ compiler supporting C++17 or later (e.g., GCC, Clang, MSVC).
*   CMake (version 3.16 or higher recommended).
*   Qt6 Development Libraries (Core, GUI, Widgets modules are essential).

**Installation examples for GUI prerequisites:**

*   **On Debian/Ubuntu-based Linux distributions:**
    ```bash
    sudo apt-get update
    sudo apt-get install build-essential cmake 
    sudo apt-get install qt6-base-dev qt6-base-dev-tools qt6-tools-dev qt6-tools-dev-tools # for qt6 essential packages
    sudo apt-get install libopengl-dev libglu1-mesa-dev libgl1-mesa-dev libglx-dev # for OpenGL essential packages
    ```
*   **On macOS (using Homebrew):**
    ```bash
    brew install cmake qt6
    ```
*   **On Windows:**
    *   Install Visual Studio with C++ development tools.
    *   Install CMake from [cmake.org](https://cmake.org/download/).
    *   Install Qt6 from [qt.io](https://www.qt.io/download-qt-installer) (select the MinGW or MSVC version compatible with your compiler). Ensure Qt's `bin` directory (e.g., `C:\Qt\6.x.y\mingw_64\bin` or `C:\Qt\6.x.y\msvc_x_y\bin`) and CMake's `bin` directory are in your system's PATH.

## Build

### 1. CLI (for Linux, using `make`)

1.  **Clone the repository:**
    ```bash
    git clone git@github.com:yente0603/huffman-packr.git
    cd huffman-packr
    ```

2.  **Build using `Makefile`:**
    ```bash
    make
    ```
    The executable `HuffmanTool` will be created in the `bin/` directory (relative to the project root).

### 2. GUI (Cross-Platform, using `CMake`)

1.  **Clone the repository (if not already done):**
    ```bash
    git clone git@github.com:yente0603/huffman-packr.git
    cd huffman-packr
    ```

2.  **Build:**
    ```bash
    mkdir build-gui # Or any other name for the build directory, e.g., 'build'
    cd build-gui
    # For macOS/Linux, if Qt6 is installed via Homebrew or in a non-standard path:
    # cmake -D CMAKE_PREFIX_PATH=/opt/homebrew/opt/qt6 .. 
    # For most systems where Qt6 is found automatically:
    cmake .. 
    make
    ```
    *   Ensure you are running `cmake ..` from *inside* your chosen build directory (e.g., `build-gui`).
    *   If CMake has trouble finding Qt, you might need to set `CMAKE_PREFIX_PATH` to your Qt installation directory (e.g., `-D CMAKE_PREFIX_PATH=/path/to/Qt/6.x.y/your_kit_arch`).
    * On Windows with Visual Studio, you might open the generated solution file in the `build-gui` directory or use `cmake --build . --config Release`.

This will build the `HuffmanToolGUI` executable (typically within the `build-gui` directory).
*Note: Your `CMakeLists.txt` might also be configured to build the CLI tool. If so, it might also appear in the `build-gui` directory.*

## Usage

### Command-Line Tool (`./bin/HuffmanTool`)

Run commands from the project root.

```bash
# Compress sourcePath/test.txt, output to output/test.txt.huff by default.
./bin/HuffmanTool -c sourcePath/test.txt                     
./bin/HuffmanTool -c sourcePath/test.txt -o yourOutputPath/yourCompressOutputName.huff 

# Decompress output/test.txt.huff, output to output/test.txt by default (restores original name/extension).
./bin/HuffmanTool -d output/test.txt.huff                   
./bin/HuffmanTool -d output/test.huff -o yourOutputPath/yourDecompressOutputName.txt 
```
*Note: The `-o` option for specifying the output path can generally be placed anywhere after the command (`-c` or `-d`) and the input file path.*

### GUI Application (`HuffmanToolGUI`)

1.  Navigate to the directory where `HuffmanToolGUI` was built (e.g., `build-gui`).
2.  Run the executable:
    *   Linux/macOS: `./HuffmanToolGUI`
    *   Windows: Double-click `HuffmanToolGUI.exe`

The GUI will provide options to select an input file, choose an operation (compress/decompress), specify an output file, and start the process.

## Output File Logs (Primarily for CLI)

The command-line tool, when built with `make`, generates several log files in the `output/tmp` directory. The GUI version aims to make interaction with these files unnecessary for the end-user by eventually embedding metadata. However, it still generates log files in `build/output/tmp`.

*   `freqTable.txt`: Stores the generated Huffman encoding table used during compression.
*   `zeroPadding.log`: Records the number of zero bits padded to the last byte of the compressed file.
*   `exten.log`: Stores the extension of the original input file.
*   `basename.log`: Stores the base name of the original input file.
*   `ratio.log`: Stores compression statistics. The `./bin/HuffmanTool -r` command reads this file.

## Limitations

*   The core logic might have limitations with files larger than approximately 4GB.
*   The GUI is an initial version and may undergo further improvements.
*   The CLI built with `make` uses separate log files for metadata, making compressed files not self-contained.

## Future Work

1.  **Core & CLI Improvements:**
    *   **Self-Contained Compressed Files:** Integrate metadata (encoding table, padding, original filename, etc.) as a file header into the compressed `.huff` file. This is a high priority for both CLI (especially if built with CMake for cross-platform use) and GUI to eliminate the dependency on separate log files.
    *   **Directory Compression:** Support compressing and decompressing entire directories (for both CLI and GUI).
    *   **Progress Indication:** Add a more detailed progress indicator for long-running operations in both CLI and GUI.
2.  **GUI Enhancements and Packaging:**
    *   Further refine the Qt GUI for improved user experience, error reporting, and visual feedback.
    *   Package the GUI application into distributable formats for Windows (.exe installer), macOS (.dmg/.app), and Linux (e.g., AppImage, .deb).
3.  **Build System & CI/CD:**
    *   Optionally, consolidate all builds (CLI and GUI) under CMake for better consistency and easier cross-platform CLI releases if desired.
    *   Set up Continuous Integration (CI) to automate builds and tests.
4.  **Exploring Alternative Techniques:**
    *   Implement techniques like Delta Encoding or Run-Length Encoding (RLE) as pre-processing steps for specific data types (like images) before Huffman compression.

## License

This project is licensed under the [MIT License] - see the [LICENSE](LICENSE) file for details.
 