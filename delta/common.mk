ifeq ($(REAL),double)
  REAL=-DiREAL=double
else
  REAL=-DiREAL=float
endif

ifeq ($(DEBUG),yes)
  CFLAGS=-Iobjs/ -g -Wall -O0 -m64 $(REAL) $(ZOLTANINC) $(HULLINC) -fopenmp -DDEBUG
  ISPC=ispc -g -O0 --arch=x86-64 $(REAL) -DDEBUG
else
  CFLAGS=-Iobjs/ -O2 -m64 $(REAL) $(ZOLTANINC) $(HULLINC) -fopenmp
  ISPC=ispc -O2 --arch=x86-64 --woff $(REAL) 
endif

ISPC_OBJS=$(addprefix objs/, $(ISPC_SRC:.ispc=_ispc.o) $(ISPC_SRC:.ispc=_ispc_sse2.o) $(ISPC_SRC:.ispc=_ispc_sse4.o) $(ISPC_SRC:.ispc=_ispc_avx.o)) 
ISPC_HEADERS=$(addprefix objs/, $(ISPC_SRC:.ispc=_ispc.h))
CPP_OBJS=$(addprefix objs/, $(CPP_SRC:.cpp=.o))
C_OBJS=$(addprefix objs/, $(C_SRC:.c=.o))
LIBS=-lm -lstdc++ $(ZOLTANLIB) $(HULLLIB) $(PYTHONLIB)

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
	$(MPICXX) $(CFLAGS) -fopenmp -o $@ $^ $(LIBS)

objs/%_ispc.h objs/%_ispc.o objs/%_ispc_sse2.o objs/%_ispc_sse4.o objs/%_ispc_avx.o: %.ispc
	$(ISPC) --target=$(ISPC_TARGETS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h

objs/input.o: input.cpp
	$(CXX) $(CFLAGS) $(PYTHONINC) $< -c -o $@

objs/tasksys.o: tasksys.cpp
	$(CXX) $(CFLAGS) -D ISPC_USE_OMP $< -c -o $@

objs/%.o: %.cpp $(ISPC_HEADERS)
	$(CXX) $(CFLAGS) $< -c -o $@

objs/%.o: %.c $(ISPC_HEADERS)
	$(MPICXX) $(CFLAGS) $< -c -o $@

