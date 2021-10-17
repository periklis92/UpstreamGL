# CMakeList.txt : CMake project for CPPAnimation, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("UpstreamGLib")

file(GLOB SRC_FILES 
${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/**/**/*.cpp)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${GLAD_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR} 
    ${GLM_INCLUDE_DIR}
)

include(CTest)
enable_testing()

add_library(${PROJECT_NAME} "STATIC" ${GLAD_SOURCE} ${SRC_FILES})

include(CPack)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-DGLR_PROFILE -DGLR_DEBUG)
target_link_libraries(${PROJECT_NAME} PUBLIC debug ${ASSIMP_LIB})
else()
add_definitions(-DGLR_PROFILE)
target_link_libraries(${PROJECT_NAME} PUBLIC ${ASSIMP_LIB})
endif()

if (UNIX)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})
endif()

target_link_libraries(${PROJECT_NAME} glm OpenGL::GL imgui glfw cereal stb ReactPhysics3D::ReactPhysics3D)
target_include_directories(${PROJECT_NAME} PUBLIC ${RAPIDXML_INCLUDE_DIRS})

target_include_directories( ${PROJECT_NAME} PUBLIC
                            "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/UpstreamGLib/include>"
                            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set(UPGL_LIBRARIES "${PROJECT_NAME} glfw ${GLFW_LIBRARIES}" CACHE STRING "")