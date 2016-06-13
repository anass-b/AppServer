project (processmonitor)

include_directories (${ZMQ_INCLUDE_DIR})

add_executable (processmonitor src/processmonitor.cpp)

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
    set (EXTRA_LIBS pthread)
endif()
target_link_libraries (processmonitor ${ZMQ_LIBRARY} ${EXTRA_LIBS})

set_property(TARGET processmonitor PROPERTY CXX_STANDARD 11)

