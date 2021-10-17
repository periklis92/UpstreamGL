cmake_minimum_required (VERSION 3.8)
project("UpstreamGLExample")

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${GLM_INCLUDE_DIR}
)

include(CPack)

if(MSVC)
    SET(OPTIONS WIN32)
    set(ASSIMP_LIB assimp::assimp)
else()
    set(ASSIMP_LIB assimp)
endif(MSVC)

add_executable(${PROJECT_NAME} ${OPTIONS} main.cpp testMenu.cpp CameraController.h)
target_link_libraries(${PROJECT_NAME} glm UpstreamGL imgui)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-DUPGL_PROFILE -DUPGL_DEBUG)
target_link_libraries(${PROJECT_NAME} debug ${ASSIMP_LIB})
else()
add_definitions(-DUPGL_PROFILE)
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB})
endif()

set(${PROJECT_NAME}_RESOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    CACHE INTERNAL "${PROJECT_NAME}: Resources Directories" FORCE)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
