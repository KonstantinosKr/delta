ifeq ($(REAL),double)
  REAL=-DiREAL=double
else
  REAL=-DiREAL=float
endif

ifeq ($(DEBUG),yes)
  CFLAGS=-Iobjs/ -g -Wall -O0 -m64 $(REAL) -fopenmp
else
  CFLAGS=-Iobjs/ -O2 -m64 $(REAL) -fopenmp
endif

CPP_OBJS=$(addprefix objs/, $(CPP_SRC:.cpp=.o))
C_OBJS=$(addprefix objs/, $(C_SRC:.c=.o))
LIBS=-lm -lstdc++ 

default: dirs $(CPP_OBJS) $(C_OBJS) $(EXE)

.PHONY: dirs clean print

print:
	@echo $(CPP_OBJS)
	@echo $(C_OBJS)

dirs:
	/bin/mkdir -p objs/

del:
	find ./ -iname "*.dump" -exec rm '{}' ';'

clean:
	/bin/rm -rf objs *~ $(EXE)
	find ./ -iname "*.dump" -exec rm '{}' ';'

lib: $(CPP_OBJS) 
		ar rcv libdelta.a $(CPP_OBJS)
		ranlib libdelta.a

$(EXE): $(CPP_OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)
	
objs/%.o: %.cpp 
	$(CXX) $(CFLAGS) $< -c -o $@

objs/%.o: %.c 
	$(CXX) $(CFLAGS) $< -c -o $@

