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
LINK_OMP=-fopenmp
LINK_MPI=-L /usr/local/Cluster-Apps/openmpi/gcc-4.9.1/1.8.2/lib/ -lmpi

##############################################################################################

GCC_SHAREDMEMORY=-fopenmp
ICC_SHAREDMEMORY=-qopenmp -qopt-threads-per-core=1

##############################################################################################

#DELTA_STATS=-DPARTICLESTATSFULL -DCONTACTSTATS
DELTA_STATS=-DPARTICLESTATSMIN
DELTA_TUNE=-DDELTATUNE

DELTAPHI_SETTINGS=-DiREAL=double -DbyteAlignment=64

DELTA_SETTINGS=-DiREAL=double -DbyteAlignment=32
PEANO_SETTINGS=-DTrackGridStatistics

##############################################################################################

GCCRUN=-O3 -Wall -ftree-vectorize -ftree-vectorizer-verbose=3 -ftree-loop-optimize -faggressive-loop-optimizations -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing
GCCDEBUG=-O3 -Wall -g -DDebug -DAsserts -pedantic -pedantic-errors
GCCNOVEC=-O3 -Wall -fno-tree-vectorize -ftree-vectorizer-verbose=3 -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing
##############################################################################################

ICCRUN=-fast -Wall -qopt-report=5 -qopt-prefetch=3 -falign-loops=32 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -qopt-streaming-stores always -xHOST
ICCDEBUG=-O3 -Wall -g -DAsserts -pedantic -pedantic-errors
ICCNOVEC=-O3 -Wall -no-vec -qopt-report=5 -qopt-prefetch=3 -falign-loops=32 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -qopt-streaming-stores always -xHOST
##############################################################################################

#COMPILER_CFLAGS=-std=c++14 -fast -fstrict-aliasing  -DMPICH_IGNORE_CXX_SEEK -fno-rtti -no-ipo -ip -xhost -wd603,3128

default: header
	@echo Please specify build target. Available targets are:
	@echo clean
	@echo delta-gcc-asserts
	@echo delta-gcc-release-novec
	@echo delta-gcc-release-tbb-omp-triangle
	@echo delta-gcc-release-tbb-omp-triangle-stats
	@echo delta-gcc-release-mpi-tbb
	@echo delta-icc-asserts
	@echo delta-icc-release-novec
	@echo delta-icc-release-tbb-omp-triangle
	@echo delta-icc-release-tbb-omp-triangle-stats
	@echo delta-icc-release-mpi-tbb

################################################################################################################
######## GNU COMPILERS #########################################################################################
################################################################################################################

delta-gcc-asserts: CC=g++ -std=c++14
delta-gcc-asserts: DIM=-DDim3
delta-gcc-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
delta-gcc-asserts: CFLAGS=$(GCCDEBUG)
delta-gcc-asserts: LFLAGS=
delta-gcc-asserts: EXECUTABLE=delta-asserts
delta-gcc-asserts: header build

delta-gcc-release-novec: CC=g++ -std=c++14
delta-gcc-release-novec: DIM=-DDim3
delta-gcc-release-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
delta-gcc-release-novec: CFLAGS=$(GCCNOVEC)
delta-gcc-release-novec: LFLAGS=
delta-gcc-release-novec: EXECUTABLE=delta-release-novec
delta-gcc-release-novec: header build

delta-gcc-release-tbb-omp-triangle: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle: CFLAGS=$(GCCNOVEC) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-omp-triangle: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-omp-triangle: EXECUTABLE=delta-release-tbb-omp-triangle
delta-gcc-release-tbb-omp-triangle: header build

delta-gcc-release-tbb-omp-triangle-novec: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle-novec: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle-novec: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-omp-triangle-novec: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-omp-triangle-novec: EXECUTABLE=delta-release-tbb-omp-triangle-novec
delta-gcc-release-tbb-omp-triangle-novec: header build

delta-gcc-release-tbb-omp-triangle-stats: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle-stats: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle-stats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle-stats: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY)
delta-gcc-release-tbb-omp-triangle-stats: LFLAGS=$(LINK_OMP)
delta-gcc-release-tbb-omp-triangle-stats: EXECUTABLE=delta-release-tbb-omp-triangle
delta-gcc-release-tbb-omp-triangle-stats: header build

