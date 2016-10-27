
PROJECT(Bombermaaan)

SET(BOMBERMAAAN_HEADERS
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

SET(BOMBERMAAAN_SOURCES
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

SET(BOMBERMAAAN_DX_HEADERS
CDirectDraw.h
CDirectInput.h
)

SET(BOMBERMAAAN_DX_SOURCES
CDirectDraw.cpp
CDirectInput.cpp
)

SET(BOMBERMAAAN_NET_HEADERS
CNetwork.h
)

SET(BOMBERMAAAN_NET_SOURCES
CNetwork.cpp
)

SET(WINREPLACE_HEADERS
WinReplace.h
)

SET(WINREPLACE_SOURCES
WinReplace.cpp
)

SOURCE_GROUP("Header Files" FILES ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_DX_HEADERS} ${BOMBERMAAAN_NET_HEADERS} ${WINREPLACE_HEADERS})
SOURCE_GROUP("Source Files" FILES ${BOMBERMAAAN_SOURCES} ${BOMBERMAAAN_DX_SOURCES} ${BOMBERMAAAN_NET_SOURCES} ${WINREPLACE_SOURCES})

FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_mixer REQUIRED)

IF(NETWORK_MODE)
    FIND_PACKAGE(SDL_net REQUIRED)
ENDIF(NETWORK_MODE)

IF(NETWORK_MODE)
    INCLUDE_DIRECTORIES(
        ${SDL_INCLUDE_DIR}
        ${SDL_MIXER_INCLUDE_DIRS}
        ${SDL_NET_INCLUDE_DIRS}    
    )
ELSE(NETWORK_MODE)
    INCLUDE_DIRECTORIES(
        ${SDL_INCLUDE_DIR}
        ${SDL_MIXER_INCLUDE_DIRS}
    )
ENDIF(NETWORK_MODE)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/../res
    ${CMAKE_CURRENT_SOURCE_DIR}/../dep/tinyxml
    ${CMAKE_CURRENT_SOURCE_DIR}/../dep/simpleini
)

SET (BOMBERMAAAN_LIBRARIES
    ${SDL_LIBRARY}
    ${SDLMIXER_LIBRARY}
    tinyxml
)

ADD_DEFINITIONS(-DTIXML_USE_STL -D_CRT_SECURE_NO_WARNINGS)

IF(USE_DIRECTX)
    ADD_DEFINITIONS(-DDIRECTX_DRAW -DDIRECTX_INPUT)
    LIST(APPEND BOMBERMAAAN_HEADERS CDirectDraw.h CDirectInput.h)
    LIST(APPEND BOMBERMAAAN_SOURCES CDirectDraw.cpp CDirectInput.cpp)
    LIST(APPEND BOMBERMAAAN_LIBRARIES ddraw dinput8 dxguid)    
ENDIF(USE_DIRECTX)

IF(LOAD_RESOURCES_FROM_FILES)
    ADD_DEFINITIONS(-DLOAD_RESOURCES_FROM_FILES)
ENDIF(LOAD_RESOURCES_FROM_FILES)

IF(NETWORK_MODE)
    ADD_DEFINITIONS(-DNETWORK_MODE)
    LIST(APPEND BOMBERMAAAN_HEADERS CNetwork.h)
    LIST(APPEND BOMBERMAAAN_SOURCES CNetwork.cpp)
    LIST(APPEND BOMBERMAAAN_LIBRARIES ${SDLNET_LIBRARY})    
ENDIF(NETWORK_MODE)

IF(WIN32)
    ADD_EXECUTABLE(Bombermaaan WIN32 ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_SOURCES})
ELSE(WIN32)
    ADD_EXECUTABLE(Bombermaaan ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_SOURCES} ${WINREPLACE_HEADERS} ${WINREPLACE_SOURCES})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(Bombermaaan 
    ${BOMBERMAAAN_LIBRARIES}
)

IF(WIN32)
    ADD_DEPENDENCIES(Bombermaaan Bombermaaan32)
    ADD_CUSTOM_COMMAND(TARGET Bombermaaan PRE_BUILD COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/IncVersion.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../scripts)
ENDIF(WIN32)

