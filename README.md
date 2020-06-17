# perplex-cpp

This repository contains a C++ wrapper for the thermodynamic code [Perple_X](perplex.ethz.ch) (written in Fortran 77).

## Prerequisites

- CMake version `3.11+`

- A Fortran 2003 compatible Fortran compiler (e.g. `gfortran`)

- A C++ 2011 compatible C++ compiler (e.g. `gcc`)

- Git

## Installation instructions

An out of source build is recommended:

	mkdir build
	cd build
	cmake ..
	make -j<N>
	
## Testing

Testing is done using CTest.

## Project layout

	data/		data files
	extern/perplex	Perple_X source code
	include/	public header files
	src/		source code
	test/		unit tests
