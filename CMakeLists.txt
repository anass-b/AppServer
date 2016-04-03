cmake_minimum_required (VERSION 3.2.2)
project (workspace)

set(ZMQ_INCLUDE_DIR "" CACHE PATH "")
set(ZMQ_LIBRARY "" CACHE FILEPATH "")

include_directories (${ZMQ_INCLUDE_DIR})

# add usr/local
#include_directories ("/usr/local/include")
#link_directories ("/usr/local/lib")

include_directories ("${PROJECT_SOURCE_DIR}/appserver/shared")
include_directories ("${PROJECT_SOURCE_DIR}/vendor")

add_subdirectory (vendor/minilzo)
add_subdirectory (sdk)
add_subdirectory (processmonitor)
add_subdirectory (appserver)

