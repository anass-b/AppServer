cmake_minimum_required (VERSION 3.2.2)
project (appserver)

# add SDL library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories ("${PROJECT_SOURCE_DIR}/src")

set (SOURCES 
    src/App.h
    src/App.cpp
    src/Compositor.h
    src/Compositor.cpp   
    src/Server.h
    src/Server.cpp    
    src/WindowManager.h
    src/WindowManager.cpp
    src/SDLCompositor.h
    src/SDLCompositor.cpp
    src/Window.h
    src/Window.cpp
    src/SDLWindow.h
    src/SDLWindow.cpp
    src/EventSource.h
    src/SDLEventSource.h
    src/SDLEventSource.cpp
    src/main.cpp
    )
add_executable(appserver ${SOURCES})

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
    set (EXTRA_LIBS pthread rt)
endif()

target_link_libraries (appserver zmq ${EXTRA_LIBS} ${SDL2_LIBRARY})
set_property(TARGET appserver PROPERTY CXX_STANDARD 11)


