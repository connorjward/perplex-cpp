include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

add_executable(testperplex c_interface.cc state.cc)

target_include_directories(testperplex PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(testperplex perplex gtest gtest_main)

# copy test files to build directory
file(
  COPY ${PROJECT_SOURCE_DIR}/data/test/simple
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/test
)

include(GoogleTest)
gtest_discover_tests(
  testperplex
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/test/simple
)

