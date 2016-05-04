#Specify the version being used aswell as the language
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Name your project here
PROJECT(BomberMaaan)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

SET(USE_DIRECTX OFF CACHE BOOL "Use DirectX")

ADD_SUBDIRECTORY(dep)
ADD_SUBDIRECTORY(res)
ADD_SUBDIRECTORY(src)

