
PROJECT(BomberMaan)

SET(BOMBERMAAN_HEADERS
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

SET(BOMBERMAAN_SOURCES
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
FIND_PACKAGE(DirectX REQUIRED COMPONENTS ddraw dxgui dinput)

SOURCE_GROUP("Header Files" FILES ${BOMBERMAAN_HEADERS})
SOURCE_GROUP("Source Files" FILES ${BOMBERMAAN_SOURCES})

SET(WINSDK_INCLUDE_DIR "C:/Program Files/Microsoft SDKs/Windows/v7.1/Include" CACHE FILEPATH "C:/Program Files/Microsoft SDKs/Windows/v7.1/Include")

INCLUDE_DIRECTORIES(
${WINSDK_INCLUDE_DIR}
${DirectX_D3D9_INCLUDE_DIR}
${SDL_INCLUDE_DIR}
${SDL_MIXER_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR}/../res
${CMAKE_CURRENT_SOURCE_DIR}/../dep/tinyxml
${CMAKE_CURRENT_SOURCE_DIR}/../dep/simpleini
)

ADD_DEFINITIONS(-DTIXML_USE_STL -D_CRT_SECURE_NO_WARNINGS)

ADD_EXECUTABLE(BomberMaan WIN32 ${BOMBERMAAN_HEADERS} ${BOMBERMAAN_SOURCES})

TARGET_LINK_LIBRARIES(BomberMaan 
ws2_32
tinyxml
${DirectX_DDRAW_LIBRARY}
${DirectX_DINPUT_LIBRARY}
${DirectX_DXGUID_LIBRARY}
${SDL_LIBRARY}
${SDL_MIXER_LIBRARIES}
)
