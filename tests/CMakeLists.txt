# Root tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(CoreLibTests)

# Enable CTest
enable_testing()

# Add each module test subdirectory
add_subdirectory(core)
add_subdirectory(battery)
add_subdirectory(can)
add_subdirectory(display)
add_subdirectory(imd)
add_subdirectory(inverter)
add_subdirectory(pedals)
add_subdirectory(pump)
