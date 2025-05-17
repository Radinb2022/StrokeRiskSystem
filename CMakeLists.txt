cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(StrokeRiskPredictionSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# --- FLTK Configuration ---
# IMPORTANT: Set this to the root directory where FLTK is installed on your system.
# Example: set(FLTK_DIR "C:/path/to/fltk-1.3.x")
# Example: set(FLTK_DIR "/usr/local" ) # If installed system-wide on Linux/macOS
set(FLTK_DIR "" CACHE PATH "Path to FLTK installation directory")

find_package(FLTK REQUIRED COMPONENTS fltk) # Use fltk_gl if you need OpenGL features

# Add FLTK include directories
include_directories(${FLTK_INCLUDE_DIRS})

# Add FLTK library directories
link_directories(${FLTK_LIBRARY_DIRS})

# Define the path to your backend C++ files
set(BACKEND_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(DATA_DIR "${BACKEND_SRC_DIR}/data")

# Define your application executable
add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    MainWindow.h
    MainWindow.cpp
    PredictionDialog.h
    PredictionDialog.cpp
    TrainingDialog.h
    TrainingDialog.cpp
    ComparisonBoardDialog.h
    ComparisonBoardDialog.cpp
)

# List ALL your backend .cpp files from the src directory
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${BACKEND_SRC_DIR}/DataProcessor.cpp
    ${BACKEND_SRC_DIR}/DecisionTree.cpp
    ${BACKEND_SRC_DIR}/RandomForest.cpp
    ${BACKEND_SRC_DIR}/StrokeRiskPredictor.cpp
    ${BACKEND_SRC_DIR}/UserProfile.cpp
    ${BACKEND_SRC_DIR}/ComparisonEngine.cpp
)

# Link to FLTK libraries and your backend includes
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${FLTK_LIBRARIES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} # For GUI .h files
    ${BACKEND_SRC_DIR}          # For backend .h files
)

# Set the working directory for the executable to ensure data files are found
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${DATA_DIR}" "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/data"
    COMMENT "Copying data directory to build location."
)
