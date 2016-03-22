project (asl-test1)

add_executable (asl-test1 main.cpp)

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
  set (EXTRA_LIBS pthread)
endif()
target_link_libraries (asl-test1 asl cairo ${LIBS} ${EXTRA_LIBS})

set_property(TARGET asl-test1 PROPERTY CXX_STANDARD 11)
