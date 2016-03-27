project(asl)

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

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
    set (EXTRA_LIBS rt)
endif()
target_link_libraries (asl zmq z ${EXTRA_LIBS})

set_property(TARGET asl PROPERTY CXX_STANDARD 11)

add_subdirectory (tests/test1)
