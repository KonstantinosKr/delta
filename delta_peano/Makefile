# This is a makefile for my PiT demonstrator. It is tailored to SuperMUC's thin
# islands. Please call it from within projects/particles/pit. Prior to usage,
# ensure you've invoked module load tbb.


# Include files
-include files.mk


# Set Paths
# ---------
# Please adopt these directories to your Peano installation. The values are
# used by the compiler and linker calls.
PROJECT_HOME = .


# Configure System
# ----------------
INCLUDE_TBB=$(TBB_INC)
INCLUDE_OMP=-fopenmp
INCLUDE_MPI=$(MPI_INC)


LINK_TBB=$(TBB_SHLIB)
LINK_OMP=$(OMP_SHLIB)
LINK_OMP=-fopenmp
LINK_MPI=

#COMPILER_CFLAGS=-std=c++0x -fast -fstrict-aliasing  -DMPICH_IGNORE_CXX_SEEK -fno-rtti -no-ipo -ip -xhost -wd603,3128

default: header
	@echo Please specify build target. Available targets are:
	@echo clean
	@echo dim3-gcc-asserts
	@echo dim3-gcc-release-novec
	@echo dim3-gcc-release-vec
	@echo dim3-gcc-release-omp
	@echo dim3-gcc-release-omp-triangle
	@echo dim3-gcc-release-omp-particle
	@echo dim3-gcc-release-omp-triangle-phi
	@echo dim3-gcc-asserts-tbb
	@echo dim3-gcc-release-tbb
	@echo dim3-gcc-release-tbb-omp
	@echo dim3-gcc-release-tbb-omp-triangle
	@echo dim3-gcc-release-tbb-omp-particle
	@echo dim3-gcc-release-tbb-omp-triangle-phi
	@echo dim3-icc-asserts
	@echo dim3-icc-release-novec
	@echo dim3-icc-release-vec
	@echo dim3-icc-release-omp
	@echo dim3-icc-release-omp-triangle
	@echo dim3-icc-release-omp-particle
	@echo dim3-icc-release-omp-triangle-phi
	@echo dim3-icc-asserts-tbb
	@echo dim3-icc-release-tbb
	@echo dim3-icc-release-tbb-omp
	@echo dim3-icc-release-tbb-omp-triangle
	@echo dim3-icc-release-tbb-omp-particle
	@echo dim3-icc-release-tbb-omp-triangle-phi

dim3-gcc-asserts: CC=g++ -std=c++0x
dim3-gcc-asserts: DIM=-DDim3
dim3-gcc-asserts: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DAsserts -DTrackGridStatistics
dim3-gcc-asserts: CFLAGS=-O0 -Wall -pedantic -pedantic-errors -debug
dim3-gcc-asserts: LFLAGS=
dim3-gcc-asserts: EXECUTABLE=dem-3d-asserts
dim3-gcc-asserts: header build

dim3-gcc-release-novec: CC=g++ -std=c++0x
dim3-gcc-release-novec: DIM=-DDim3
dim3-gcc-release-novec: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DTrackGridStatistics
dim3-gcc-release-novec: CFLAGS=-O0 -Wall -pedantic -pedantic-errors
dim3-gcc-release-novec: LFLAGS=
dim3-gcc-release-novec: EXECUTABLE=dem-3d-release-novec
dim3-gcc-release-novec: header build

dim3-gcc-release-vec: CC=g++ --std=c++11
dim3-gcc-release-vec: DIM=-DDim3
dim3-gcc-release-vec: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DTrackGridStatistics
dim3-gcc-release-vec: CFLAGS=-O3 -ftree-vectorizer-verbose=5 -fdump-tree-vect-details -fstrict-aliasing
dim3-gcc-release-vec: LFLAGS=-lrt
dim3-gcc-release-vec: EXECUTABLE=dem-3d-release-vec
dim3-gcc-release-vec: header build

dim3-gcc-release-omp-triangle: CC=g++ --std=c++11
dim3-gcc-release-omp-triangle: DIM=-DDim3
dim3-gcc-release-omp-triangle: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DompTriangle -DTrackGridStatistics
dim3-gcc-release-omp-triangle: CFLAGS=-O3 -ftree-vectorizer-verbose=7 -fdump-tree-vect-details -fstrict-aliasing
dim3-gcc-release-omp-triangle: LFLAGS=$(LINK_OMP) -lrt
dim3-gcc-release-omp-triangle: EXECUTABLE=dem-3d-release-omp-triangle
dim3-gcc-release-omp-triangle: header build

