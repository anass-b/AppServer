cmake_minimum_required (VERSION 3.2.2)
project (workspace)

# ZeroMQ
set(ZMQ_INCLUDE_DIR "" CACHE PATH "")
set(ZMQ_LIBRARY "" CACHE FILEPATH "")

# cairo graphics
set(CAIRO_INCLUDE_DIR "" CACHE PATH "")
set(CAIRO_LIBRARY "" CACHE FILEPATH "")

# SDL
set(SDL2_INCLUDE_DIR "" CACHE PATH "")
set(SDL2_LIBRARY "" CACHE FILEPATH "")

SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

include_directories ("${PROJECT_SOURCE_DIR}/vendor")
include_directories ("${PROJECT_SOURCE_DIR}/sdk/asl/include")
include_directories ("${PROJECT_SOURCE_DIR}/appserver/shared")

add_subdirectory (appserver)
add_subdirectory (processmonitor)
add_subdirectory (sdk)

# Disabled because it causes build errors when using QtCreator
#add_subdirectory (apps/webbrowser)

add_subdirectory (vendor/minilzo)

