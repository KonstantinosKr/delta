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
GCCRUNCNT=-O3 -Wall -g -ftree-vectorize -ftree-vectorizer-verbose=3 -ftree-loop-optimize -faggressive-loop-optimizations -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing
GCCDEBUG=-O3 -Wall -g -DDebug -DAsserts -pedantic -pedantic-errors
GCCNOVEC=-O3 -Wall -fno-tree-vectorize -ftree-vectorizer-verbose=3 -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing
GCCNOVECCNT=-O3 -Wall -g -fno-tree-vectorize -ftree-vectorizer-verbose=3 -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing
##############################################################################################

ICCRUN=-fast -Wall -simd-function-pointers -qopt-report=5 -qopt-prefetch=3 -falign-loops=32 -funroll-loops -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHOST
ICCRUNCNT=-fast -g -Wall -simd-function-pointers -qopt-report=5 -qopt-prefetch=3 -falign-loops=32 -funroll-loops -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHOST
ICCDEBUG=-O3 -Wall -g -DAsserts -pedantic -pedantic-errors
ICCNOVEC=-O3 -Wall -no-vec -qno-openmp-simd -no-ipo -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -xHOST
ICCNOVECCNT=-O3 -Wall -g -no-vec -qno-openmp-simd -no-ipo -qopt-report=5 -fstrict-aliasing -restrict -fno-rtti -xHOST
##############################################################################################

#COMPILER_CFLAGS=-std=c++14 -fast -fstrict-aliasing  -DMPICH_IGNORE_CXX_SEEK -fno-rtti -no-ipo -ip -xhost -wd603,3128

default: header
	@echo Please specify build target. Available targets are:
	@echo clean
	@echo --GCC-------------------------------
	@echo delta-gcc-asserts
	@echo delta-gcc-pstats
	@echo delta-gcc-dstats
	@echo delta-gcc-asserts-tbb-omp-triangle
	@echo delta-gcc-release-tbb-omp-triangle-novec
	@echo delta-gcc-asserts-tbb-omp-triangle-counter
	@echo delta-gcc-release-tbb-omp-triangle-novec-counter
	@echo delta-gcc-release-mpi
	@echo --ICC-------------------------------
	@echo delta-icc-asserts
	@echo delta-icc-release-pstats
	@echo delta-icc-release-dstats
	@echo delta-icc-asserts-tbb-omp-triangle
	@echo delta-icc-release-tbb-omp-triangle-novec
	@echo delta-icc-asserts-tbb-omp-triangle-counter
	@echo delta-icc-release-tbb-omp-triangle-novec-counter
	@echo delta-icc-release-mpi

################################################################################################################
######## GNU COMPILERS #########################################################################################
################################################################################################################

delta-gcc-asserts: CC=g++ -std=c++14
delta-gcc-asserts: DIM=-DDim3
delta-gcc-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
delta-gcc-asserts: CFLAGS=$(GCCDEBUG)
delta-gcc-asserts: LFLAGS=
delta-gcc-asserts: EXECUTABLE=delta-gcc-asserts
delta-gcc-asserts: header build

delta-gcc-release-pstats: CC=g++ -std=c++14
delta-gcc-release-pstats: DIM=-DDim3
delta-gcc-release-pstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DSharedTBB -DompTriangle
delta-gcc-release-pstats: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY)
delta-gcc-release-pstats: LFLAGS=$(LINK_OMP)
delta-gcc-release-pstats: EXECUTABLE=delta-gcc-release-stats
delta-gcc-release-pstats: header build

delta-gcc-release-dstats: CC=g++ -std=c++14
delta-gcc-release-dstats: DIM=-DDim3
delta-gcc-release-dstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_TUNE) -DSharedTBB -DompTriangle
delta-gcc-release-dstats: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY)
delta-gcc-release-dstats: LFLAGS=$(LINK_OMP)
delta-gcc-release-dstats: EXECUTABLE=delta-gcc-release-dstats
delta-gcc-release-dstats: header build

delta-gcc-release-tbb-omp-triangle: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-omp-triangle: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-omp-triangle: EXECUTABLE=delta-gcc-release-tbb-omp-triangle
delta-gcc-release-tbb-omp-triangle: header build

