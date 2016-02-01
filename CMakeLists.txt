cmake_minimum_required(VERSION 2.8)
project(cdna)

# setup build directory
set(CFLAGS -m64)
set(CXXFLAGS -m64)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# setup header files
include_directories(include)

# setup libraries
set(Boost_USE_STATIC_LIBS off)
set(Boost_USE_MULTITHREAD on)
set(Boost_USE_STATIC_RUNTIME off)
find_package(Boost 1.53 COMPONENTS system thread chrono)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# setup sub directory
add_subdirectory(src)
