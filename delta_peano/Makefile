# Include files
-include files.mk

PROJECT_HOME = .

# Configure System
# ----------------
INCLUDE_TBB=$(TBB_INC)
INCLUDE_OMP=
INCLUDE_MPI=$(MPI_INC)

LINK_TBB=$(TBB_SHLIB)
LINK_OMP=$(OMP_SHLIB)
LINK_OMP=
LINK_MPI=

##############################################################################################

GCC_SHAREDMEMORY=-fopenmp -qopt-threads-per-core=1 
ICC_SHAREDMEMORY=-qopenmp -qopt-threads-per-core=1

##############################################################################################

DELTA_STATS=-DPARTICLESTATS -DCONTACTSTATS
DELTA_TUNE=-DDELTATUNE

DELTAPHI_SETTINGS=-DiREAL=double -DbyteAlignment=64

DELTA_SETTINGS=-DiREAL=double -DbyteAlignment=32
PEANO_SETTINGS=-DTrackGridStatistics

##############################################################################################

GCCRUN=-O3 -Wall -ftree-vectorize -ftree-vectorizer-verbose=3 -ftree-loop-optimize -faggressive-loop-optimizations -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing -march=native
GCCDEBUG=-O3 -Wall -Werror -Wconversions -Wno-long-long -Wno-conversion -g -DDebug -DAsserts -pedantic -pedantic-errors -march=native 

##############################################################################################

ICCRUN=-fast -Wall -qopt-report=5 -qopt-prefetch=3 -falign-loops=32 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -opt-streaming-stores always -xHOST
ICCDEBUG=-O3 -Wall -Werror -Wconversions -g -DDebug -DAsserts -pedantic -pedantic-errors 

##############################################################################################

#COMPILER_CFLAGS=-std=c++0x -fast -fstrict-aliasing  -DMPICH_IGNORE_CXX_SEEK -fno-rtti -no-ipo -ip -xhost -wd603,3128

default: header
	@echo Please specify build target. Available targets are:
	@echo clean
	@echo dim3-gcc-asserts
	@echo dim3-gcc-release-novec
	@echo dim3-gcc-release-vec
	@echo dim3-gcc-release-omp-triangle
	@echo dim3-gcc-release-omp-triangle-phi
	@echo dim3-gcc-release-omp-triangle-ampl
	@echo dim3-gcc-release-omp-particle
	@echo dim3-gcc-release-tbb
	@echo dim3-gcc-release-mpi-tbb
	@echo dim3-gcc-release-tbb-omp-triangle
	@echo dim3-icc-asserts
	@echo dim3-icc-release-novec
	@echo dim3-icc-release-vec
	@echo dim3-icc-release-vec-stats
	@echo dim3-icc-release-omp-triangle
	@echo dim3-icc-release-omp-triangle-ampl
	@echo dim3-icc-release-omp-particle
	@echo dim3-icc-release-tbb
	@echo dim3-icc-release-mpi-tbb
	@echo dim3-icc-release-tbb-omp-triangle
	@echo dim3-icc-release-tbb-omp-particle

dim3-gcc-asserts: CC=g++ -std=c++0x
dim3-gcc-asserts: DIM=-DDim3
dim3-gcc-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) 
dim3-gcc-asserts: CFLAGS=$(GCCDEBUG)
dim3-gcc-asserts: LFLAGS=
dim3-gcc-asserts: EXECUTABLE=dem-3d-asserts
dim3-gcc-asserts: header build

dim3-gcc-release-novec: CC=g++ -std=c++0x
dim3-gcc-release-novec: DIM=-DDim3
dim3-gcc-release-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
dim3-gcc-release-novec: CFLAGS=$(GCCRUN) -mnovect-double
dim3-gcc-release-novec: LFLAGS=
dim3-gcc-release-novec: EXECUTABLE=dem-3d-release-novec
dim3-gcc-release-novec: header build

