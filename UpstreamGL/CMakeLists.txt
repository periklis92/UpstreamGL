# CMakeList.txt : CMake project for CPPAnimation, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("UpstreamGL")

include(GNUInstallDirs)

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

if(NOT WIN32)
target_compile_options(${PROJECT_NAME} PRIVATE  -fPIC)
endif()

include(CPack)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DUPGL_DEBUG")

list(APPEND UPGL_DEPENDENCIES assimp::assimp glm OpenGL::GL imgui glfw stb ReactPhysics3D::ReactPhysics3D)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(${PROJECT_NAME} PRIVATE debug ${UPGL_DEPENDENCIES})
else()
target_link_libraries(${PROJECT_NAME} PRIVATE ${UPGL_DEPENDENCIES})
endif()

if (UNIX)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

#target_include_directories(${PROJECT_NAME} PRIVATE ${RAPIDXML_INCLUDE_DIRS})

if(UPGL_PROFILING)
target_compile_definitions(${PROJECT_NAME} PUBLIC UPGL_PROFILE)
endif()

target_include_directories( ${PROJECT_NAME} PUBLIC
                            "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/UpstreamGL/include>"
                            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(
    TARGETS ${PROJECT_NAME}
    EXPORT upglConfig
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# install(
#   EXPORT upglConfig
#   NAMESPACE upgl::
#   DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/upgl"
# )