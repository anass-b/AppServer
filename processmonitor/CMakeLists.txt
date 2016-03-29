project (processmonitor)

add_executable (processmonitor src/processmonitor.cpp)

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
    set (EXTRA_LIBS pthread)
endif()
target_link_libraries (processmonitor zmq ${EXTRA_LIBS})

set_property(TARGET processmonitor PROPERTY CXX_STANDARD 11)

