project (nativecode-test1)

add_executable (nativecode-test1 main.cpp)

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
	set (EXTRA_LIBS pthread)
endif()
target_link_libraries (nativecode-test1 nativecode ${EXTRA_LIBS})

set_property(TARGET nativecode-test1 PROPERTY CXX_STANDARD 11)