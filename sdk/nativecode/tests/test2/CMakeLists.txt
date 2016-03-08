project (nativecode-test2)

add_executable (nativecode-test2 main.cpp)

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
	set (EXTRA_LIBS pthread)
endif()
target_link_libraries (nativecode-test2 nativecode ${EXTRA_LIBS})

set_property(TARGET nativecode-test2 PROPERTY CXX_STANDARD 11)