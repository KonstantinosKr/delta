# Include files
-include files.mk

PROJECT_HOME = .

# Configure System
# ----------------
INCLUDE_TBB=$(TBB_INC)
INCLUDE_OMP=
INCLUDE_MPI=$(MPI_INC)
INCLUDE_LIBS=$(ASSIMP_INC) #export ASSIMP_INC=-I/Users/konstantinos/Devel/libraries/assimp/include/

LINK_TBB=$(TBB_SHLIB)
LINK_OMP=$(OMP_SHLIB)
LINK_OMP=-fopenmp
LINK_MPI=-L/usr/local/Cluster-Apps/openmpi/gcc-4.9.1/1.8.2/lib/ -lmpi
LINK_LIBS=$(ASSIMP_LIB) #export ASSIMP_LIB="-L/Users/konstantinos/Devel/libraries/assimp/lib/ -lassimp"

##############################################################################################

GCC_OMP_SHAREDMEMORY=-fopenmp
ICC_OMP_SHAREDMEMORY=-qopenmp -qopt-threads-per-core=1

##############################################################################################

#DELTA_STATS=-DPARTICLESTATSFULL -DCONTACTSTATS
DELTA_STATS=-DPARTICLESTATSMIN
DELTA_TUNE=-DDELTATUNE

DELTAPHI_SETTINGS=-DiREAL=double -DbyteAlignment=64

DELTA_SETTINGS=-DiREAL=double -DbyteAlignment=32
PEANO_SETTINGS=-DTrackGridStatistics

##############################################################################################

GCCRUN=-O3 -Wall -ftree-vectorize -ftree-vectorizer-verbose=3 -ftree-loop-optimize -faggressive-loop-optimizations -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fstrict-aliasing
GCCRUNCNT=-O3 -Wall -g -ftree-vectorize -ftree-vectorizer-verbose=3 -ftree-loop-optimize -faggressive-loop-optimizations -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fstrict-aliasing
GCCDEBUG=-O3 -Wall -g -DDebug -DAsserts -pedantic -pedantic-errors
GCCNOVEC=-O3 -Wall -fno-tree-vectorize -ftree-vectorizer-verbose=3 -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing
GCCNOVECCNT=-O3 -Wall -g -fno-tree-vectorize -ftree-vectorizer-verbose=3 -funroll-loops -fprefetch-loop-arrays -falign-loops -fopt-info-vec -fopt-info-loop -fopenmp -fstrict-aliasing
##############################################################################################

ICCRUN=-O3 -Wall -simd-function-pointers -qopt-report=5 -qopt-prefetch=2 -falign-loops=32 -funroll-loops -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHOST
ICCRUNCNT=-O3 -g -Wall -simd-function-pointers -qopt-report=5 -qopt-prefetch=2 -falign-loops=32 -funroll-loops -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHOST
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
	@echo delta-gcc-release-tbb-novec
	@echo delta-gcc-asserts-tbb-counter
	@echo delta-gcc-release-tbb-novec-counter
	@echo delta-gcc-release-mpi
	@echo --ICC-------------------------------
	@echo delta-icc-asserts
	@echo delta-icc-release-pstats
	@echo delta-icc-release-dstats
	@echo delta-icc-release-tbb-novec
	@echo delta-icc-asserts-tbb-counter
	@echo delta-icc-release-tbb-novec-counter
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
delta-gcc-release-pstats: CFLAGS=$(GCCRUN) $(GCC_OMP_SHAREDMEMORY)
delta-gcc-release-pstats: LFLAGS=$(LINK_OMP)
delta-gcc-release-pstats: EXECUTABLE=delta-gcc-release-tbb-stats
delta-gcc-release-pstats: header build

delta-gcc-release-dstats: CC=g++ -std=c++14
delta-gcc-release-dstats: DIM=-DDim3
delta-gcc-release-dstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_TUNE) -DSharedTBB -DompTriangle
delta-gcc-release-dstats: CFLAGS=$(GCCRUN) $(GCC_OMP_SHAREDMEMORY)
delta-gcc-release-dstats: LFLAGS=$(LINK_OMP)
delta-gcc-release-dstats: EXECUTABLE=delta-gcc-release-tbb-dstats
delta-gcc-release-dstats: header build

delta-gcc-release-tbb: CC=g++ -std=c++14
delta-gcc-release-tbb: DIM=-DDim3
delta-gcc-release-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb: CFLAGS=$(GCCRUN) $(INCLUDE_TBB)
delta-gcc-release-tbb: LFLAGS=$(LINK_TBB)
delta-gcc-release-tbb: EXECUTABLE=delta-gcc-release-tbb
delta-gcc-release-tbb: header build

