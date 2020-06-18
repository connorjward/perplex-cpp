# PerpleX-cpp

This repository contains a C++ wrapper for the thermodynamic code [Perple_X](perplex.ethz.ch) (written in Fortran 77).

## Prerequisites

- CMake version `3.11+`

- A Fortran 2003 compatible Fortran compiler (e.g. `gfortran`)

- A C++ 2011 compatible C++ compiler (e.g. `gcc`)

## Build instructions

An out of source build is recommended:

	mkdir build
	cd build
	cmake ..
	make -j<N>
	
## Testing

Unit testing is done using CMake and GoogleTest (installed as part of main build). 
To run the tests just run `ctest` from the root build directory.

## Project layout

	data/		data files
	extern/perplex	Perple_X source code
	include/	public header files
	src/		source code
	test/		unit tests
