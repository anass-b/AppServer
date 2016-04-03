project (asl-test1)

set(CAIRO_INCLUDE_DIR "" CACHE PATH "")
set(CAIRO_LIBRARY "" CACHE FILEPATH "")

include_directories (${CAIRO_INCLUDE_DIR})

add_executable (asl-test1 main.cpp)

target_link_libraries (asl-test1 asl ${CAIRO_LIBRARY} ${LIBS} ${EXTRA_LIBS})

set_property(TARGET asl-test1 PROPERTY CXX_STANDARD 11)
