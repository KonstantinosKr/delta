ifeq ($(REAL),double)
  REAL=-DiREAL=double
else
  REAL=-DiREAL=float
endif

ifeq ($(DEBUG),yes)
  CFLAGS=-Iobjs/ -g -Wall -O0 -m64 $(REAL) $(HULLINC) -fopenmp -DDEBUG
  ISPC=ispc -g -O0 --arch=x86-64 $(REAL) -DDEBUG
else
  CFLAGS=-Iobjs/ -O2 -m64 $(REAL) $(HULLINC) -fopenmp
  ISPC=ispc -O2 --arch=x86-64 --woff $(REAL) 
endif

ISPC_OBJS=$(addprefix objs/, $(ISPC_SRC:.ispc=_ispc.o)) 
ISPC_HEADERS=$(addprefix objs/, $(ISPC_SRC:.ispc=_ispc.h))
CPP_OBJS=$(addprefix objs/, $(CPP_SRC:.cpp=.o))
C_OBJS=$(addprefix objs/, $(C_SRC:.c=.o))
LIBS=-lm -lstdc++ $(HULLLIB) #$(PYTHONLIB)

default: dirs $(ISPC_HEADERS) $(CPP_OBJS) $(C_OBJS) $(EXE)

.PHONY: dirs clean print

print:
	@echo $(ISPC_HEADERS)
	@echo $(CPP_OBJS)
	@echo $(C_OBJS)
	@echo $(ISPC_OBJS)

dirs:
	/bin/mkdir -p objs/

del:
	find ./ -iname "*.dump" -exec rm '{}' ';'

clean:
	/bin/rm -rf objs *~ $(EXE)
	find ./ -iname "*.dump" -exec rm '{}' ';'

$(EXE): $(CPP_OBJS) $(C_OBJS) $(ISPC_OBJS)
	$(CXX) $(CFLAGS) -fopenmp -o $@ $^ $(LIBS)

objs/%_ispc.h objs/%_ispc.o: %.ispc
	$(ISPC) --target=$(ISPC_TARGETS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h

objs/%.o: %.cpp $(ISPC_HEADERS)
	$(CXX) $(CFLAGS) $< -c -o $@

objs/%.o: %.c $(ISPC_HEADERS)
	$(CXX) $(CFLAGS) $< -c -o $@
