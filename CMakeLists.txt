cmake_minimum_required(VERSION 2.8)

project(GestureClassifier)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")


# option(DEBUG_PARSER "verbose parser debugging output" OFF)
# option(EXAMPLE_TEST "build test example" ON)
# option(EXAMPLE_OSC "build osc example" ON)


# add our own cmake-modules
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules/")

# directories
add_subdirectory(lbimproved)
add_subdirectory(YIN)
add_subdirectory(GestureClassification)
add_subdirectory(OSCGestureClassifier) 

# if( EXAMPLE_OSC )
#   add_subdirectory(osc)
# endif()

