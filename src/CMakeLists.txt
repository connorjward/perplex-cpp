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

set(CMAKE_Fortran_FLAGS 
    "${CMAKE_Fortran_FLAGS} -ffixed-line-length-132")
