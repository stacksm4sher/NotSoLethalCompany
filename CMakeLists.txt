cmake_minimum_required(VERSION 3.24)
project(NotSoLethalCompany C)

set(CMAKE_C_STANDARD 11)
#set(
#        EXECUTABLE_OUTPUT_PATH
#        ${CMAKE_CURRENT_SOURCE_DIR}/bin
#)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_library(
        NotSoLethalCompany
        SHARED
        src/main.c
        src/esp.c
        src/gui.c
        src/util.c
)

target_link_directories(
        NotSoLethalCompany
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(
        NotSoLethalCompany
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/MinHook/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/logger/include
)

find_library(
        MinHook
        MinHook.dll
        ${CMAKE_SOURCE_DIR}/lib/bin
)

target_link_libraries(
        NotSoLethalCompany
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/MinHook/bin/MinHook.x64.dll
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/logger/bin/liblogger.dll
)