set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -ffixed-line-length-132")

add_library(
  perplex 
  SHARED
  c_interface.f
  utils.cc 
  wrapper.cc 
)

target_include_directories(
  perplex 
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${perplex-orig_SOURCE_DIR}
)

target_link_libraries(perplex PRIVATE perplex-orig)
