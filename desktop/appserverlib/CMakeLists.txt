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
add_library (appserverlib ${SOURCES} ${HEADERS})
set_property(TARGET appserverlib PROPERTY CXX_STANDARD 11)