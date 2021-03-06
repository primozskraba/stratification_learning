# CMAKE VERSION
cmake_minimum_required(VERSION 2.8.11)

# PROJECT PROPERTIES
set(PROJECT_NAME_STR topology)
project(${PROJECT_NAME_STR} C CXX)

# REQUIRED PACKAGES
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

find_package(CGAL REQUIRED) 
include                     (${CGAL_USE_FILE})

find_package(Eigen3 REQUIRED) 
include                     (${EIGEN3_USE_FILE})

add_definitions             (${CGAL_CXX_FLAGS_INIT})
include_directories         (${CGAL_INCLUDE_DIRS})


# COMPILER FLAGS
# if(CMAKE_COMPILER_IS_GNUCXX)
# endif()
add_definitions(-Wall -ansi -Wno-deprecated -pthread -std=c++14 )

# INCLUDE DIRECTORIES
# include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../src)

# BUILD RELEASE / DEBUG
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

# SOURCE FILES
set(TEST_FILES tests.cpp)
file(GLOB SRC_FILES ../src/*.cpp)

# LIBRARIES
add_library(toplib ${SRC_FILES})


# EXECUTABLES
add_executable(runUnitTests tests.cpp ${TEST_FILES})

# LINK LIBRARIES
target_link_libraries(runUnitTests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
target_link_libraries(runUnitTests toplib)
target_link_libraries(runUnitTests ${CGAL_LIBRARY} ${CGAL_3RD_PARTY_LIBRARIES})
