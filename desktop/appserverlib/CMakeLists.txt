set (SOURCES
	src/CApi.cpp
	src/Connector.cpp
	src/Logger.cpp
	src/Utils.cpp
	)
set (HEADERS
	include/Asl/CApi.h
	include/Asl/Connector.h
	include/Asl/Logger.h
	include/Asl/Protocol.h
	include/Asl/Utils.h
	)
add_library (appserver SHARED ${SOURCES} ${HEADERS})

if ("${CMAKE_SYSTEM}" MATCHES "Linux")
	target_link_libraries (appserver rt)
endif()

set_property(TARGET appserver PROPERTY CXX_STANDARD 11)