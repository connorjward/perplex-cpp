include(GoogleTest)

add_executable(testperplexcpp c_interface.cc wrapper.cc)

target_include_directories(testperplexcpp PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(testperplexcpp perplexcpp
                      gtest gtest_main)

# copy test files to build directory
file(
  COPY ${PROJECT_SOURCE_DIR}/data/test
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data
)

gtest_discover_tests(testperplexcpp
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