dim3-gcc-release-omp-triangle-phi: CC=g++ --std=c++11
dim3-gcc-release-omp-triangle-phi: DIM=-DDim3
dim3-gcc-release-omp-triangle-phi: SETTINGS=-DiREAL=double -DbyteAlignment=64 -DompTriangle -DTrackGridStatistics
dim3-gcc-release-omp-triangle-phi: CFLAGS=-mmic -O3 -ftree-vectorizer-verbose=7 -fdump-tree-vect-details -fstrict-aliasing
dim3-gcc-release-omp-triangle-phi: LFLAGS=$(LINK_OMP) -lrt -mmic
dim3-gcc-release-omp-triangle-phi: EXECUTABLE=dem-3d-release-omp-triangle-phi
dim3-gcc-release-omp-triangle-phi: header build

dim3-gcc-release-omp-particle: CC=g++ --std=c++11
dim3-gcc-release-omp-particle: DIM=-DDim3
dim3-gcc-release-omp-particle: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DompParticle -DTrackGridStatistics
dim3-gcc-release-omp-particle: CFLAGS=-O3 -ftree-vectorizer-verbose=7 -fdump-tree-vect-details -qopt-threads-per-core=1 -fstrict-aliasing
dim3-gcc-release-omp-particle: LFLAGS=$(LINK_OMP) -lrt
dim3-gcc-release-omp-particle: EXECUTABLE=dem-3d-release-omp-particle
dim3-gcc-release-omp-particle: header build

dim3-gcc-release-omp-particle-triangle: CC=g++ --std=c++11
dim3-gcc-release-omp-particle-triangle: DIM=-DDim3
dim3-gcc-release-omp-particle-triangle: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DompParticle -DompTriangle -DTrackGridStatistics
dim3-gcc-release-omp-particle-triangle: CFLAGS=-O3 -ftree-vectorizer-verbose=7 -fdump-tree-vect-details -qopt-threads-per-core=1 -fstrict-aliasing
dim3-gcc-release-omp-particle-triangle: LFLAGS=$(LINK_OMP) -lrt
dim3-gcc-release-omp-particle-triangle: EXECUTABLE=dem-3d-release-omp-particle-triangle
dim3-gcc-release-omp-particle-triangle: header build

dim3-gcc-release-tbb: CC=g++ --std=c++11
dim3-gcc-release-tbb: DIM=-DDim3
dim3-gcc-release-tbb: SETTINGS=-DiREAL=double -DSharedTBB -DbyteAlignment=32 -DTrackGridStatistics
dim3-gcc-release-tbb: CFLAGS=-O3 -ftree-vectorizer-verbose=7 -fdump-tree-vect-details -fstrict-aliasing $(INCLUDE_TBB)
dim3-gcc-release-tbb: LFLAGS=-lrt $(LINK_TBB)
dim3-gcc-release-tbb: EXECUTABLE=dem-3d-release-tbb
dim3-gcc-release-tbb: header build

dim3-gcc-release-tbb-omp-triangle: CC=g++ --std=c++11
dim3-gcc-release-tbb-omp-triangle: DIM=-DDim3
dim3-gcc-release-tbb-omp-triangle: SETTINGS=-DiREAL=double -DSharedTBB -DompTriangle -DTrackGridStatistics -DbyteAlignment=32
dim3-gcc-release-tbb-omp-triangle: CFLAGS=-O3 -qopenmp -ftree-vectorizer-verbose=7 -fdump-tree-vect-details -fstrict-aliasing $(INCLUDE_TBB)
dim3-gcc-release-tbb-omp-triangle: LFLAGS=-lrt $(LINK_OMP) $(LINK_TBB)
dim3-gcc-release-tbb-omp-triangle: EXECUTABLE=dem-3d-release-tbb-omp-triangle
dim3-gcc-release-tbb-omp-triangle: header build

dim3-gcc-release-omp-triangle-ampl: CC=g++ --std=c++11
dim3-gcc-release-omp-triangle-ampl: DIM=-DDim3
dim3-gcc-release-omp-triangle-ampl: SETTINGS=-DiREAL=double -DTrackGridStatistics -DompTriangle -DbyteAlignment=32
dim3-gcc-release-omp-triangle-ampl: CFLAGS=-O3 -g -qopenmp -ftree-vectorizer-verbose=7 -fdump-tree-vect-details -fstrict-aliasing $(INCLUDE_TBB)
dim3-gcc-release-omp-triangle-ampl: LFLAGS=-lrt $(LINK_OMP) $(LINK_TBB)
dim3-gcc-release-omp-triangle-ampl: EXECUTABLE=dem-3d-release-omp-triangle-ampl
dim3-gcc-release-omp-triangle-ampl: header build