delta-gcc-release-tbb-counter: CC=g++ -std=c++14
delta-gcc-release-tbb-counter: DIM=-DDim3
delta-gcc-release-tbb-counter: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-counter: CFLAGS=$(GCCRUNCNT $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-counter: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-counter: EXECUTABLE=delta-gcc-release-tbb-counter
delta-gcc-release-tbb-counter: header build

delta-gcc-release-tbb-novec: CC=g++ -std=c++14
delta-gcc-release-tbb-novec: DIM=-DDim3
delta-gcc-release-tbb-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-novec: CFLAGS=$(GCCNOVEC) $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-novec: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-novec: EXECUTABLE=delta-gcc-release-tbb-novec
delta-gcc-release-tbb-novec: header build

delta-gcc-release-tbb-novec-counter: CC=g++ -std=c++14
delta-gcc-release-tbb-novec-counter: DIM=-DDim3
delta-gcc-release-tbb-novec-counter: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-novec-counter: CFLAGS=$(GCCNOVECCNT) $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-novec-counter: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-gcc-release-tbb-novec-counter: EXECUTABLE=delta-gcc-release-tbb-novec-counter
delta-gcc-release-tbb-novec-counter: header build

delta-gcc-release-tbb-phi: CC=g++ -std=c++14
delta-gcc-release-tbb-phi: DIM=-DDim3
delta-gcc-release-tbb-phi: SETTINGS=$(DELTAPHI_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-gcc-release-tbb-phi: CFLAGS=$(GCCRUN) $(GCC_OMP_SHAREDMEMORY) $(INCLUDE_TBB)
delta-gcc-release-tbb-phi: LFLAGS=$(LINK_OMP) $(LINK_TBB) -mmic
delta-gcc-release-tbb-phi: EXECUTABLE=delta-gcc-release-tbb-phi
delta-gcc-release-tbb-phi: header build

delta-gcc-release-mpi: CC=mpic++ -std=c++14
delta-gcc-release-mpi: DIM=-DDim3
delta-gcc-release-mpi: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DParallel -DMPICH_IGNORE_CXX_SEEK -DMultipleThreadsMayTriggerMPICalls 
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

delta-icc-release-tbb: CC=icpc -std=c++14
delta-icc-release-tbb: DIM=-DDim3
delta-icc-release-tbb: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB# -DompTriangle
delta-icc-release-tbb: CFLAGS=$(ICCRUN) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_TBB) $(INCLUDE_LIBS)
delta-icc-release-tbb: LFLAGS=$(LINK_TBB) $(LINK_OMP) $(LINK_LIBS)
delta-icc-release-tbb: EXECUTABLE=delta-icc-release-tbb
delta-icc-release-tbb: header build

delta-icc-release-tbb-counter: CC=icpc -std=c++14
delta-icc-release-tbb-counter: DIM=-DDim3
delta-icc-release-tbb-counter: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-counter: CFLAGS=$(ICCCNT) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-counter: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-counter: EXECUTABLE=delta-icc-release-tbb-counter
delta-icc-release-tbb-counter: header build

delta-icc-release-tbb-novec: CC=icpc -std=c++14
delta-icc-release-tbb-novec: DIM=-DDim3
delta-icc-release-tbb-novec: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-novec: CFLAGS=$(ICCNOVEC) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-novec: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-novec: EXECUTABLE=delta-icc-release-tbb-novec
delta-icc-release-tbb-novec: header build

delta-icc-release-tbb-novec-counter: CC=icpc -std=c++14
delta-icc-release-tbb-novec-counter: DIM=-DDim3
delta-icc-release-tbb-novec-counter: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DompTriangle
delta-icc-release-tbb-novec-counter: CFLAGS=$(ICCNOVECCNT) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_TBB)
delta-icc-release-tbb-novec-counter: LFLAGS=$(LINK_TBB) $(LINK_OMP)
delta-icc-release-tbb-novec-counter: EXECUTABLE=delta-icc-release-tbb-novec-counter
delta-icc-release-tbb-novec-counter: header build

delta-icc-release-pstats: CC=icpc -std=c++14
delta-icc-release-pstats: DIM=-DDim3
delta-icc-release-pstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_STATS) -DSharedTBB -DompTriangle
delta-icc-release-pstats: CFLAGS=$(ICCRUN) $(ICC_OMP_SHAREDMEMORY)
delta-icc-release-pstats: LFLAGS=$(LINK_OMP)
delta-icc-release-pstats: EXECUTABLE=delta-icc-release-tbb-pstats
delta-icc-release-pstats: header build

delta-icc-release-dstats: CC=icpc -std=c++14
delta-icc-release-dstats: DIM=-DDim3
delta-icc-release-dstats: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) $(DELTA_TUNE) -DSharedTBB -DompTriangle
delta-icc-release-dstats: CFLAGS=$(ICCRUN) $(ICC_OMP_SHAREDMEMORY)
delta-icc-release-dstats: LFLAGS=$(LINK_OMP)
delta-icc-release-dstats: EXECUTABLE=delta-icc-release-tbb-dstats
delta-icc-release-dstats: header build

delta-icc-release-mpi: CC=mpiicc -std=c++14
delta-icc-release-mpi: DIM=-DDim3
delta-icc-release-mpi: SETTINGS=$(DELTA_SETTINGS) $(PEANO_SETTINGS) -DSharedTBB -DParallel -DMPICH_IGNORE_CXX_SEEK -DMultipleThreadsMayTriggerMPICalls 
delta-icc-release-mpi: CFLAGS=$(ICCRUN) $(ICC_OMP_SHAREDMEMORY) $(INCLUDE_MPI) $(INCLUDE_TBB)
delta-icc-release-mpi: LFLAGS=$(LINK_MPI) $(LINK_OMP) $(LINK_TBB)
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
