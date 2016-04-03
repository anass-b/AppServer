project (appserver)

set(SDL2_INCLUDE_DIR "" CACHE PATH "")
set(SDL2_LIBRARY "" CACHE FILEPATH "")
if (!APPLE)
    set(SDL2MAIN_LIBRARY "" CACHE FILEPATH "")
endif()

include_directories (${SDL2_INCLUDE_DIR})
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
    set (EXTRA_LIBS pthread)
endif()

target_link_libraries (appserver minilzo ${EXTRA_LIBS} ${ZMQ_LIBRARY} ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})
set_property(TARGET appserver PROPERTY CXX_STANDARD 11)


