add_library(
  perplex-orig
  OBJECT
  BLASlib.f
  flib.f
  nlib.f
  olib.f
  resub.f
  rlib.f
  tlib.f
)

set_target_properties(perplex-orig PROPERTIES POSITION_INDEPENDENT_CODE True)

target_compile_options(
  perplex-orig 
  PRIVATE 
  -cpp			  # enable preprocessing 
  -ffixed-line-length-132 # increase line length so prefix doesn't break compilation
)

# add a prefix to conflicting function names
target_compile_definitions(
  perplex-orig
  PRIVATE 
  dgemv=perplex_dgemv # conflicts with BLAS dgemv
)
		       
set(perplex-orig_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} 
    CACHE PATH "Location of perplex-orig source files")
