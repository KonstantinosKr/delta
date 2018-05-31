# CPP files
SOURCE = input.cpp geom.cpp material.cpp output.cpp log.cpp delta.cpp migration.cpp loba.cpp contact.cpp bf.cpp forces.cpp dynamics.cpp hybrid.cpp 

# MPI
MPICXXGNU=mpic++
MPICXXINTEL = mpiicpc

# Zoltan paths
# local
 ZOLTANINC = -I/usr/local/include
 ZOLTANLIB = -L/usr/local/lib -lzoltan                                                                                 
# Polaris 
# ZOLTANINC = -I/home/polaris_dur1/durkk/zlocal/include                                                                  
# ZOLTANLIB = -L/home/polaris_dur1/durkk/zlocal/lib -lzoltan                                                             
# Hamilton                                                                                                              
# ZOLTANINC = -I/home/hudson/pg/rfmw74/zlocal/include                                                                   
# ZOLTANLIB = -L/home/hudson/pg/rfmw74/zlocal/lib -lzoltan  

HULLINC = -Iext/hul
HULLLIB = -Lext/hul -lhul

# Python paths
PYTHONINC=-I/usr/include/python2.7
PYTHONLIB=-L/usr/lib -lpython2.7

# Program name
EXE=delta

# Floating point type
REAL=double

# Debug version
DEBUG=yes

ifeq ($(REAL),double)
  REAL=-DiREAL=double
else
  REAL=-DiREAL=float
endif

default: header
	@echo Please specify build target. Available targets are:
	@echo clean
	@echo delta-gcc-asserts
	@echo delta-gcc-release
	@echo delta-icc-asserts
	@echo delta-icc-release-novec
	@echo delta-icc-release-vec

delta-gcc-asserts: CXX=$(MPICXXGNU) -std=c++0x
delta-gcc-asserts: CFLAGS= $(REAL) -Wall -pedantic -pedantic-errors -O0 $(ZOLTANINC) $(HULLINC)
delta-gcc-asserts: LFLAGS= -lm -lstdc++ $(ZOLTANLIB) $(HULLLIB) $(PYTHONLIB)
delta-gcc-asserts: EXECUTABLE=deltaZoltan-gcc-asserts
delta-gcc-asserts: header build

delta-gcc-release: CXX=$(MPICXXGNU) -std=c++0x
delta-gcc-release: CFLAGS=$(REAL) -Wall -pedantic -pedantic-errors -O3 $(ZOLTANINC) $(HULLINC)
delta-gcc-release: LFLAGS= -lm -lstdc++ $(ZOLTANLIB) $(HULLLIB) $(PYTHONLIB)
delta-gcc-release: EXECUTABLE=deltaZoltan-gcc-release
delta-gcc-release: header build

delta-icc-asserts: CXX=$(MPICXXINTEL) --std=c++11
delta-icc-asserts: CFLAGS=$(REAL) -O0 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost $(ZOLTANINC) $(HULLINC) 
delta-icc-asserts: LFLAGS=-lrt -lm -lstdc++ $(ZOLTANLIB) $(HULLLIB) $(PYTHONLIB)
delta-icc-asserts: EXECUTABLE=deltaZoltan-icc-asserts
delta-icc-asserts: header build

delta-icc-release-novec: CXX=$(MPICXXINTEL) --std=c++11
delta-icc-release-novec: CFLAGS=$(REAL) -O0 -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost -no-vec $(ZOLTANINC) $(HULLINC)
delta-icc-release-novec: LFLAGS=-qopenmp -lm -lstdc++ $(ZOLTANLIB) $(HULLLIB) $(PYTHONLIB)
delta-icc-release-novec: EXECUTABLE=deltaZoltan-icc-release-novec
delta-icc-release-novec: header build

delta-icc-release-vec: CXX=$(MPICXXINTEL) --std=c++11
delta-icc-release-vec: CFLAGS=$(REAL) -fast -qopenmp -fstrict-aliasing -restrict -fno-rtti -no-ipo -ip -xHost $(ZOLTANINC) $(HULLINC)
delta-icc-release-vec: LFLAGS=-qopenmp -lm -lstdc++ $(ZOLTANLIB) $(HULLLIB) $(PYTHONLIB)
delta-icc-release-vec: EXECUTABLE=deltaZoltan-icc-release-vec
delta-icc-release-vec: header build

header: 
	@echo  --- This is Delta @ Zoltan ---

OBJECTS=$(SOURCE:.cpp=.o)

build: $(SOURCE) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS) 
	@echo
	@echo build of Delta successful

$(OBJECTS): %.o : %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(LFLAGS)


clean:
	/bin/rm -rf output/dump/*
	/bin/rm -rf output/mpi/*
	/bin/rm -rf output/data/*
	/bin/rm -r *.o
	find ./ -iname "*.dump" -exec rm '{}' ';'

