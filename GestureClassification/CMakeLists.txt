message( "=== GestureClassification ===" )

include_directories(
  ../utils/
  ../lbimproved/
  ../YIN/
)

set(gestureclassification_SRCS
  DtwPsClassifier.cpp
  RepClassifier.cpp
  Template.cpp
)

add_library( GestureClassification STATIC ${gestureclassification_SRCS} )


# main.cpp is a test program
