cmake_minimum_required (VERSION 3.2.2)
project (workspace)

# Necessary for sdk/asl/tests/cef project
set(CMAKE_OSX_DEPLOYMENT_TARGET  "10.7")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# add usr/local
include_directories ("/usr/local/include")
link_directories ("/usr/local/lib")

include_directories ("${PROJECT_SOURCE_DIR}/headers")
include_directories ("${PROJECT_SOURCE_DIR}/vendor")

add_subdirectory (sdk)
add_subdirectory (processmonitor)
add_subdirectory (appserver)




