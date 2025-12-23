cmake_minimum_required(VERSION 3.5)

project(nBodyProblemSolver)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(include)

# Define the executable and the source files
add_executable(simulation src/Main.cpp src/Body.cpp src/Universe.cpp src/Gravity.cpp)
