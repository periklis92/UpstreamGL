cmake_minimum_required (VERSION 3.8)

project("UpstreamGL")

if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

if (MSVC)
include("C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

if(WIN32) 
list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\ReactPhysics3D")
list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\box2d")
endif()

option(UPGL_PROFILING "Enable/Disable Profiling" ON)

set(OpenGL_GL_PREFERENCE "GLVND")

#dependencies
find_package(ReactPhysics3D REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_definitions(-DGLFW_INCLUDE_NONE)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

# Include sub-projects.
add_subdirectory("libraries")
add_subdirectory("UpstreamGL")
add_subdirectory("tests")
add_subdirectory("3DExample")

include(CTest)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/UpstreamGL/include/UpstreamGL"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)