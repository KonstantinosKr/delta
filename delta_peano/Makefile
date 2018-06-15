# Include files
-include files.mk

PROJECT_HOME = .

# Configure System
# ----------------
INCLUDE_TBB=$(TBB_INC)
INCLUDE_OMP=$(OMP_INC)
INCLUDE_MPI=$(MPI_INC)
INCLUDE_LIBS=$(ASSIMP_INC)

LINK_TBB=$(TBB_SHLIB)
LINK_OMP=$(OMP_SHLIB)
LINK_OMP=-fopenmp
LINK_MPI=-L/usr/local/Cluster-Apps/openmpi/gcc-4.9.1/1.8.2/lib/ -lmpi
LINK_LIBS=$(ASSIMP_LIB)

##############################################################################################

GCC_OMP_SHAREDMEMORY=-fopenmp
ICC_OMP_SHAREDMEMORY=-qopenmp -qopt-threads-per-core=1

##############################################################################################

DELTA_STATS=-DPARTICLESTATSMIN #-DPARTICLESTATSFULL -DCONTACTSTATS

DELTAPHI_SETTINGS=-DiREAL=double -DbyteAlignment=64

DELTA_SETTINGS=-DiREAL=double -DbyteAlignment=32
PEANO_SETTINGS=-DTrackGridStatistics -DpeanoCall

##############################################################################################

GCCRUN=-O3 -Wall -ftree-vectorize -ftree-vectorizer-verbose=3 -ftree-loop-optimize -faggressive-loop-optimizations -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fstrict-aliasing
GCCDEBUG=-O3 -Wall -g -DDebug -DAsserts -pedantic -pedantic-errors
GCCNOVEC=-O3 -Wall -fno-tree-vectorize -ftree-vectorizer-verbose=3 -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing
##############################################################################################

ICCRUN=-O3 -Wall -simd-function-pointers -qopt-report=5 -qopt-prefetch=2 -falign-loops=32 -funroll-loops -fstrict-aliasing -restrict -pedantic -fno-rtti -ip -xHOST
ICCDEBUG=-O3 -Wall -g -DAsserts -pedantic -pedantic-errors -no-ipo
ICCNOVEC=-O3 -Wall -no-vec -qno-openmp-simd -no-ipo -qopt-report=5 -fstrict-aliasing -restrict -pedantic -fno-rtti -xHOST
##############################################################################################

default: header
	@echo Please specify build target. Available targets are:
	@echo clean
	@echo --GCC-------------------------------
	@echo delta-gcc-asserts
	@echo delta-gcc-serial
	@echo delta-gcc-omp
	@echo delta-gcc-omp-novec
	@echo delta-gcc-pstats
	@echo delta-gcc-release-tbb-novec
	@echo delta-gcc-release-mpi
	@echo --ICC-------------------------------
	@echo delta-icc-asserts
	@echo delta-icc-serial
	@echo delta-icc-omp
	@echo delta-icc-omp-novec
	@echo delta-gcc-pstats
	@echo delta-icc-release-pstats
	@echo delta-icc-release-tbb-novec
	@echo delta-icc-release-mpi

################################################################################################################
######## GNU COMPILERS #########################################################################################
################################################################################################################

delta-gcc-asserts: CC=g++ -std=c++14
delta-gcc-asserts: DIM=-DDim3
delta-gcc-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
delta-gcc-asserts: CFLAGS=$(GCCDEBUG) $(INCLUDE_LIBS)
delta-gcc-asserts: LFLAGS=$(LINK_LIBS)
delta-gcc-asserts: EXECUTABLE=delta-gcc-asserts
delta-gcc-asserts: header build

delta-gcc-serial: CC=g++ -std=c++14
delta-gcc-serial: DIM=-DDim3
delta-gcc-serial: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS)
delta-gcc-serial: CFLAGS=$(GCCRUN) $(INCLUDE_LIBS)
delta-gcc-serial: LFLAGS=$(LINK_LIBS)
delta-gcc-serial: EXECUTABLE=delta-gcc-serial
delta-gcc-serial: header build

