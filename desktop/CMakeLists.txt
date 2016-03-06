cmake_minimum_required (VERSION 3.2.2)
project (DesktopClient)

FIND_PACKAGE(OpenGL REQUIRED)
if(OpenGL_FOUND)
	link_directories(${OpenGL_LIBRARY_DIRS})
	include_directories(${OpenGL_INCLUDE_DIR})
endif(OpenGL_FOUND)

include_directories ("${PROJECT_SOURCE_DIR}/include")

# add usr/local
include_directories ("/usr/local/include")
link_directories ("/usr/local/lib")

# add appserverlib library
include_directories ("${PROJECT_SOURCE_DIR}/appserverlib/include")
add_subdirectory (appserverlib)

set (SOURCES 
	src/App.cpp
	src/Compositor.cpp
	src/Geometry.cpp
	src/HCompositor.cpp
	src/HWindow.cpp
	src/InputSource.cpp
	src/main.cpp
	src/Server.cpp
	src/Window.cpp
	src/WindowManager.cpp
	src/Workspace.cpp
	src/glfw/GlfwInputSource.cpp
	src/glfw/GlfwWorkspace.cpp
	)
set (HEADERS
	include/App.h
	include/Compositor.h
	include/Geometry.h
	include/HCompositor.h
	include/HWindow.h
	include/InputSource.h
	include/Server.h
	include/Window.h
	include/WindowManager.h
	include/Workspace.h
	include/glfw/GlfwInputSource.h
	include/glfw/GlfwWorkspace.h
	)
add_executable(DesktopClient ${SOURCES} ${HEADERS})

if (APPLE)
	set (EXTRA_LIBS glfw3)
elseif (UNIX)
	set (EXTRA_LIBS glfw pthread rt)
endif()

target_link_libraries (DesktopClient appserver ${EXTRA_LIBS} ${OPENGL_LIBRARY})
set_property(TARGET DesktopClient PROPERTY CXX_STANDARD 11)