cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(StrokeRiskPredictionSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# --- FLTK Configuration ---
# IMPORTANT: Set this to the root directory where FLTK is installed on your system.
set(FLTK_DIR "C:/Users/im-a-/Downloads/fltk-1.4.3-source/fltk-1.4.3" CACHE PATH "Path to FLTK installation directory")

find_package(FLTK REQUIRED COMPONENTS fltk)

include_directories(${FLTK_INCLUDE_DIRS})
link_directories(${FLTK_LIBRARY_DIRS})

# Define the path to your backend C++ files
set(BACKEND_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(DATA_DIR "${BACKEND_SRC_DIR}/data")
set(DATA_FILE_NAME "stroke_data_new.csv") # <--- UPDATED: New data file name

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

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${FLTK_LIBRARIES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BACKEND_SRC_DIR}
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${DATA_DIR}/${DATA_FILE_NAME}" "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/data/${DATA_FILE_NAME}"
    COMMENT "Copying data file to build location."
)
# Ensure the 'data' directory itself is created in the build output if it doesn't exist
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/data"
    COMMENT "Ensuring data directory exists in build location."
)