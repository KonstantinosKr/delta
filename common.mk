TASK_CPP_OBJ=tasksys.o
TASK_LIB=-lpthread

ifeq ($(REAL),double)
  REAL=-DiREAL=double
else
  REAL=-DiREAL=float
endif

ifeq ($(DEBUG),yes)
  CFLAGS=-Iobjs/ -g -O2 -m64 $(REAL) -DDEBUG $(ZOLTANINC) $(HULLINC)
  ISPC=ispc -g -O2 --arch=x86-64 $(REAL) -DDEBUG
else
  CFLAGS=-Iobjs/ -O2 -std=c11 -m64 $(REAL) $(ZOLTANINC) $(HULLINC)
  ISPC=ispc -O2 --arch=x86-64 --woff $(REAL) 
endif

CC=icc
CXX=icpc

ISPC_OBJS=$(addprefix objs/, $(ISPC_SRC:.ispc=_ispc.o))
ISPC_HEADERS=$(addprefix objs/, $(ISPC_SRC:.ispc=_ispc.h))
CPP_OBJS=$(addprefix objs/, $(CPP_SRC:.cpp=.o) $(TASK_OBJ))
C_OBJS=$(addprefix objs/, $(C_SRC:.c=.o))
LIBS=-lm $(TASK_LIB) -lstdc++ $(ZOLTANLIB) $(HULLLIB)


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
	$(MPICXX) -o $@ $^ $(LIBS)

objs/%_ispc.h objs/%_ispc.o objs/%_ispc_sse2.o objs/%_ispc_sse4.o objs/%_ispc_avx.o: %.ispc
	$(ISPC) --target=$(ISPC_TARGETS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h

objs/%.o: %.cpp $(ISPC_HEADERS)
	$(CXX) $(CFLAGS) $< -c -o $@

objs/%.o: %.c $(ISPC_HEADERS)
	$(MPICXX) $(CFLAGS) $< -c -o $@
