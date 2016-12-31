
project(BombermaaanNet)

set(BOMBERMAAAN_NET_HEADERS
../src/CNetwork.h
anyoption.h
aes256.hpp
)

set(BOMBERMAAAN_NET_SOURCES
../src/CNetwork.cpp
../src/CLog.cpp
main.cpp
anyoption.cpp
aes256.cpp
)

source_group("Header Files" FILES ${BOMBERMAAAN_NET_HEADERS})
source_group("Source Files" FILES ${BOMBERMAAAN_NET_SOURCES})

find_package(SDL REQUIRED)
find_package(SDL_net REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../dep/tinyxml
    ${SDL_INCLUDE_DIR}
    ${SDL_NET_INCLUDE_DIRS}    
)

add_executable(BombermaaanNet ${BOMBERMAAAN_NET_HEADERS} ${BOMBERMAAAN_NET_SOURCES})

target_link_libraries(BombermaaanNet
    ${SDL_LIBRARY}
    ${SDLNET_LIBRARY}
    tinyxml
)


