cmake_minimum_required(VERSION 3.0.0)
project(luaTest VERSION 0.1.0)

INCLUDE_DIRECTORIES(
    3rdparty
    3rdparty/lua
)

set(CMAKE_CXX_STANDARD 17)

set(LUATESTSOURCES
    src/main.cpp
    src/npc.cpp
    src/ScriptInterface.cpp
)

add_executable(luaTest ${LUATESTSOURCES})
target_precompile_headers(luaTest PRIVATE src/pch.h)

if (WIN32)
    find_library(
        LUA_LIBRARY 
        NAMES lua54
        PATHS "${CMAKE_SOURCE_DIR}/3rdparty/lua/lib/Win32"
    )
    TARGET_LINK_LIBRARIES(
        luaTest 
		PUBLIC
        ${LUA_LIBRARY}
    )
elseif (UNIX)
    find_library(
        LUA_LIBRARY 
        NAMES lua54
        PATHS "${CMAKE_SOURCE_DIR}/3rdparty/lua/lib/linux"
    )
    TARGET_LINK_LIBRARIES(
        luaTest 
        PUBLIC 
        ${LUA_LIBRARY}
        ${CMAKE_DL_LIBS}
    )
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