delta-gcc-omp: CC=g++ -std=c++14
delta-gcc-omp: DIM=-DDim3
delta-gcc-omp: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DOMPTriangle
delta-gcc-omp: CFLAGS=$(GCCRUN) $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS)
delta-gcc-omp: LFLAGS=$(LINK_OMP) $(LINK_LIBS)
delta-gcc-omp: EXECUTABLE=delta-gcc-omp
delta-gcc-omp: header build

delta-gcc-release-tbb: CC=g++ -std=c++14
delta-gcc-release-tbb: DIM=-DDim3
delta-gcc-release-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
delta-gcc-release-tbb: CFLAGS=$(GCCRUN) $(INCLUDE_TBB) $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS)
delta-gcc-release-tbb: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_LIBS)
delta-gcc-release-tbb: EXECUTABLE=delta-gcc-release-tbb
delta-gcc-release-tbb: header build

delta-gcc-release-tbb-novec: CC=g++ -std=c++14
delta-gcc-release-tbb-novec: DIM=-DDim3
delta-gcc-release-tbb-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
delta-gcc-release-tbb-novec: CFLAGS=$(GCCNOVEC) $(INCLUDE_TBB) $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS)
delta-gcc-release-tbb-novec: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_LIBS)
delta-gcc-release-tbb-novec: EXECUTABLE=delta-gcc-release-tbb-novec
delta-gcc-release-tbb-novec: header build

delta-gcc-release-pstats: CC=g++ -std=c++14
delta-gcc-release-pstats: DIM=-DDim3
delta-gcc-release-pstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DSharedTBB
delta-gcc-release-pstats: CFLAGS=$(GCCRUN) $(INCLUDE_LIBS)
delta-gcc-release-pstats: LFLAGS=$(LINK_LIBS)
delta-gcc-release-pstats: EXECUTABLE=delta-gcc-release-tbb-stats
delta-gcc-release-pstats: header build

delta-gcc-release-tbb-phi: CC=g++ -std=c++14
delta-gcc-release-tbb-phi: DIM=-DDim3
delta-gcc-release-tbb-phi: SETTINGS=$(DELTAPHI_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
delta-gcc-release-tbb-phi: CFLAGS=$(GCCRUN) $(INCLUDE_TBB) $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS)
delta-gcc-release-tbb-phi: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_LIBS) -mmic
delta-gcc-release-tbb-phi: EXECUTABLE=delta-gcc-release-tbb-phi
delta-gcc-release-tbb-phi: header build

delta-gcc-release-mpi: CC=mpic++ -std=c++14
delta-gcc-release-mpi: DIM=-DDim3
delta-gcc-release-mpi: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DParallel -DMPICH_IGNORE_CXX_SEEK -DMultipleThreadsMayTriggerMPICalls
delta-gcc-release-mpi: CFLAGS=$(GCCRUN) $(INCLUDE_TBB) $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_MPI) $(INCLUDE_LIBS)
delta-gcc-release-mpi: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_MPI) $(LINK_LIBS)
delta-gcc-release-mpi: EXECUTABLE=delta-gcc-release-mpi-tbb
delta-gcc-release-mpi: header build

################################################################################################################
######## INTEL COMPILERS #######################################################################################
################################################################################################################

delta-icc-asserts: CC=icpc -std=c++14
delta-icc-asserts: DIM=-DDim3
delta-icc-asserts: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(INCLUDE_LIBS)
delta-icc-asserts: CFLAGS=$(ICCDEBUG) -g
delta-icc-asserts: LFLAGS= $(LINK_LIBS)
delta-icc-asserts: EXECUTABLE=delta-icc-asserts
delta-icc-asserts: header build

delta-icc-serial: CC=icpc -std=c++14
delta-icc-serial: DIM=-DDim3
delta-icc-serial: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(INCLUDE_LIBS)
delta-icc-serial: CFLAGS=$(ICCRUN)
delta-icc-serial: LFLAGS= $(LINK_LIBS)
delta-icc-serial: EXECUTABLE=delta-icc-serial
delta-icc-serial: header build

