cmake_minimum_required(VERSION 3.20)
project(endian_switch_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(endian_switch_test ${PROJECT_SOURCE_DIR}/main.cpp)
target_include_directories(endian_switch_test PRIVATE ${PARENT_INC_DIR_NBT})
target_link_libraries(endian_switch_test PRIVATE NBT)