delta-gcc-release-tbb-omp-triangle-counter: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle-counter: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle-counter: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle-counter: CFLAGS=$(GCCRUNCNT $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-omp-triangle-counter: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-omp-triangle-counter: EXECUTABLE=delta-gcc-release-tbb-omp-triangle-counter
delta-gcc-release-tbb-omp-triangle-counter: header build

delta-gcc-release-tbb-omp-triangle-novec: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle-novec: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle-novec: CFLAGS=$(GCCNOVEC) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-omp-triangle-novec: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-omp-triangle-novec: EXECUTABLE=delta-gcc-release-tbb-omp-triangle-novec
delta-gcc-release-tbb-omp-triangle-novec: header build

delta-gcc-release-tbb-omp-triangle-novec-counter: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle-novec-counter: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle-novec-counter: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle-novec-counter: CFLAGS=$(GCCNOVECCNT) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-omp-triangle-novec-counter: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-omp-triangle-novec-counter: EXECUTABLE=delta-gcc-release-tbb-omp-triangle-novec-counter
delta-gcc-release-tbb-omp-triangle-novec-counter: header build

delta-gcc-release-tbb-omp-triangle-phi: CC=g++ -std=c++14
delta-gcc-release-tbb-omp-triangle-phi: DIM=-DDim3
delta-gcc-release-tbb-omp-triangle-phi: SETTINGS=$(DELTAPHI_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-omp-triangle-phi: CFLAGS=$(GCCRUN) $(GCC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-omp-triangle-phi: LFLAGS=$(LINK_OMP) $(LINK_TBB) -mmic
delta-gcc-release-tbb-omp-triangle-phi: EXECUTABLE=delta-gcc-release-tbb-omp-triangle-phi
delta-gcc-release-tbb-omp-triangle-phi: header build

delta-gcc-release-mpi: CC=mpic++ -std=c++14
delta-gcc-release-mpi: DIM=-DDim3
delta-gcc-release-mpi: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DParallel
delta-gcc-release-mpi: CFLAGS=$(GCCRUN) $(INCLUDE_TBB) $(INCLUDE_MPI)
delta-gcc-release-mpi: LFLAGS=$(LINK_TBB) $(LINK_MPI)
delta-gcc-release-mpi: EXECUTABLE=delta-gcc-release-mpi-tbb
delta-gcc-release-mpi: header build

################################################################################################################
######## INTEL COMPILERS #######################################################################################
################################################################################################################

delta-icc-asserts: CC=icpc -std=c++14
delta-icc-asserts: DIM=-DDim3
delta-icc-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
delta-icc-asserts: CFLAGS=$(ICCDEBUG)
delta-icc-asserts: LFLAGS=
delta-icc-asserts: EXECUTABLE=delta-icc-asserts
delta-icc-asserts: header build

delta-icc-release-tbb-omp-triangle: CC=icpc -std=c++14
delta-icc-release-tbb-omp-triangle: DIM=-DDim3
delta-icc-release-tbb-omp-triangle: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-omp-triangle: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-omp-triangle: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-omp-triangle: EXECUTABLE=delta-icc-release-tbb-omp-triangle
delta-icc-release-tbb-omp-triangle: header build

delta-icc-release-tbb-omp-triangle-counter: CC=icpc -std=c++14
delta-icc-release-tbb-omp-triangle-counter: DIM=-DDim3
delta-icc-release-tbb-omp-triangle-counter: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-omp-triangle-counter: CFLAGS=$(ICCCNT) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-omp-triangle-counter: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-omp-triangle-counter: EXECUTABLE=delta-icc-release-tbb-omp-triangle-counter
delta-icc-release-tbb-omp-triangle-counter: header build

delta-icc-release-tbb-omp-triangle-novec: CC=icpc -std=c++14
delta-icc-release-tbb-omp-triangle-novec: DIM=-DDim3
delta-icc-release-tbb-omp-triangle-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-omp-triangle-novec: CFLAGS=$(ICCNOVEC) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-omp-triangle-novec: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-omp-triangle-novec: EXECUTABLE=delta-icc-release-tbb-omp-triangle-novec
delta-icc-release-tbb-omp-triangle-novec: header build

delta-icc-release-tbb-omp-triangle-novec-counter: CC=icpc -std=c++14
delta-icc-release-tbb-omp-triangle-novec-counter: DIM=-DDim3
delta-icc-release-tbb-omp-triangle-novec-counter: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-omp-triangle-novec-counter: CFLAGS=$(ICCNOVECCNT) $(ICC_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-omp-triangle-novec-counter: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-omp-triangle-novec-counter: EXECUTABLE=delta-icc-release-tbb-omp-triangle-novec-counter
delta-icc-release-tbb-omp-triangle-novec-counter: header build

delta-icc-release-pstats: CC=icpc -std=c++14
delta-icc-release-pstats: DIM=-DDim3
delta-icc-release-pstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DSharedTBB -DompTriangle
delta-icc-release-pstats: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY)
delta-icc-release-pstats: LFLAGS=$(LINK_OMP)
delta-icc-release-pstats: EXECUTABLE=delta-icc-release-pstats
delta-icc-release-pstats: header build

delta-icc-release-dstats: CC=icpc -std=c++14
delta-icc-release-dstats: DIM=-DDim3
delta-icc-release-dstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_TUNE) -DSharedTBB -DompTriangle
delta-icc-release-dstats: CFLAGS=$(ICCRUN) $(ICC_SHAREDMEMORY)
delta-icc-release-dstats: LFLAGS=$(LINK_OMP)
delta-icc-release-dstats: EXECUTABLE=delta-icc-release-dstats
delta-icc-release-dstats: header build

delta-icc-release-mpi: CC=mpiicc -std=c++14
delta-icc-release-mpi: DIM=-DDim3
delta-icc-release-mpi: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DParallel
delta-icc-release-mpi: CFLAGS=$(ICCRUN) $(INCLUDE_MPI) $(INCLUDE_TBB)
delta-icc-release-mpi: LFLAGS=$(LINK_MPI) $(LINK_TBB)
delta-icc-release-mpi: EXECUTABLE=delta-icc-release-mpi-tbb
delta-icc-release-mpi: header build

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