dim3-gcc-release-tbb-omp-triangle-phi: CC=g++ --std=c++11
dim3-gcc-release-tbb-omp-triangle-phi: DIM=-DDim3
dim3-gcc-release-tbb-omp-triangle-phi: SETTINGS=-DiREAL=double -DTrackGridStatistics -DSharedTBB -DompTriangle -DbyteAlignment=64
dim3-gcc-release-tbb-omp-triangle-phi: CFLAGS=-mmic -qopenmp -ftree-vectorizer-verbose=7 -fdump-tree-vect-details -O3 -fstrict-aliasing $(INCLUDE_TBB)
dim3-gcc-release-tbb-omp-triangle-phi: LFLAGS=-lrt $(LINK_OMP) $(LINK_TBB) -mmic
dim3-gcc-release-tbb-omp-triangle-phi: EXECUTABLE=dem-3d-release-tbb-omp-triangle-phi
dim3-gcc-release-tbb-omp-triangle-phi: header build

dim3-icc-asserts: CC=icpc --std=c++11
dim3-icc-asserts: DIM=-DDim3
dim3-icc-asserts: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DAsserts -DTrackGridStatistics
dim3-icc-asserts: CFLAGS=-O0 -g -debug -Wall -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost
dim3-icc-asserts: LFLAGS=
dim3-icc-asserts: EXECUTABLE=dem-3d-asserts
dim3-icc-asserts: header build

dim3-icc-release-novec: CC=icpc --std=c++11
dim3-icc-release-novec: DIM=-DDim3
dim3-icc-release-novec: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DTrackGridStatistics
dim3-icc-release-novec: CFLAGS=-O0 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost -no-vec
dim3-icc-release-novec: LFLAGS=-lrt
dim3-icc-release-novec: EXECUTABLE=dem-3d-release-novec
dim3-icc-release-novec: header build

dim3-icc-release-vec: CC=icpc --std=c++11
dim3-icc-release-vec: DIM=-DDim3
dim3-icc-release-vec: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DTrackGridStatistics
dim3-icc-release-vec: CFLAGS=-fast -g -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost
dim3-icc-release-vec: LFLAGS=#-lrt
dim3-icc-release-vec: EXECUTABLE=dem-3d-release-vec
dim3-icc-release-vec: header build

dim3-icc-release-vec-stats: CC=icpc --std=c++11
dim3-icc-release-vec-stats: DIM=-DDim3
dim3-icc-release-vec-stats: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DTrackGridStatistics -DPARTICLESTATS #-DCONTACTSTATS -DFORCESTATS
dim3-icc-release-vec-stats: CFLAGS=-fast -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost
dim3-icc-release-vec-stats: LFLAGS=#-lrt
dim3-icc-release-vec-stats: EXECUTABLE=dem-3d-release-vec
dim3-icc-release-vec-stats: header build

dim3-icc-release-omp-triangle: CC=icpc --std=c++11
dim3-icc-release-omp-triangle: DIM=-DDim3
dim3-icc-release-omp-triangle: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DompTriangle -DTrackGridStatistics
dim3-icc-release-omp-triangle: CFLAGS=-fast -qopenmp -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost
dim3-icc-release-omp-triangle: LFLAGS=$(LINK_OMP) -lrt
dim3-icc-release-omp-triangle: EXECUTABLE=dem-3d-release-omp-triangle
dim3-icc-release-omp-triangle: header build

dim3-icc-release-omp-triangle-phi: CC=icpc --std=c++11
dim3-icc-release-omp-triangle-phi: DIM=-DDim3
dim3-icc-release-omp-triangle-phi: SETTINGS=-DiREAL=double -DbyteAlignment=64 -DompTriangle -DTrackGridStatistics
dim3-icc-release-omp-triangle-phi: CFLAGS=-mmic -fast -qopenmp -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip
dim3-icc-release-omp-triangle-phi: LFLAGS=$(LINK_OMP) -lrt -mmic
dim3-icc-release-omp-triangle-phi: EXECUTABLE=dem-3d-release-omp-triangle-phi
dim3-icc-release-omp-triangle-phi: header build

dim3-icc-release-omp-particle: CC=icpc --std=c++11
dim3-icc-release-omp-particle: DIM=-DDim3
dim3-icc-release-omp-particle: SETTINGS=-DiREAL=double -DbyteAlignment=32 -DompParticle -DTrackGridStatistics
dim3-icc-release-omp-particle: CFLAGS=-fast -qopenmp -qopt-threads-per-core=1 -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost
dim3-icc-release-omp-particle: LFLAGS=$(LINK_OMP) -lrt
dim3-icc-release-omp-particle: EXECUTABLE=dem-3d-release-omp-particle
dim3-icc-release-omp-particle: header build

