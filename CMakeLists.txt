cmake_minimum_required (VERSION 3.2.2)
project (DesktopClient)

# add usr/local
include_directories ("/usr/local/include")
link_directories ("/usr/local/lib")

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
	link_directories(${OpenGL_LIBRARY_DIRS})
	include_directories(${OpenGL_INCLUDE_DIR})
endif(OpenGL_FOUND)

# add SDL library
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DesktopClient_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories ("${PROJECT_SOURCE_DIR}/src")

# add appserver library
include_directories ("${PROJECT_SOURCE_DIR}/appserverlib/include")
add_subdirectory (appserverlib)

# add nativecode library
add_subdirectory (sdk/nativecode)
#add_subdirectory (sdk/nativecode/tests/test1)
#add_subdirectory (sdk/nativecode/tests/test2)

set (SOURCES 
	src/App.h
	src/App.cpp
	src/Compositor.h
	src/Compositor.cpp
	src/Geometry.h
	src/Geometry.cpp	
	src/InputSource.h
	src/InputSource.cpp
	src/main.cpp
	src/Server.h
	src/Server.cpp
	src/Window.h
	src/Window.cpp
	src/WindowManager.h
	src/WindowManager.cpp
	src/Workspace.h
	src/Workspace.cpp
	src/GLCompositor.h
	src/GLCompositor.cpp
	src/GLWindow.h
	src/GLWindow.cpp
	src/GlfwInputSource.h
	src/GlfwInputSource.cpp
	src/GlfwWorkspace.h
	src/GlfwWorkspace.cpp
	src/SDLCompositor.h
	src/SDLCompositor.cpp
	src/SDLWindow.h
	src/SDLWindow.cpp
	src/SDLInputSource.h
	src/SDLInputSource.cpp
	src/SDLWorkspace.h
	src/SDLWorkspace.cpp
	)
add_executable(DesktopClient ${SOURCES})

if (APPLE)
	set (EXTRA_LIBS glfw3)
elseif (UNIX)
	set (EXTRA_LIBS glfw pthread rt)
endif()

target_link_libraries (DesktopClient appserver zmq ${EXTRA_LIBS} ${OPENGL_LIBRARY} ${SDL2_LIBRARY})
set_property(TARGET DesktopClient PROPERTY CXX_STANDARD 11)


