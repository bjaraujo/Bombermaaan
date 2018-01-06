#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)

#Name your project here
project(Bombermaaan)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(GRAPHICS_LIBRARY_OPTION "SDL" CACHE STRING "Graphics library")
set(GRAPHICS_LIBRARY_OPTION_VALUES "SDL" "Alegro" "DirectX")
set_property(CACHE GRAPHICS_LIBRARY_OPTION PROPERTY STRINGS ${GRAPHICS_LIBRARY_OPTION_VALUES})

list(FIND GRAPHICS_LIBRARY_OPTION_VALUES ${GRAPHICS_LIBRARY_OPTION} GRAPHICS_LIBRARY_OPTION_INDEX)

if(${GRAPHICS_LIBRARY_OPTION_INDEX} EQUAL 1)
  set(USE_ALEGRO ON CACHE BOOL "Alegro" FORCE)  
endif()

if(${GRAPHICS_LIBRARY_OPTION_INDEX} EQUAL 2)
  set(USE_DIRECTX ON CACHE BOOL "DirectX" FORCE)  
endif()
 
set(LOAD_RESOURCES_FROM_FILES OFF CACHE BOOL "Load Resources From Files")
set(NETWORK_MODE OFF CACHE BOOL "Network Mode")
set(CRASH_REPORT OFF CACHE BOOL "Crash Report")

add_subdirectory(dep)

if(WIN32)
add_subdirectory(res)
else(WIN32)
add_subdirectory(resgen)
endif(WIN32)

#add_subdirectory(net)

add_subdirectory(src)
