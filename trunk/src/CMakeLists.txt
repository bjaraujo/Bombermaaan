
project(Bombermaaan)

set(BOMBERMAAAN_HEADERS
BombermaaanIco.h
Bombermaaan.h
CAiArena.h
CAiBomber.h
CAiManager.h
CArena.h
CArenaCloser.h
CArenaSnapshot.h
CBoard.h
CBomb.h
CBomber.h
CBomberMove.h
CClock.h
CCloudManager.h
CCommandChunk.h
CConsole.h
CControls.h
CCredits.h
CDebug.h
CDemo.h
CDisplay.h
CDrawGame.h
CElement.h
CExplosion.h
CFloor.h
CFont.h
CGame.h
CHurryMessage.h
CInput.h
CItem.h
CLevel.h
CLog.h
CMainInput.h
CMatch.h
CMenu.h
CMenuBase.h
CMenuBomber.h
CMenuInput.h
CMenuLevel.h
CMenuMatch.h
CMenuTeam.h
CMenuYesNo.h
CModeScreen.h
CMosaic.h
COptions.h
CPauseMessage.h
CPlayerInput.h
CRandomMosaic.h
CScores.h
CScroller.h
CSDLInput.h
CSDLVideo.h
CSound.h
CTeam.h
CTimer.h
CTitle.h
CVictory.h
CWall.h
CWindow.h
CWinner.h
resource.h
StdAfx.h
)

set(BOMBERMAAAN_SOURCES
CAiArena.cpp
CAiBomber.cpp
CAiManager.cpp
CArena.cpp
CArenaCloser.cpp
CArenaSnapshot.cpp
CBoard.cpp
CBomb.cpp
CBomber.cpp
CBomberMove.cpp
CClock.cpp
CCloudManager.cpp
CCommandChunk.cpp
CConsole.cpp
CControls.cpp
CCredits.cpp
CDebug.cpp
CDemo.cpp
CDisplay.cpp
CDrawGame.cpp
CElement.cpp
CExplosion.cpp
CFloor.cpp
CFont.cpp
CGame.cpp
CHurryMessage.cpp
CInput.cpp
CItem.cpp
CLevel.cpp
CLog.cpp
CMainInput.cpp
CMatch.cpp
CMenu.cpp
CMenuBase.cpp
CMenuBomber.cpp
CMenuInput.cpp
CMenuLevel.cpp
CMenuMatch.cpp
CMenuTeam.cpp
CMenuYesNo.cpp
CModeScreen.cpp
CMosaic.cpp
COptions.cpp
CPauseMessage.cpp
CPlayerInput.cpp
CRandomMosaic.cpp
CScores.cpp
CScroller.cpp
CSDLInput.cpp
CSDLVideo.cpp
CSound.cpp
CTitle.cpp
CTeam.cpp
CVictory.cpp
CWall.cpp
CWindow.cpp
CWinner.cpp
StdAfx.cpp
WinMain.cpp
Bombermaaan.rc
)

set(BOMBERMAAAN_DX_HEADERS
CDirectDraw.h
CDirectInput.h
)

set(BOMBERMAAAN_DX_SOURCES
CDirectDraw.cpp
CDirectInput.cpp
)

set(BOMBERMAAAN_NET_HEADERS
CNetwork.h
)

set(BOMBERMAAAN_NET_SOURCES
CNetwork.cpp
)

set(WINREPLACE_HEADERS
WinReplace.h
)

set(WINREPLACE_SOURCES
WinReplace.cpp
)

source_group("Header Files" FILES ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_DX_HEADERS} ${BOMBERMAAAN_NET_HEADERS} ${WINREPLACE_HEADERS})
source_group("Source Files" FILES ${BOMBERMAAAN_SOURCES} ${BOMBERMAAAN_DX_SOURCES} ${BOMBERMAAAN_NET_SOURCES} ${WINREPLACE_SOURCES})

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

if(NETWORK_MODE)
    find_package(SDL_net REQUIRED)
endif(NETWORK_MODE)

if(CRASH_REPORT)
    if(WIN32)
        find_package(CrashRpt)
    endif(WIN32)
endif(CRASH_REPORT)

if(NETWORK_MODE)
    include_directories(
        ${SDL_INCLUDE_DIR}
        ${SDL_MIXER_INCLUDE_DIRS}
        ${SDL_NET_INCLUDE_DIRS}    
    )
else(NETWORK_MODE)
    include_directories(
        ${SDL_INCLUDE_DIR}
        ${SDL_MIXER_INCLUDE_DIRS}
    )
endif(NETWORK_MODE)

if(CRASH_REPORT)
    include_directories(
        ${CRASHRPT_INCLUDE_DIR}
    )
endif(CRASH_REPORT)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../res
    ${CMAKE_CURRENT_SOURCE_DIR}/../dep/tinyxml
    ${CMAKE_CURRENT_SOURCE_DIR}/../dep/simpleini
)

set(BOMBERMAAAN_LIBRARIES
    ${SDL_LIBRARY}
    ${SDLMIXER_LIBRARY}
    tinyxml
)

add_definitions(-DTIXML_USE_STL -D_CRT_SECURE_NO_WARNINGS)

if(USE_DIRECTX)
    add_definitions(-DDIRECTX_DRAW -DDIRECTX_INPUT)
    list(APPEND BOMBERMAAAN_HEADERS CDirectDraw.h CDirectInput.h)
    list(APPEND BOMBERMAAAN_SOURCES CDirectDraw.cpp CDirectInput.cpp)
    list(APPEND BOMBERMAAAN_LIBRARIES ddraw dinput8 dxguid)    
endif(USE_DIRECTX)

if(LOAD_RESOURCES_FROM_FILES)
    add_definitions(-DLOAD_RESOURCES_FROM_FILES)
endif(LOAD_RESOURCES_FROM_FILES)

if(NETWORK_MODE)
    add_definitions(-DNETWORK_MODE)
    list(APPEND BOMBERMAAAN_HEADERS CNetwork.h)
    list(APPEND BOMBERMAAAN_SOURCES CNetwork.cpp)
    list(APPEND BOMBERMAAAN_LIBRARIES ${SDLNET_LIBRARY})    
endif(NETWORK_MODE)

if(CRASH_REPORT)
    add_definitions(-DCRASH_REPORT)
    list(APPEND BOMBERMAAAN_LIBRARIES ${CRASHRPT_LIBRARY})    
endif(CRASH_REPORT)

if(WIN32)
    add_executable(Bombermaaan WIN32 ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_SOURCES})
else(WIN32)
    add_executable(Bombermaaan ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_SOURCES} ${WINREPLACE_HEADERS} ${WINREPLACE_SOURCES})
endif(WIN32)

target_link_libraries(Bombermaaan 
    ${BOMBERMAAAN_LIBRARIES}
)

if(WIN32)
    add_dependencies(Bombermaaan Bombermaaan32)
    add_custom_command(TARGET Bombermaaan PRE_BUILD COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/IncVersion.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../scripts)
endif(WIN32)

