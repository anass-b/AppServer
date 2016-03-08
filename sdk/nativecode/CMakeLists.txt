project(nativecode)

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_subdirectory (tests/test1)
add_subdirectory (tests/test2)

set (SOURCES
	src/application.cpp
	src/geometry.cpp
	src/graphics.cpp
	src/server.cpp
	src/server.cpp
	src/view.cpp
	src/window.cpp
	)
set (HEADERS
	include/nativecode/application.h
	include/nativecode/geometry.h
	include/nativecode/graphics.h
	include/nativecode/input.h
	include/nativecode/nativecode.h
	include/nativecode/view.h
	include/nativecode/window.h
	)
add_library (nativecode SHARED ${SOURCES} ${HEADERS})

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
	set (EXTRA_LIBS rt)
endif()

target_link_libraries (nativecode appserver cairo ${EXTRA_LIBS})

set_property(TARGET nativecode PROPERTY CXX_STANDARD 11)