
project(tinyxml)

set(TINYXML_HEADERS
tinystr.h
tinyxml.h
)

set(TINYXML_SOURCES
tinystr.cpp
tinyxml.cpp
tinyxmlerror.cpp
tinyxmlparser.cpp
)

source_group("Header Files" FILES ${TINYXML_HEADERS})
source_group("Source Files" FILES ${TINYXML_SOURCES})

add_definitions(-DTIXML_USE_STL)

add_library(tinyxml STATIC ${TINYXML_HEADERS} ${TINYXML_SOURCES})


