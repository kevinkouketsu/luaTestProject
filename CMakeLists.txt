cmake_minimum_required(VERSION 3.0.0)
project(luaTest VERSION 0.1.0)

INCLUDE_DIRECTORIES(
    3rdparty
    3rdparty/lua
)

if (WIN32)
    LINK_DIRECTORIES("3rdparty/lua/lib/Win32")
endif (WIN32)

add_executable(
    luaTest 
    main.cpp
    )
TARGET_LINK_LIBRARIES(
    luaTest 
    lua54
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
