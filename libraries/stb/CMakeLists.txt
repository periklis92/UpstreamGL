
cmake_minimum_required (VERSION 3.8)
project("stb")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB STB_SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${PROJECT_NAME} STATIC ${STB_SRC})

if(NOT WIN32)
target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)