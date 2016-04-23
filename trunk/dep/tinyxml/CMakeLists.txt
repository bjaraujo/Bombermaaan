
PROJECT(tinyxml)

SET(TINYXML_HEADERS
tinystr.h
tinyxml.h
)

SET(TINYXML_SOURCES
tinystr.cpp
tinyxml.cpp
tinyxmlerror.cpp
tinyxmlparser.cpp
)

SOURCE_GROUP("Header Files" FILES ${TINYXML_HEADERS})
SOURCE_GROUP("Source Files" FILES ${TINYXML_SOURCES})

#ADD_DEFINITIONS(-DTIXML_USE_STL)

ADD_LIBRARY(tinyxml STATIC ${TINYXML_HEADERS} ${TINYXML_SOURCES})


