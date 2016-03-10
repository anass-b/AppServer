project (processmonitor)

add_executable (processmonitor processmonitor.cpp)

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
    set (EXTRA_LIBS rt pthread)
endif()
target_link_libraries (processmonitor appserver zmq ${EXTRA_LIBS})

set_property(TARGET processmonitor PROPERTY CXX_STANDARD 11)

