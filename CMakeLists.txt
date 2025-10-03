cmake_minimum_required(VERSION 3.16)
project(CoreLib LANGUAGES CXX)

# Set C++ standard (optional)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing for all subdirectories
enable_testing()

# Add all modules
add_subdirectory(modules/core)
add_subdirectory(modules/battery)
add_subdirectory(modules/can)
add_subdirectory(modules/display)
add_subdirectory(modules/imd)
add_subdirectory(modules/inverter)
add_subdirectory(modules/pedals)
add_subdirectory(modules/pump)

add_subdirectory(tests)