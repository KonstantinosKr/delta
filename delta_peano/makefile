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
	@echo dim2-gcc-asserts
	@echo dim2-gcc-release
	@echo dim3-gcc-asserts
	@echo dim3-gcc-release
	@echo dim3-icc-asserts
	@echo dim3-icc-release-novec
	@echo dim3-icc-release-vec



#	@echo dim2-mpi
#	@echo dim3-mpi


#dim2-nompi: CC=$(CC_NOMPI)
#dim2-nompi: DIM=-DDim2
#dim2-nompi: PROJECT_CFLAGS=
#dim2-nompi: PROJECT_LFLAGS=
#dim2-nompi: EXECUTABLE=peano-noMPI-noTBB-DDim2
#dim2-nompi: all

dim3-gcc-asserts: CC=g++ -std=c++0x
dim3-gcc-asserts: DIM=-DDim3 -DAsserts
dim3-gcc-asserts: CFLAGS=-DiREAL=double -Wall -pedantic -pedantic-errors -O3
dim3-gcc-asserts: LFLAGS=
dim3-gcc-asserts: EXECUTABLE=dem-3d-asserts
dim3-gcc-asserts: header build

dim3-gcc-release: CC=g++ -std=c++0x
dim3-gcc-release: DIM=-DDim3
dim3-gcc-release: CFLAGS=-DiREAL=double -Wall -pedantic -pedantic-errors -O3
dim3-gcc-release: LFLAGS=
dim3-gcc-release: EXECUTABLE=dem-3d-release
dim3-gcc-release: header build


dim3-icc-asserts: CC=icpc --std=c++11
dim3-icc-asserts: DIM=-DDim3 -DAsserts 
dim3-icc-asserts: CFLAGS=-DiREAL=double -fast -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost 
dim3-icc-asserts: LFLAGS=-lrt
dim3-icc-asserts: EXECUTABLE=dem-3d-asserts
dim3-icc-asserts: header build

dim3-icc-release-novec: CC=icpc --std=c++11
dim3-icc-release-novec: DIM=-DDim3
dim3-icc-release-novec: CFLAGS=-DiREAL=double -O0 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost -no-vec
dim3-icc-release-novec: LFLAGS=-qopenmp
	#-lrt
dim3-icc-release-novec: EXECUTABLE=dem-3d-release-novec
dim3-icc-release-novec: header build

dim3-icc-release-vec: CC=icpc --std=c++11
dim3-icc-release-vec: DIM=-DDim3
dim3-icc-release-vec: CFLAGS=-DiREAL=double -fast -qopenmp -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost
dim3-icc-release-vec: LFLAGS=-qopenmp
	#-lrt 
dim3-icc-release-vec: EXECUTABLE=dem-3d-release-vec
dim3-icc-release-vec: header build



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
	$(CC) $(DIM) $(CFLAGS) -I$(PROJECT_HOME) -c $< -o $@

