# CMakeList.txt : CMake project for CPPAnimation, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("UpstreamGLib")

include("C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

SET(OPTIONS "")

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)

find_package(glfw3 3.3 REQUIRED)
add_definitions(-DGLFW_INCLUDE_NONE)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB SRC_FILES 
${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/**/**/*.cpp)

include_directories(
${OPENGL_INCLUDE_DIR} 
${GLM_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/src
${CMAKE_CURRENT_SOURCE_DIR}/GLAD/include)

file(GLOB SRC_GLAD 
${CMAKE_CURRENT_SOURCE_DIR}/GLAD/src/glad.c)

# set(ASSIMP_BUILD_STATIC_LIB ON)

IF(MSVC)
    SET(OPTIONS WIN32)
ENDIF(MSVC)

include(CTest)
enable_testing()

add_executable (${PROJECT_NAME} ${OPTIONS} "main.cpp" ${SRC_GLAD} ${SRC_FILES})

include(CPack)

#add_library(${PROJECT_NAME} STATIC "main.cpp" ${SRC_GLAD} ${SRC_FILES})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-DGLR_PROFILE -DGLR_DEBUG)
target_link_libraries(${PROJECT_NAME} PRIVATE debug assimp::assimp)
else()
add_definitions(-DGLR_PROFILE -DGLR_DEBUG)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw imgui::imgui cereal reactphysics3d)

set(${PROJECT_NAME}_RESOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    CACHE INTERNAL "${PROJECT_NAME}: Resources Directories" FORCE)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
