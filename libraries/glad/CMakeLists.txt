project("glad")

message("glaaaaaaaaaaaaaad")

set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "${GLAD_INCLUDE_DIR}: glad" FORCE)
set(GLAD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c CACHE INTERNAL "${GLAD_SOURCE}: glad.c" FORCE)


# set(${PROJECT_NAME}_RESOURCE_DIR
#     ${CMAKE_CURRENT_SOURCE_DIR}/resources
#     CACHE INTERNAL "${PROJECT_NAME}: Resources Directories" FORCE)

# include_directories(${GLAD_INCLUDE_DIR})
# add_library(${PROJECT_NAME} ${GLAD_SOURCE} )