dim3-icc-release-tbb: CC=icpc --std=c++11
dim3-icc-release-tbb: DIM=-DDim3
dim3-icc-release-tbb: SETTINGS=-DiREAL=double -DSharedTBB -DbyteAlignment=32 -DTrackGridStatistics
dim3-icc-release-tbb: CFLAGS=-fast -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost $(INCLUDE_TBB)
dim3-icc-release-tbb: LFLAGS=-lrt $(LINK_TBB)
dim3-icc-release-tbb: EXECUTABLE=dem-3d-release-tbb
dim3-icc-release-tbb: header build

dim3-icc-release-tbb-omp-triangle: CC=icpc --std=c++11
dim3-icc-release-tbb-omp-triangle: DIM=-DDim3
dim3-icc-release-tbb-omp-triangle: SETTINGS=-DiREAL=double -DSharedTBB -DompTriangle -DTrackGridStatistics -DbyteAlignment=32
dim3-icc-release-tbb-omp-triangle: CFLAGS=-fast -qopenmp -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost $(INCLUDE_TBB)
dim3-icc-release-tbb-omp-triangle: LFLAGS=-lrt $(LINK_OMP) $(LINK_TBB)
dim3-icc-release-tbb-omp-triangle: EXECUTABLE=dem-3d-release-tbb-omp-triangle
dim3-icc-release-tbb-omp-triangle: header build

dim3-icc-release-tbb-omp-particle: CC=icpc --std=c++11
dim3-icc-release-tbb-omp-particle: DIM=-DDim3
dim3-icc-release-tbb-omp-particle: SETTINGS=-DiREAL=double -DSharedTBB -DompParticle -DTrackGridStatistics -DbyteAlignment=32
dim3-icc-release-tbb-omp-particle: CFLAGS=-fast -qopenmp -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost $(INCLUDE_TBB)
dim3-icc-release-tbb-omp-particle: LFLAGS=-lrt $(LINK_OMP) $(LINK_TBB)
dim3-icc-release-tbb-omp-particle: EXECUTABLE=dem-3d-release-tbb-omp-particle
dim3-icc-release-tbb-omp-particle: header build

dim3-icc-release-tbb-omp-triangle-phi: CC=icpc --std=c++11
dim3-icc-release-tbb-omp-triangle-phi: DIM=-DDim3
dim3-icc-release-tbb-omp-triangle-phi: SETTINGS=-DiREAL=double -DTrackGridStatistics -DSharedTBB -DompTriangle -DbyteAlignment=64
dim3-icc-release-tbb-omp-triangle-phi: CFLAGS=-mmic -qopenmp -fast -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip $(INCLUDE_TBB)
dim3-icc-release-tbb-omp-triangle-phi: LFLAGS=-lrt $(LINK_OMP) $(LINK_TBB) -mmic
dim3-icc-release-tbb-omp-triangle-phi: EXECUTABLE=dem-3d-release-tbb-omp-triangle-phi
dim3-icc-release-tbb-omp-triangle-phi: header build

dim3-icc-release-omp-triangle-ampl: CC=icpc --std=c++11
dim3-icc-release-omp-triangle-ampl: DIM=-DDim3
dim3-icc-release-omp-triangle-ampl: SETTINGS=-DiREAL=double -DTrackGridStatistics -DompTriangle -DbyteAlignment=32
dim3-icc-release-omp-triangle-ampl: CFLAGS=-fast -g -qopenmp -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost $(INCLUDE_TBB)
dim3-icc-release-omp-triangle-ampl: LFLAGS=-lrt $(LINK_OMP) $(LINK_TBB)
dim3-icc-release-omp-triangle-ampl: EXECUTABLE=dem-3d-release-omp-triangle-ampl
dim3-icc-release-omp-triangle-ampl: header build

OBJECTS=$(SOURCES:.cpp=.o)

files.mk:
	touch files.mk
	echo SOURCES=\\ > files.mk
	find -L $(PROJECT_HOME) -name '*.cpp' | tr '\n' ' ' >> files.mk

header:
	@echo  --- This is Delta @ Peano 3 ---

build: files.mk $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS)
	@echo
	@echo build of Peano with DEM code based upon Delta successful
	@echo visit Peano\'s homepage at http://www.peano-framework.org

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
	rm -f files.mk
	rm -f compiler-minutes.txt

$(OBJECTS): %.o : %.cpp
	$(CC) $(DIM) $(SETTINGS) $(CFLAGS) -I$(PROJECT_HOME) -c $< -o $@
