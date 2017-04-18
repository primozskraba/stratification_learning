cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME_STR topology)
project(${PROJECT_NAME_STR} C CXX)

# Locate GTest
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(../src)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -ansi -Wno-deprecated -pthread -std=c++14)
endif()

include_directories(${GTEST_INCLUDE_DIRS})
# include_directories(../src)

# set (CMAKE_EXE_LINKER_FLAGS -headerpad_max_install_names)
# find_package(CGAL)
# include                     (${CGAL_USE_FILE})


# add_definitions             (${CGAL_CXX_FLAGS_INIT})
# include_directories         (${CGAL_INCLUDE_DIRS})

# add_executable          (buildComplex   buildComplex.cpp)
# target_link_libraries   (buildComplex ${CGAL_LIBRARY} ${CGAL_3RD_PARTY_LIBRARIES})
      

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)

# include_directories(../src)

add_library(num_lib ../src/num.cpp)
add_library(linalg_lib ../src/linalg.cpp)
# add_library(struct_lib ../src/struct.cpp)

##############
# Unit Tests
##############
file(GLOB TEST_SRC_FILES *.cpp)
add_executable(runUnitTests ${TEST_SRC_FILES})

# Standard linking to gtest stuff.
# target_link_libraries(run-all-tests gtest gtest_main)
target_link_libraries(runUnitTests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)


# Extra linking for the project.
# target_link_libraries(run-all-tests project1_lib)

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
# add_test(NAME that-test-I-made COMMAND runUnitTests)

# You can also omit NAME and COMMAND. The second argument could be some other
# test executable.
add_test(test-all runUnitTests)
