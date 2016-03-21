project(asl)

include_directories ("${PROJECT_SOURCE_DIR}/include")

set (SOURCES
    src/CApi.cpp
    src/Connector.cpp
    )
set (HEADERS
    include/Asl/CApi.h
    include/Asl/Connector.h
    )
add_library (asl SHARED ${SOURCES} ${HEADERS})

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
    set (EXTRA_LIBS rt)
endif()
target_link_libraries (asl zmq ${EXTRA_LIBS})

set_property(TARGET asl PROPERTY CXX_STANDARD 11)

add_subdirectory (tests/test1)