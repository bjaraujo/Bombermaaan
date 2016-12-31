
project(ResGen)

set(RESGEN_HEADERS
ResGen.h
)

set(RESGEN_SOURCES
ResGen.cpp
)

source_group("Header Files" FILES ${RESGEN_HEADERS})
source_group("Source Files" FILES ${RESGEN_SOURCES})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

add_executable(ResGen ${RESGEN_HEADERS} ${RESGEN_SOURCES})

#message(${CMAKE_CURRENT_SOURCE_DIR}/../res)

#add_custom_command(OUTPUT ResData.c
#    COMMAND ResGen 
#    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../res
#    DEPENDS ResGen
#    WORKING_DIRECTORY ResGen
#)

add_library(Bombermaaan32 SHARED ResData.c)