delta-gcc-release-tbb-omp-triangle-phi: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle-phi: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle-phi: SETTINGS=$(DELTAPHI_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle-phi: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-omp-triangle-phi: LFLAGS=$(LINK_OMP) $(LINK_TBB) -mmic
delta-gcc-release-tbb-omp-triangle-phi: EXECUTABLE=delta-release-tbb-omp-triangle-phi
delta-gcc-release-tbb-omp-triangle-phi: header build

delta-gcc-release-mpi-tbb: CC=mpic++ -std=c++14
delta-gcc-release-mpi-tbb: DIM=-DDim3
delta-gcc-release-mpi-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DParallel
delta-gcc-release-mpi-tbb: CFLAGS=$(GCCRUN) $(INCLUDE_TBB) $(INCLUDE_MPI)
delta-gcc-release-mpi-tbb: LFLAGS=$(LINK_TBB) $(LINK_MPI)
delta-gcc-release-mpi-tbb: EXECUTABLE=delta-release-mpi-tbb
delta-gcc-release-mpi-tbb: header build

################################################################################################################
######## INTEL COMPILERS #######################################################################################
################################################################################################################

delta-icc-asserts: CC=icpc --std=c++11
delta-icc-asserts: DIM=-DDim3
delta-icc-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
delta-icc-asserts: CFLAGS=$(ICCDEBUG)
delta-icc-asserts: LFLAGS=
delta-icc-asserts: EXECUTABLE=delta-asserts
delta-icc-asserts: header build

delta-icc-release-novec: CC=icpc --std=c++11
delta-icc-release-novec: DIM=-DDim3
delta-icc-release-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
delta-icc-release-novec: CFLAGS=$(ICCNOVEC)
delta-icc-release-novec: LFLAGS=
delta-icc-release-novec: EXECUTABLE=delta-release-novec
delta-icc-release-novec: header build

delta-icc-release-tbb-omp-triangle: CC=icpc --std=c++11
delta-icc-release-tbb-omp-triangle: DIM=-DDim3
delta-icc-release-tbb-omp-triangle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-omp-triangle: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-omp-triangle: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-omp-triangle: EXECUTABLE=delta-release-tbb-omp-triangle
delta-icc-release-tbb-omp-triangle: header build

delta-icc-release-tbb-omp-triangle-novec: CC=icpc --std=c++11
delta-icc-release-tbb-omp-triangle-novec: DIM=-DDim3
delta-icc-release-tbb-omp-triangle-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-omp-triangle-novec: CFLAGS=$(ICCNOVEC) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-omp-triangle-novec: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-omp-triangle-novec: EXECUTABLE=delta-release-tbb-omp-triangle-novec
delta-icc-release-tbb-omp-triangle-novec: header build

delta-icc-release-tbb-omp-triangle-stats: CC=icpc --std=c++11
delta-icc-release-tbb-omp-triangle-stats: DIM=-DDim3
delta-icc-release-tbb-omp-triangle-stats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-omp-triangle-stats: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY)
delta-icc-release-tbb-omp-triangle-stats: LFLAGS=$(LINK_OMP)
delta-icc-release-tbb-omp-triangle-stats: EXECUTABLE=delta-release-tbb-omp-triangle
delta-icc-release-tbb-omp-triangle-stats: header build

delta-icc-release-mpi-tbb: CC=mpiicc --std=c++11
delta-icc-release-mpi-tbb: DIM=-DDim3
delta-icc-release-mpi-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DParallel
delta-icc-release-mpi-tbb: CFLAGS=$(ICCRUN) $(INCLUDE_MPI) $(INCLUDE_TBB)
delta-icc-release-mpi-tbb: LFLAGS=$(LINK_MPI) $(LINK_TBB)
delta-icc-release-mpi-tbb: EXECUTABLE=delta-release-mpi-tbb
delta-icc-release-mpi-tbb: header build

################################################################################################################
################################################################################################################
################################################################################################################

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
	find -L . -name "*.optrpt" | xargs rm
	find -L . -name "*.vect" | xargs rm

$(OBJECTS): %.o : %.cpp
	$(CC) $(DIM) $(SETTINGS) $(CFLAGS) -I$(PROJECT_HOME) -c $< -o $@
