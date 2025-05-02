cmake_minimum_required(VERSION 3.12)

project(
  trash
  VERSION 1.0.0.0
  LANGUAGES C
)

file(
  GLOB_RECURSE 
  SRC_FILES 
  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(
  ${PROJECT_NAME}
  ${SRC_FILES}
)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/gen_headers.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

target_include_directories(
  ${PROJECT_NAME} 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include
)