cmake_minimum_required(VERSION 3.10)
project(StrokeRiskSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Armadillo REQUIRED)
find_package(mlpack REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${ARMADILLO_INCLUDE_DIRS}
    ${MLPACK_INCLUDE_DIRS}
)

# Add executable
add_executable(stroke_risk_system
    src/main.cpp
    src/RandomForestUI.cpp
    src/RFClassifier.cpp
    src/StrokeRiskPredictor.cpp
    src/KaggleData.cpp
    src/Comparison.cpp
    src/Tester.cpp
)

# Link libraries
target_link_libraries(stroke_risk_system
    ${ARMADILLO_LIBRARIES}
    ${MLPACK_LIBRARIES}
) 