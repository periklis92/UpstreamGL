# CMakeList.txt : CMake project for CPPAnimation, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("UpstreamGLib")

if (MSVC)
include("C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

SET(OPTIONS "")

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp CONFIG REQUIRED)

# if (NOT ASSIMP_FOUND)
#     message( SEND_ERROR "ASSIMP NOT FOUND!" )
# endif()

find_package(glfw3 3.3 REQUIRED)
add_definitions(-DGLFW_INCLUDE_NONE)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB SRC_FILES 
${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/**/**/*.cpp)

message(glaaaaaaaad: ${GLAD_INCLUDE_DIR})

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/src
${GLAD_INCLUDE_DIR}
${IMGUI_INCLUDE_DIR}
${OPENGL_INCLUDE_DIR} 
${GLM_INCLUDE_DIR}
)

set(ASSIMP_BUILD_STATIC_LIB ON)

if(MSVC)
    SET(OPTIONS WIN32)
    set(ASSIMP_LIB assimp::assimp)
else()
    set(ASSIMP_LIB assimp)
endif(MSVC)

include(CTest)
enable_testing()

add_executable (${PROJECT_NAME} ${OPTIONS} "main.cpp" ${GLAD_SOURCE} ${SRC_FILES})

include(CPack)

#add_library(${PROJECT_NAME} STATIC "main.cpp" ${SRC_GLAD} ${SRC_FILES})



if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-DGLR_PROFILE -DGLR_DEBUG)
target_link_libraries(${PROJECT_NAME} PRIVATE debug ${ASSIMP_LIB})
else()
add_definitions(-DGLR_PROFILE -DGLR_DEBUG)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMP_LIB})
endif()

if (UNIX)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL imgui glfw cereal reactphysics3d stb)

set(${PROJECT_NAME}_RESOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    CACHE INTERNAL "${PROJECT_NAME}: Resources Directories" FORCE)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
