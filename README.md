# PerpleX-cpp

This repository contains a C++ wrapper for the thermodynamic code [Perple_X](perplex.ethz.ch) (written in Fortran 77).


## Prerequisites

- CMake version `2.8.12+`

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


## Perple_X data files

Two Perple_X data sets are provided in the repository, both modelling KLB-1 peridotite.
The `klb-1` data set was taken from [here](http://www.perplex.ethz.ch/perplex/examples/example_holland_et_al_2018_melt_model/).
The `simple` data set is practically the same but has been altered to reduce the complexity and run time.

## Project layout

	data/		data files
	extern/perplex	Perple_X source code
	include/	header files
	src/		source code
	test/		unit tests

