message( "=== lbimproved ===" )

include_directories(
  ../utils/
)

set(lbimproved_SRCS
  vector_test.cpp
  unittesting.cpp
)

add_library( lbimproved STATIC ${lbimproved_SRCS} )
