cmake_minimum_required(VERSION 2.8)

include_directories(./include)
add_definitions(-std=c++11)

set (CMAKE_EXE_LINKER_FLAGS -headerpad_max_install_names)
find_package(CGAL)
include                     (${CGAL_USE_FILE})


add_definitions             (${CGAL_CXX_FLAGS_INIT})
include_directories         (${CGAL_INCLUDE_DIRS})

add_executable          (buildComplex   buildComplex.cpp)
target_link_libraries   (buildComplex ${CGAL_LIBRARY} ${CGAL_3RD_PARTY_LIBRARIES})
      

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)