delta-icc-omp: CC=icpc -std=c++14
delta-icc-omp: DIM=-DDim3
delta-icc-omp: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(INCLUDE_LIBS) -DOMPTriangle
delta-icc-omp: CFLAGS=$(ICCRUN) $(ICC_OMP_SHAREDMEMORY)
delta-icc-omp: LFLAGS=$(LINK_OMP) $(LINK_LIBS)
delta-icc-omp: EXECUTABLE=delta-icc-omp
delta-icc-omp: header build

delta-icc-omp-novec: CC=icpc -std=c++14
delta-icc-omp-novec: DIM=-DDim3
delta-icc-omp-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(INCLUDE_LIBS) -DOMPTriangle
delta-icc-omp-novec: CFLAGS=$(ICCNOVEC) $(ICC_OMP_SHAREDMEMORY)
delta-icc-omp-novec: LFLAGS=$(LINK_OMP) $(LINK_LIBS)
delta-icc-omp-novec: EXECUTABLE=delta-icc-omp-novec
delta-icc-omp-novec: header build

delta-icc-release-tbb: CC=icpc -std=c++14
delta-icc-release-tbb: DIM=-DDim3
delta-icc-release-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
delta-icc-release-tbb: CFLAGS=$(ICCRUN) $(INCLUDE_TBB) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS)
delta-icc-release-tbb: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_LIBS)
delta-icc-release-tbb: EXECUTABLE=delta-icc-release-tbb
delta-icc-release-tbb: header build

delta-icc-release-tbb-novec: CC=icpc -std=c++14
delta-icc-release-tbb-novec: DIM=-DDim3
delta-icc-release-tbb-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
delta-icc-release-tbb-novec: CFLAGS=$(ICCNOVEC) $(INCLUDE_TBB) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS)
delta-icc-release-tbb-novec: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_LIBS)
delta-icc-release-tbb-novec: EXECUTABLE=delta-icc-release-tbb-novec
delta-icc-release-tbb-novec: header build

delta-icc-release-pstats: CC=icpc -std=c++14
delta-icc-release-pstats: DIM=-DDim3
delta-icc-release-pstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DSharedTBB
delta-icc-release-pstats: CFLAGS=$(ICCRUN) $(INCLUDE_TBB) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS)
delta-icc-release-pstats: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_LIBS)
delta-icc-release-pstats: EXECUTABLE=delta-icc-release-tbb-pstats
delta-icc-release-pstats: header build

delta-icc-release-tbb-phi: CC=g++ -std=c++14
delta-icc-release-tbb-phi: DIM=-DDim3
delta-icc-release-tbb-phi: SETTINGS=$(DELTAPHI_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB
delta-icc-release-tbb-phi: CFLAGS=$(GCCRUN) $(INCLUDE_TBB) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS)
delta-icc-release-tbb-phi: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_LIBS) -mmic
delta-icc-release-tbb-phi: EXECUTABLE=delta-icc-release-tbb-phi
delta-icc-release-tbb-phi: header build

delta-icc-release-mpi: CC=mpiicc -std=c++14
delta-icc-release-mpi: DIM=-DDim3
delta-icc-release-mpi: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DParallel -DMPICH_IGNORE_CXX_SEEK -DMultipleThreadsMayTriggerMPICalls
delta-icc-release-mpi: CFLAGS=$(ICCRUN) $(INCLUDE_MPI) $(INCLUDE_TBB) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_LIBS) -DSharedTBB
delta-icc-release-mpi: LFLAGS=$(LINK_MPI) $(LINK_TBB) $(LINK_OMP) $(LINK_LIBS)
delta-icc-release-mpi: EXECUTABLE=delta-icc-release-mpi-tbb
delta-icc-release-mpi: header build

################################################################################################################
################################################################################################################
################################################################################################################

OBJECTS=$(SOURCES:.cpp=.o)

files.mk:
	touch files.mk
	echo SOURCES=\\ > files.mk
	find -L $(PROJECT_HOME)  -path delta/core/libraries/ -prune -o -name '*.cpp' | grep -v ./libraries/ | tr '\n' ' ' >> files.mk

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
