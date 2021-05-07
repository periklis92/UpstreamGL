
cmake_minimum_required (VERSION 3.8)
project("ImGui")

set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${IMGUI_INCLUDE_DIR})

file(GLOB IMGUI_SRC 
${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} ${IMGUI_SRC})
