cmake_minimum_required(VERSION 3.11)

project(perplex-cpp LANGUAGES CXX Fortran)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

add_subdirectory(extern/perplex)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(test)
endif()
