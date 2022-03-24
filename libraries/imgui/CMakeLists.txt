
cmake_minimum_required (VERSION 3.8)
project("imgui")

set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${IMGUI_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

file(GLOB IMGUI_SRC 
${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} STATIC ${IMGUI_SRC})
target_link_libraries(${PROJECT_NAME} glfw)

if(NOT WIN32)
target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)