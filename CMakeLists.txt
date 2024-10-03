cmake_minimum_required(VERSION 3.14)
project(NamReamp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libsndfile package
find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)

# Print SNDFILE variables for debugging
message(STATUS "SNDFILE_LIBRARIES: ${SNDFILE_LIBRARIES}")
message(STATUS "SNDFILE_LIBRARY_DIRS: ${SNDFILE_LIBRARY_DIRS}")
message(STATUS "SNDFILE_LDFLAGS: ${SNDFILE_LDFLAGS}")

# Find all source files in the Source directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

# Add NAM source files directly to the project
file(GLOB_RECURSE NAM_SOURCE_FILES 
    "deps/NeuralAmpModelerCore/NAM/*.cpp"
    "deps/NeuralAmpModelerCore/NAM/*.h"
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${NAM_SOURCE_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/deps/NeuralAmpModelerCore
    ${CMAKE_SOURCE_DIR}/deps/NeuralAmpModelerCore/NAM
    ${CMAKE_SOURCE_DIR}/deps/NeuralAmpModelerCore/Dependencies/eigen
    ${CMAKE_SOURCE_DIR}/deps/NeuralAmpModelerCore/Dependencies/nlohmann
    ${SNDFILE_INCLUDE_DIRS}  # Add this line
)

# Add SNDFILE_LIBRARY_DIRS to the link directories
link_directories(${SNDFILE_LIBRARY_DIRS})

# Link against libsndfile
target_link_libraries(${PROJECT_NAME} PRIVATE ${SNDFILE_LDFLAGS})

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

add_definitions(-DRELEASE=1 -DNDEBUG=1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -fno-rtti -std=c++17 -Ofast -DNDEBUG -pipe -feliminate-unused-debug-types -funroll-loops")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvectorize")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize")
endif()

# Eigen-specific optimizations
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    EIGEN_NO_DEBUG
)

# Print the library search path for debugging
get_property(lib_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
message(STATUS "Library search path: ${lib_dirs}")
