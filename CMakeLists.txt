cmake_minimum_required(VERSION 3.16) 

project(HuffmanToolGUI CXX)

# --- Qt Specifics ---
# Find Qt6 package, specifically Widgets module
find_package(Qt6 COMPONENTS Widgets REQUIRED) 
# # --- End Qt Specifics ---

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define core Huffman logic source files
set(HUFFMAN_LIB_SRCS
    src/tree.cpp
    src/file.cpp
    src/table.cpp
    src/utils.cpp
    src/config.cpp
)

# Define Qt-specific GUI source files
set(QT_GUI_SRCS 
    src/qt/mainQt.cpp
    src/qt/mainWindow.cpp
    include/qt/mainWindow.hpp # speical define for MacOS problem
    # .hpp and .ui files will be handled automatically by AUTOMOC and AUTOUIC
)

# Create a static library for the Huffman logic
add_library(HuffmanLib STATIC ${HUFFMAN_LIB_SRCS})
target_include_directories(HuffmanLib PUBLIC include) # So GUI can find huffman headers

# Create the GUI executable and link source files
add_executable(${PROJECT_NAME} ${QT_GUI_SRCS})
set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOUIC ON)

# Link Qt6 and the Huffman logic library
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets HuffmanLib) 

# Include directories for the executable
target_include_directories(${PROJECT_NAME} PRIVATE 
    include       # For huffman core headers
    include/qt    # For mainWindow.hpp
)

# Add compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wall) 

# ====== Output directory creation ======
# Create output directories at configure/build time
add_custom_target(create_output_dirs ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output/tmp
    COMMENT "Creating output directories in source tree"
)

# Ensure dependencies on directory creation
add_dependencies(HuffmanLib create_output_dirs)
add_dependencies(${PROJECT_NAME} create_output_dirs)
