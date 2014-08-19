message( "=== OSC Gesture Classifier ===" )

include(FindPkgConfig)
pkg_check_modules(liblo REQUIRED liblo)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${liblo_INCLUDE_DIRS}
  ../GestureClassification/
  ../YIN/
  ../lbimproved/
  ../utils/
)

link_directories(
   ${liblo_LIBRARY_DIRS}
)

set( CMAKE_CXX_FLAGS "-std=c++0x" )

##########################################################################
# osc gesture classifier
##########################################################################

set(oscgestureclassifier_SRCS
  OSCGestureClassifier.cpp
)

add_executable( OSCGestureClassifier ${oscgestureclassifier_SRCS} )

target_link_libraries(OSCGestureClassifier ${liblo_LIBRARIES} )

install(TARGETS OSCGestureClassifier DESTINATION bin)