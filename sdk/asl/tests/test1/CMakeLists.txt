project (asl-test1)

add_executable (asl-test1 main.cpp)

target_link_libraries (asl-test1 asl cairo ${LIBS} ${EXTRA_LIBS})

set_property(TARGET asl-test1 PROPERTY CXX_STANDARD 11)
