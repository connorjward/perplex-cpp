set(perplex_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/perplex 
    CACHE FILEPATH "Path to Perple_X sources")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  include(FetchContent)
  FetchContent_Declare(googletest
                       GIT_REPOSITORY https://github.com/google/googletest.git
                       GIT_TAG        release-1.8.0)

  FetchContent_GetProperties(googletest)

  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
  endif()
endif()

# set_target_properties(perplex-orig PROPERTIES POSITION_INDEPENDENT_CODE True)

# target_compile_options(
#   perplex-orig 
#   PRIVATE 
#   -cpp			  # enable preprocessing 
#   -ffixed-line-length-132 # increase line length so prefix doesn't break compilation
#   )

# # add a prefix to conflicting function names
# target_compile_definitions(
#   perplex-orig
#   PRIVATE 
#   dgemv=perplex_dgemv # conflicts with BLAS dgemv
#   )
