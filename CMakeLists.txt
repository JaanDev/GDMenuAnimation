cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
project(MenuAnimation VERSION 1.0.0)

file(GLOB_RECURSE SOURCES
    libraries/ini/ini.cpp
    src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set(CMAKE_SIZEOF_VOID_P 4)

target_include_directories(${PROJECT_NAME} PRIVATE
	libraries/minhook/include
	libraries/gd.h/include
	libraries/gd.h/
    libraries/ini
)

add_subdirectory(libraries/cocos-headers)
add_subdirectory(libraries/minhook)

target_link_libraries(${PROJECT_NAME} minhook cocos2d)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "D:\\SteamLibrary\\steamapps\\common\\Geometry Dash Mods Testing\\adaf-dll"
)