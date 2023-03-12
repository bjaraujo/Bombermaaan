
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
CHelp.h
CHurryMessage.h
CInput.h
CInputSDL.h
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
CSnowManager.h
CVideo.h
CVideoSDL.h
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
CHelp.cpp
CHurryMessage.cpp
CInput.cpp
CInputSDL.cpp
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
CSnowManager.cpp
CSound.cpp
CTitle.cpp
CTeam.cpp
CVictory.cpp
CVideo.cpp
CVideoSDL.cpp
CWall.cpp
CWindow.cpp
CWinner.cpp
StdAfx.cpp
WinMain.cpp
Bombermaaan.rc
)

set(BOMBERMAAAN_DX_HEADERS
CInputDX.h
CVideoDX.h
)

set(BOMBERMAAAN_DX_SOURCES
CInputDX.cpp
CVideoDX.cpp
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

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

if(USE_ALLEGRO)
  find_package(Allegro4 REQUIRED)
endif()

if(NETWORK_MODE)
  find_package(SDL_net REQUIRED)
endif()

if(CRASH_REPORT)
  if(WIN32)
    find_package(CrashRpt)
  endif()
endif()

set(BOMBERMAAAN_INCLUDE_DIRS
  ${SDL2_INCLUDE_DIR}
  ${SDL2_MIXER_INCLUDE_DIR}
)

if(USE_ALLEGRO)
  list(APPEND BOMBERMAAAN_INCLUDE_DIRS ${ALLEGRO_INCLUDE_DIR})    
  add_definitions(-DNOGDI)
endif()

if(NETWORK_MODE)
    list(APPEND BOMBERMAAAN_INCLUDE_DIRS ${SDL_NET_INCLUDE_DIRS})    
endif()

if(CRASH_REPORT)
    list(APPEND BOMBERMAAAN_INCLUDE_DIRS ${CRASHRPT_INCLUDE_DIR})    
endif()

include_directories(
  ${BOMBERMAAAN_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/../res
  ${CMAKE_CURRENT_SOURCE_DIR}/../dep/tinyxml
  ${CMAKE_CURRENT_SOURCE_DIR}/../dep/simpleini
)

set(BOMBERMAAAN_LIBRARIES
  ${SDL2_LIBRARIES}
  SDL2_mixer::SDL2_mixer
  tinyxml
)

add_definitions(-DTIXML_USE_STL -D_CRT_SECURE_NO_WARNINGS)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

if(MINGW)
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(CMAKE_RC_COMPILE_OBJECT
    "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()

if(USE_SDL)
  add_definitions(-DSDL)
endif(USE_SDL)

if(USE_ALLEGRO)
  add_definitions(-DALLEGRO)
  list(APPEND BOMBERMAAAN_LIBRARIES ${ALLEGRO_LIBRARY})    
endif()

if(USE_DIRECTX)
  add_definitions(-DDIRECTX -DDIRECTX_INPUT)
  list(APPEND BOMBERMAAAN_HEADERS ${BOMBERMAAAN_DX_HEADERS})
  list(APPEND BOMBERMAAAN_SOURCES ${BOMBERMAAAN_DX_SOURCES})
  list(APPEND BOMBERMAAAN_LIBRARIES ddraw dinput8 dxguid)    
endif()

if(ENABLE_LOG)
  add_definitions(-DENABLE_LOG)
endif()

if(ENABLE_DEBUG_LOG)
  add_definitions(-DENABLE_DEBUG_LOG)
endif()

if(ENABLE_DEBUG_KEYS)
  add_definitions(-DENABLE_DEBUG_KEYS)
endif()

if(LOAD_RESOURCES_FROM_FILES)
  add_definitions(-DLOAD_RESOURCES_FROM_FILES)
endif()

if(NETWORK_MODE)
  add_definitions(-DNETWORK_MODE)
  list(APPEND BOMBERMAAAN_HEADERS CNetwork.h)
  list(APPEND BOMBERMAAAN_SOURCES CNetwork.cpp)
  list(APPEND BOMBERMAAAN_LIBRARIES ${SDLNET_LIBRARY})    
endif()

if(CRASH_REPORT)
    add_definitions(-DCRASH_REPORT)
    list(APPEND BOMBERMAAAN_LIBRARIES ${CRASHRPT_LIBRARY})    
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

if(WIN32)
  add_executable(Bombermaaan WIN32 ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_SOURCES})
else()
  add_executable(Bombermaaan ${BOMBERMAAAN_HEADERS} ${BOMBERMAAAN_SOURCES} ${WINREPLACE_HEADERS} ${WINREPLACE_SOURCES})
endif()

target_link_libraries(Bombermaaan 
  ${BOMBERMAAAN_LIBRARIES}
)

find_package(PythonInterp)

if(WIN32)
  add_dependencies(Bombermaaan Bombermaaan32)
  add_custom_command(TARGET Bombermaaan PRE_BUILD COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/IncVersion.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../scripts)
endif()

