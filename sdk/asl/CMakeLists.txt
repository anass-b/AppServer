project(asl)

include_directories (${ZMQ_INCLUDE_DIR})
include_directories ("${PROJECT_SOURCE_DIR}/include")

set (SOURCES
    src/asl.cpp
    src/connector.cpp
    )
set (HEADERS
    include/asl/asl.h
    include/asl/keycode.h
    include/asl/scancode.h
    include/private/connector.h 
    )
add_library (asl SHARED ${SOURCES} ${HEADERS})

target_link_libraries (asl ${ZMQ_LIBRARY} minilzo ${EXTRA_LIBS})

set_property(TARGET asl PROPERTY CXX_STANDARD 11)

add_subdirectory (tests/test1)
