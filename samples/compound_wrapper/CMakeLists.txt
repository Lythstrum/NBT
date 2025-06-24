cmake_minimum_required(VERSION 3.20)
project(compound_wrapper_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(compound_wrapper_test ${PROJECT_SOURCE_DIR}/main.cpp)
target_include_directories(compound_wrapper_test PRIVATE ${PARENT_INC_DIR_NBT})
target_link_libraries(compound_wrapper_test PRIVATE NBT)
