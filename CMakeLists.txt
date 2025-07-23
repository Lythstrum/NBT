cmake_minimum_required(VERSION 3.20)
project(NBT)

set(CMAKE_CXX_STANDARD 17)
set(LIBS_PATH_NBT ${PROJECT_SOURCE_DIR}/libs)

option(NBT_COMPILE_TESTS "compile the all tests or not." OFF)
option(NBT_SHARED "compile the library as shared." OFF)

file(GLOB_RECURSE LIB_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if(NOT NBT_SHARED)
	add_library(NBT STATIC ${LIB_FILES})
else()
	add_library(NBT SHARED ${LIB_FILES})
endif()

if(NOT TARGET BinaryStream)
	add_subdirectory(${LIBS_PATH_NBT}/CppBinaryStream)
endif()

target_include_directories(NBT PUBLIC
	${PROJECT_SOURCE_DIR}/include
	${LIBS_PATH_NBT}/CppBinaryStream/include
)

target_link_libraries(NBT PRIVATE BinaryStream)

if(NBT_COMPILE_TESTS)
	set(NBT_SAMPLES_HELPER_PATH ${LIBS_PATH_NBT}/SamplesHelper)
	set(PARENT_INC_DIR_NBT ${PROJECT_SOURCE_DIR}/include)
	include(${NBT_SAMPLES_HELPER_PATH}/SamplesHelper.cmake)
	initialize_samples_helper(${NBT_SAMPLES_HELPER_PATH} ${PROJECT_SOURCE_DIR}/samples)
	add_all_samples()
	unset(NBT_SAMPLES_HELPER_PATH)
	unset(PARENT_INC_DIR_NBT)
endif()
