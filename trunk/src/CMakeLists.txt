
PROJECT(BomberMaaan)

SET(BOMBERMAAAN_HEADERS
BombermaaanIco.h
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
CDirectDraw.h
CDirectInput.h
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
CMenuYesNo.h
CModeScreen.h
CMosaic.h
CNetwork.h
COptions.h
CPauseMessage.h
CPlayerInput.h
CRandomMosaic.h
CScores.h
CScroller.h
CSDLInput.h
CSDLVideo.h
CSound.h
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
CDirectDraw.cpp
CDirectInput.cpp
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
CMenuYesNo.cpp
CModeScreen.cpp
CMosaic.cpp
CNetwork.cpp
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
CVictory.cpp
CWall.cpp
CWindow.cpp
CWinner.cpp
StdAfx.cpp
WinMain.cpp
Bombermaaan.rc
)

FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_mixer REQUIRED)

SOURCE_GROUP("Header Files" FILES ${BOMBERMAAAN_HEADERS})
SOURCE_GROUP("Source Files" FILES ${BOMBERMAAAN_SOURCES})

INCLUDE_DIRECTORIES(
    ${SDL_INCLUDE_DIR}
    ${SDL_MIXER_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../res
    ${CMAKE_CURRENT_SOURCE_DIR}/../dep/tinyxml
    ${CMAKE_CURRENT_SOURCE_DIR}/../dep/simpleini
)

ADD_DEFINITIONS(-DTIXML_USE_STL -DDIRECTX -D_CRT_SECURE_NO_WARNINGS)

IF(WIN32)
    ADD_EXECUTABLE(BomberMaaan WIN32 ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_SOURCES})
ELSE(WIN32)
    ADD_EXECUTABLE(BomberMaaan ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_SOURCES})
ENDIF(WIN32)

IF(WIN32)
    TARGET_LINK_LIBRARIES(BomberMaaan 
        ws2_32
		ddraw
		dinput8
		dxguid
        tinyxml
        ${SDL_LIBRARY}
        ${SDL_MIXER_LIBRARIES}
    )
ELSE(WIN32)
    TARGET_LINK_LIBRARIES(BomberMaaan 
        tinyxml
        ${SDL_LIBRARY}
        ${SDL_MIXER_LIBRARIES}
    )
ENDIF(WIN32)

