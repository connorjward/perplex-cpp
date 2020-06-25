option(ALLOW_PERPLEX_OUTPUT OFF)

# Set the Fortran compilation flags. '-cpp' is required to enable the 
# preprocessor and '-ffixed-line-length-132' increases the maximum line
# length for fixed-form Fortran code so adding any prefixes does not
# break the code.
set(CMAKE_Fortran_FLAGS 
    "${CMAKE_Fortran_FLAGS} -cpp -ffixed-line-length-132 -std=legacy")

add_library(
  perplexcpp SHARED

  c_interface.f
  utils.cc 
  wrapper.cc 

  ${perplex_SOURCE_DIR}/BLASlib.f
  ${perplex_SOURCE_DIR}/flib.f
  ${perplex_SOURCE_DIR}/nlib.f
  ${perplex_SOURCE_DIR}/olib.f
  ${perplex_SOURCE_DIR}/resub.f
  ${perplex_SOURCE_DIR}/rlib.f
  ${perplex_SOURCE_DIR}/tlib.f
)

target_include_directories(
  perplexcpp 
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${perplex_SOURCE_DIR}
)

if(ALLOW_PERPLEX_OUTPUT)
  target_compile_definitions(perplexcpp PUBLIC ALLOW_PERPLEX_OUTPUT)
endif()

# Add a prefix to the following conflicting function names:
# - dgemv (conflicts with the BLAS implementation of dgemv)
target_compile_definitions(perplexcpp
                           PRIVATE dgemv=perplexcpp_dgemv)
