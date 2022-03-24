enable_testing()

set(CTEST_OUTPUT_ON_FAILURE 1)
link_libraries(glm UpstreamGL imgui)
include_directories(".")

add_executable(id "IdTest.cpp" "Base/Assert.h")
add_executable(block_list "BlockListTest.cpp" "Base/Assert.h")

#Generates 1 million ids and checks for collisions
add_test(
  NAME
    "id_collisions"
  COMMAND 
    id 2000000 1
)

#Generates 10 thousand ids for 100 different seed values and checks for collisions
add_test(
  NAME
    "id_collisions_2"
  COMMAND 
    id 10000 100
)

add_test(
  NAME
    "block_list"
  COMMAND 
    block_list
)