dim3-gcc-release-vec: CC=g++ --std=c++0x
dim3-gcc-release-vec: DIM=-DDim3
dim3-gcc-release-vec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
dim3-gcc-release-vec: CFLAGS=$(GCCRUN) 
dim3-gcc-release-vec: LFLAGS=
dim3-gcc-release-vec: EXECUTABLE=dem-3d-release-vec
dim3-gcc-release-vec: header build

dim3-gcc-release-omp-triangle: CC=g++ -std=c++0x
dim3-gcc-release-omp-triangle: DIM=-DDim3
dim3-gcc-release-omp-triangle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DompTriangle
dim3-gcc-release-omp-triangle: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY)
dim3-gcc-release-omp-triangle: LFLAGS=$(LINK_OMP)
dim3-gcc-release-omp-triangle: EXECUTABLE=dem-3d-release-omp-triangle
dim3-gcc-release-omp-triangle: header build

dim3-gcc-release-omp-triangle-phi: CC=g++ -std=c++0x
dim3-gcc-release-omp-triangle-phi: DIM=-DDim3
dim3-gcc-release-omp-triangle-phi: SETTINGS=$(DELTAPHI_SETTINGS) $(PEANO_SETTINGS) -DompTriangle
dim3-gcc-release-omp-triangle-phi: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY)
dim3-gcc-release-omp-triangle-phi: LFLAGS=$(LINK_OMP) -mmic
dim3-gcc-release-omp-triangle-phi: EXECUTABLE=dem-3d-release-omp-triangle-phi
dim3-gcc-release-omp-triangle-phi: header build

dim3-gcc-release-omp-triangle-ampl: CC=g++ -std=c++0x
dim3-gcc-release-omp-triangle-ampl: DIM=-DDim3
dim3-gcc-release-omp-triangle-ampl: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DompTriangle
dim3-gcc-release-omp-triangle-ampl: CFLAGS=-g $(GCCRUN) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
dim3-gcc-release-omp-triangle-ampl: LFLAGS= $(LINK_OMP) 
dim3-gcc-release-omp-triangle-ampl: EXECUTABLE=dem-3d-release-omp-triangle-ampl
dim3-gcc-release-omp-triangle-ampl: header build

dim3-gcc-release-omp-particle: CC=g++ -std=c++0x
dim3-gcc-release-omp-particle: DIM=-DDim3
dim3-gcc-release-omp-particle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DompParticle
dim3-gcc-release-omp-particle: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY)
dim3-gcc-release-omp-particle: LFLAGS=$(LINK_OMP)
dim3-gcc-release-omp-particle: EXECUTABLE=dem-3d-release-omp-particle
dim3-gcc-release-omp-particle: header build

dim3-gcc-release-tbb: CC=g++ -std=c++0x
dim3-gcc-release-tbb: DIM=-DDim3
dim3-gcc-release-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
dim3-gcc-release-tbb: CFLAGS=$(GCCRUN) $(INCLUDE_TBB)
dim3-gcc-release-tbb: LFLAGS= $(LINK_TBB) 
dim3-gcc-release-tbb: EXECUTABLE=dem-3d-release-tbb
dim3-gcc-release-tbb: header build

dim3-gcc-release-mpi-tbb: CC=mpic++ -std=c++0x
dim3-gcc-release-mpi-tbb: DIM=-DDim3
dim3-gcc-release-mpi-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
dim3-gcc-release-mpi-tbb: CFLAGS=$(GCCRUN) $(INCLUDE_TBB)
dim3-gcc-release-mpi-tbb: LFLAGS=$(MPI_INC) $(LINK_TBB) 
dim3-gcc-release-mpi-tbb: EXECUTABLE=dem-3d-release-mpi-tbb
dim3-gcc-release-mpi-tbb: header build

