cmake_minimum_required (VERSION 3.8)

if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()
project("UpstreamGL")

if (MSVC)
include("C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

if(WIN32) 
list(APPEND CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\ReactPhysics3D")
endif()

#dependencies
find_package(ReactPhysics3D REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_definitions(-DGLFW_INCLUDE_NONE)

find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

# Include sub-projects.
add_subdirectory("libraries")
add_subdirectory("UpstreamGL")
add_subdirectory("example")