cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(StrokeRiskPredictionSystem LANGUAGES CXX)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(BACKEND_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(DATA_DIR "${BACKEND_SRC_DIR}/data") # Define path to  data directory

# Define application executable
add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    mainwindow.h
    mainwindow.cpp
    predictiondialog.h
    predictiondialog.cpp
    trainingdialog.h
    trainingdialog.cpp
    comparisonboarddialog.h
    comparisonboarddialog.cpp
)

# Link  Qt project to the existing backend C++ files
# IMPORTANT: List ALL backend .cpp files from the src directory here!
# This is a placeholder list; adjust it based on your actual repository content.
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
   # ${BACKEND_SRC_DIR}/DecisionTree.cpp
    ${BACKEND_SRC_DIR}/RandomForest.cpp
    ${BACKEND_SRC_DIR}/StrokeRiskPredictor.cpp
    ${BACKEND_SRC_DIR}/UserProfile.cpp
   # ${BACKEND_SRC_DIR}/QuestionnaireResponses.cpp
    ${BACKEND_SRC_DIR}/KaggleData.cpp # Assuming you have this
    ${BACKEND_SRC_DIR}/Comparison.cpp # Assuming you have this
    # Add any other core logic .cpp files here
)

# Link to Qt libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Widgets)

# Include directories for backend headers and the main project directory
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} # For all GUI .h files
    ${BACKEND_SRC_DIR}          # For backend .h files
)

# Set the working directory for the executable to ensure data files are found
# This assumes your 'data' folder is inside 'src'
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    QT_MESSAGELOG_DEBUG
)

# Enable Qt's MOC, UIC, and RCC processing
qt_add_executable(${CMAKE_PROJECT_NAME})

# Custom command to ensure data directory and file are copied to build directory
# This helps the executable find the CSV when run from the build folder.
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${DATA_DIR}" "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/data"
    COMMENT "Copying data directory to build location."
)