dim3-gcc-release-tbb-omp-triangle: CC=g++ -std=c++0x
dim3-gcc-release-tbb-omp-triangle: DIM=-DDim3
dim3-gcc-release-tbb-omp-triangle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
dim3-gcc-release-tbb-omp-triangle: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
dim3-gcc-release-tbb-omp-triangle: LFLAGS=$(LINK_TBB) $(LINK_OMP)
dim3-gcc-release-tbb-omp-triangle: EXECUTABLE=dem-3d-release-tbb-omp-triangle
dim3-gcc-release-tbb-omp-triangle: header build

dim3-gcc-release-tbb-omp-triangle-phi: CC=g++ -std=c++0x
dim3-gcc-release-tbb-omp-triangle-phi: DIM=-DDim3
dim3-gcc-release-tbb-omp-triangle-phi: SETTINGS=$(DELTAPHI_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
dim3-gcc-release-tbb-omp-triangle-phi: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
dim3-gcc-release-tbb-omp-triangle-phi: LFLAGS=$(LINK_OMP) $(LINK_TBB) -mmic 
dim3-gcc-release-tbb-omp-triangle-phi: EXECUTABLE=dem-3d-release-tbb-omp-triangle-phi
dim3-gcc-release-tbb-omp-triangle-phi: header build

dim3-icc-asserts: CC=icpc --std=c++11
dim3-icc-asserts: DIM=-DDim3
dim3-icc-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
dim3-icc-asserts: CFLAGS=$(ICCDEBUG) 
dim3-icc-asserts: LFLAGS=
dim3-icc-asserts: EXECUTABLE=dem-3d-asserts
dim3-icc-asserts: header build

dim3-icc-release-novec: CC=icpc --std=c++11
dim3-icc-release-novec: DIM=-DDim3
dim3-icc-release-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
dim3-icc-release-novec: CFLAGS=$(ICCRUN) -no-vec
dim3-icc-release-novec: LFLAGS=
dim3-icc-release-novec: EXECUTABLE=dem-3d-release-novec
dim3-icc-release-novec: header build

dim3-icc-release-vec: CC=icpc --std=c++11
dim3-icc-release-vec: DIM=-DDim3
dim3-icc-release-vec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
dim3-icc-release-vec: CFLAGS=$(ICCRUN)
dim3-icc-release-vec: LFLAGS=
dim3-icc-release-vec: EXECUTABLE=dem-3d-release-vec
dim3-icc-release-vec: header build

dim3-icc-release-vec-stats: CC=icpc --std=c++11
dim3-icc-release-vec-stats: DIM=-DDim3
dim3-icc-release-vec-stats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS)
dim3-icc-release-vec-stats: CFLAGS=$(ICCRUN)
dim3-icc-release-vec-stats: LFLAGS=
dim3-icc-release-vec-stats: EXECUTABLE=dem-3d-release-vec
dim3-icc-release-vec-stats: header build

dim3-icc-release-omp-triangle: CC=icpc --std=c++11
dim3-icc-release-omp-triangle: DIM=-DDim3
dim3-icc-release-omp-triangle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DompTriangle
dim3-icc-release-omp-triangle: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY) 
dim3-icc-release-omp-triangle: LFLAGS=$(LINK_OMP)
dim3-icc-release-omp-triangle: EXECUTABLE=dem-3d-release-omp-triangle
dim3-icc-release-omp-triangle: header build

dim3-icc-release-omp-triangle-stats: CC=icpc --std=c++11
dim3-icc-release-omp-triangle-stats: DIM=-DDim3
dim3-icc-release-omp-triangle-stats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DompTriangle
dim3-icc-release-omp-triangle-stats: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY) 
dim3-icc-release-omp-triangle-stats: LFLAGS=$(LINK_OMP)
dim3-icc-release-omp-triangle-stats: EXECUTABLE=dem-3d-release-omp-triangle
dim3-icc-release-omp-triangle-stats: header build

dim3-icc-release-omp-triangle-ampl: CC=icpc --std=c++11
dim3-icc-release-omp-triangle-ampl: DIM=-DDim3
dim3-icc-release-omp-triangle-ampl: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DompTriangle
dim3-icc-release-omp-triangle-ampl: CFLAGS=-g $(ICCRUN) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
dim3-icc-release-omp-triangle-ampl: LFLAGS=$(LINK_OMP)
dim3-icc-release-omp-triangle-ampl: EXECUTABLE=dem-3d-release-omp-triangle-ampl
dim3-icc-release-omp-triangle-ampl: header build

