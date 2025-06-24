cmake_minimum_required(VERSION 3.20)
project(writing_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(writing_test ${PROJECT_SOURCE_DIR}/main.cpp)
target_include_directories(writing_test PRIVATE ${PARENT_INC_DIR_NBT})
target_link_libraries(writing_test PRIVATE NBT)
