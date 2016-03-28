project (minilzo)

set (SOURCES lzoconf.h lzodefs.h minilzo.h minilzo.c)
add_library(minilzo ${SOURCES})

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

target_link_libraries (minilzo)
