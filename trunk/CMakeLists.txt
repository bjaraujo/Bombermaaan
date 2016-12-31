#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)

#Name your project here
project(Bombermaaan)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(USE_DIRECTX OFF CACHE BOOL "Use DirectX")
set(LOAD_RESOURCES_FROM_FILES OFF CACHE BOOL "Load Resources From Files")
set(NETWORK_MODE OFF CACHE BOOL "Network Mode")

add_subdirectory(dep)

if(WIN32)
add_subdirectory(res)
else(WIN32)
add_subdirectory(resgen)
endif(WIN32)

#add_subdirectory(net)

add_subdirectory(src)
