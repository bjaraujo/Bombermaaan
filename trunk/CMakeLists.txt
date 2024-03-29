#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 3.0)

#Name your project here
project(Bombermaaan)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(GRAPHICS_LIBRARY_OPTION "SDL" CACHE STRING "Graphics library")
set(GRAPHICS_LIBRARY_OPTION_VALUES "SDL" "Allegro" "DirectX")
set_property(CACHE GRAPHICS_LIBRARY_OPTION PROPERTY STRINGS ${GRAPHICS_LIBRARY_OPTION_VALUES})

list(FIND GRAPHICS_LIBRARY_OPTION_VALUES ${GRAPHICS_LIBRARY_OPTION} GRAPHICS_LIBRARY_OPTION_INDEX)
    
if(${GRAPHICS_LIBRARY_OPTION_INDEX} EQUAL 0)
  message("Use SDL only")
  set(USE_SDL ON INTERNAL FORCE) 
elseif(${GRAPHICS_LIBRARY_OPTION_INDEX} EQUAL 1)
  message("Use Allegro")
  set(USE_ALLEGRO ON INTERNAL FORCE) 
elseif(${GRAPHICS_LIBRARY_OPTION_INDEX} EQUAL 2)
  message("Use DirectX")
  set(USE_DIRECTX ON INTERNAL FORCE) 
endif()
 
set(ENABLE_LOG OFF CACHE BOOL "Enable logging")
set(ENABLE_DEBUG_LOG OFF CACHE BOOL "Enable debug logging")
set(ENABLE_DEBUG_KEYS OFF CACHE BOOL "Enable debug keys")
set(ENABLE_ASAN OFF CACHE BOOL "Enable asan support")
set(LOAD_RESOURCES_FROM_FILES ON CACHE BOOL "Load resources from files")
set(NETWORK_MODE OFF CACHE BOOL "Network mode")
set(CRASH_REPORT OFF CACHE BOOL "Crash report")

if(ENABLE_ASAN)
  # AddressSanitizer
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif() 

add_subdirectory(dep)

if(WIN32)
  add_subdirectory(res)
else()
  if(NOT LOAD_RESOURCES_FROM_FILES)
    add_subdirectory(resgen)
  endif()
endif()

#add_subdirectory(net)
add_subdirectory(src)

install(TARGETS Bombermaaan DESTINATION ".")
install(FILES config/config.xml DESTINATION ".")
install(DIRECTORY levels DESTINATION ".")
install(DIRECTORY res/images DESTINATION ".")
install(DIRECTORY res/sounds DESTINATION ".")


