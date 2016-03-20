project (cef-test)

find_package(Cef3 REQUIRED)

set (INCLUDES ${INCLUDES} ${OGRE_INCLUDE_DIRS} ${CEF3_INCLUDE_DIRS} )
set (LIBS ${LIBS} ${OGRE_LIBRARIES} ${CEF3_LIBRARIES} )

include_directories(${INCLUDES})

add_executable (cef-test main.cpp)

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
  set (EXTRA_LIBS pthread)
endif()
target_link_libraries (cef-test asl stdc++ ${LIBS} ${EXTRA_LIBS})

set_property(TARGET cef-test PROPERTY CXX_STANDARD 11)