dim3-icc-release-omp-particle: CC=icpc --std=c++11
dim3-icc-release-omp-particle: DIM=-DDim3
dim3-icc-release-omp-particle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DompParticle
dim3-icc-release-omp-particle: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY)
dim3-icc-release-omp-particle: LFLAGS=$(LINK_OMP)
dim3-icc-release-omp-particle: EXECUTABLE=dem-3d-release-omp-particle
dim3-icc-release-omp-particle: header build

dim3-icc-release-tbb: CC=icpc --std=c++11
dim3-icc-release-tbb: DIM=-DDim3
dim3-icc-release-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
dim3-icc-release-tbb: CFLAGS=$(ICCRUN) $(INCLUDE_TBB)
dim3-icc-release-tbb: LFLAGS=$(LINK_TBB)
dim3-icc-release-tbb: EXECUTABLE=dem-3d-release-tbb
dim3-icc-release-tbb: header build

dim3-icc-release-tbb-asserts: CC=icpc --std=c++11
dim3-icc-release-tbb-asserts: DIM=-DDim3
dim3-icc-release-tbb-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
dim3-icc-release-tbb-asserts: CFLAGS=$(ICCDEBUG) $(INCLUDE_TBB)
dim3-icc-release-tbb-asserts: LFLAGS=$(LINK_TBB)
dim3-icc-release-tbb-asserts: EXECUTABLE=dem-3d-release-tbb
dim3-icc-release-tbb-asserts: header build

dim3-icc-release-tbb-stats: CC=icpc --std=c++11
dim3-icc-release-tbb-stats: DIM=-DDim3
dim3-icc-release-tbb-stats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DSharedTBB
dim3-icc-release-tbb-stats: CFLAGS=$(ICCRUN) $(INCLUDE_TBB)
dim3-icc-release-tbb-stats: LFLAGS=$(LINK_TBB)
dim3-icc-release-tbb-stats: EXECUTABLE=dem-3d-release-tbb
dim3-icc-release-tbb-stats: header build

dim3-icc-release-mpi-tbb: CC=mpiicc --std=c++11
dim3-icc-release-mpi-tbb: DIM=-DDim3
dim3-icc-release-mpi-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
dim3-icc-release-mpi-tbb: CFLAGS=$(ICCRUN) $(INCLUDE_TBB)
dim3-icc-release-mpi-tbb: LFLAGS=$(LINK_TBB)
dim3-icc-release-mpi-tbb: EXECUTABLE=dem-3d-release-mpi-tbb
dim3-icc-release-mpi-tbb: header build

dim3-icc-release-tbb-omp-triangle: CC=icpc --std=c++11
dim3-icc-release-tbb-omp-triangle: DIM=-DDim3
dim3-icc-release-tbb-omp-triangle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
dim3-icc-release-tbb-omp-triangle: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
dim3-icc-release-tbb-omp-triangle: LFLAGS=$(LINK_TBB) $(LINK_OMP)
dim3-icc-release-tbb-omp-triangle: EXECUTABLE=dem-3d-release-tbb-omp-triangle
dim3-icc-release-tbb-omp-triangle: header build

dim3-icc-release-tbb-omp-particle: CC=icpc --std=c++11
dim3-icc-release-tbb-omp-particle: DIM=-DDim3
dim3-icc-release-tbb-omp-particle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)-DSharedTBB -DompParticle
dim3-icc-release-tbb-omp-particle: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
dim3-icc-release-tbb-omp-particle: LFLAGS=$(LINK_TBB) $(LINK_OMP)
dim3-icc-release-tbb-omp-particle: EXECUTABLE=dem-3d-release-tbb-omp-particle
dim3-icc-release-tbb-omp-particle: header build

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
