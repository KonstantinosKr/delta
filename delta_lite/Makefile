
-include files.mk

PROJECT_HOME = .

default: header
	@echo Please specify build target. Available targets are:
	@echo clean
	@echo dim3-gcc-asserts
	@echo dim3-gcc-release


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

OBJECTS=$(SOURCES:.cpp=.o)

files.mk:
	touch files.mk
	echo SOURCES=\\ > files.mk
	find -L $(PROJECT_HOME) -name '*.cpp' | tr '\n' ' ' >> files.mk

header:
	@echo  --- This is Delta @ LIGHT ---

build: files.mk $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS) 
	@echo
	@echo build of LIGHT successful

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
	rm -f files.mk


$(OBJECTS): %.o : %.cpp
	$(CC) $(DIM) $(CFLAGS) -I$(PROJECT_HOME) -c $< -o $@