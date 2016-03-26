cmake_minimum_required (VERSION 3.2.2)
project (appserver)

# add SDL library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories ("${PROJECT_SOURCE_DIR}/src")

set (SOURCES
    shared/events.h
    shared/protocol.h
    src/app.h
    src/app.cpp
    src/compositor.h
    src/compositor.cpp   
    src/server.h
    src/server.cpp    
    src/windowmanager.h
    src/windowmanager.cpp
    src/sdlcompositor.h
    src/sdlcompositor.cpp
    src/window.h
    src/window.cpp
    src/sdlwindow.h
    src/sdlwindow.cpp
    src/eventsource.h
    src/sdleventsource.h
    src/sdleventsource.cpp
    src/main.cpp
    )
add_executable(appserver ${SOURCES})

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
    set (EXTRA_LIBS pthread rt)
endif()

target_link_libraries (appserver zmq ${EXTRA_LIBS} ${SDL2_LIBRARY})
set_property(TARGET appserver PROPERTY CXX_STANDARD 11)


