cmake_minimum_required(VERSION 3.16)
project(inventory LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(inventory_lib
    src/inventory.cpp
    src/merge.cpp
)
target_include_directories(inventory_lib PUBLIC include)

add_executable(inventory src/main.cpp)
target_link_libraries(inventory PRIVATE inventory_lib)

add_executable(inventory_test tests/inventory_test.cpp)
target_include_directories(inventory_test PRIVATE tests)
target_link_libraries(inventory_test PRIVATE inventory_lib)

add_executable(merge_test tests/merge_test.cpp)
target_include_directories(merge_test PRIVATE tests)
target_link_libraries(merge_test PRIVATE inventory_lib)

add_library(review_exercise OBJECT exercises/review.cpp)
target_include_directories(review_exercise PRIVATE include)

enable_testing()
add_test(NAME inventory_test COMMAND inventory_test)

