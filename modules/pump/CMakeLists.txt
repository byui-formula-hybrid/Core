# CMakeLists.txt template for a CoreLib module

# Replace pump with the actual module name, e.g., core, battery, can, etc.
set(MODULE_NAME pump)
set(LIB_TARGET corelib_${MODULE_NAME})

# Gather all .cpp files automatically
file(GLOB MODULE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create the library target
add_library(${LIB_TARGET} ${MODULE_SRC})

# Set the include path for this target and for consumers
target_include_directories(${LIB_TARGET} PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: If this module depends on another module, add it here
# target_link_libraries(${LIB_TARGET} PUBLIC corelib_<dependency>)