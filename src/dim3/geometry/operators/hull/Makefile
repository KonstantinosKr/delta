include Config.mak

include Flags.mak

CFLAGS = $(STD) $(DEBUG) $(PROFILE) $(NOTHROW) $(MEMDEBUG) $(GEOMDEBUG)

OBJ =   err.o \
        alg.o \
        mem.o \
        kdt.o \
        map.o \
        set.o \
        hul.o \
        tri.o \
        hyb.o \
        spx.o \
        tsi.o \
        predicates.o


default:
	@echo Please select target:
	@echo
	@echo " clean       Clean up project"
	@echo " release     Build release library"
	@echo " debug       Build debug library"

clean:
	rm -f lirm -f *.o
	rm -f *.o


release: $(OBJ)
	ar rcv libhul.a $(OBJ)
	ranlib libhul.a

debug: STD+=-g3
debug: $(OBJ)
	ar rcv libhul-debug.a $(OBJ)
	ranlib libhul-debug.a


err.o: err.c err.h
	$(CC) $(CFLAGS) -c -o $@ $<

alg.o: alg.c alg.h
	$(CC) $(CFLAGS) -c -o $@ $<

mem.o: mem.c mem.h err.h
	$(CC) $(CFLAGS) -c -o $@ $<

kdt.o: kdt.c kdt.h mem.h err.h alg.h
	$(CC) $(CFLAGS) -c -o $@ $<

map.o: map.c map.h mem.h err.h
	$(CC) $(CFLAGS) -c -o $@ $<

set.o: set.c set.h mem.h err.h
	$(CC) $(CFLAGS) -c -o $@ $<

hul.o: hul.c hul.h mem.h alg.h err.h
	$(CC) $(CFLAGS) -c -o $@ $<

tri.o: tri.c tri.h mem.h err.h map.h set.h alg.h
	$(CC) $(CFLAGS) -c -o $@ $<

hyb.o: hyb.c hyb.h err.h alg.h
	$(CC) $(CFLAGS) -c -o $@ $<

box.o: box.c box.h bod.h hyb.h mem.h map.h set.h err.h alg.h
	$(CC) $(CFLAGS) -c -o $@ $<

spx.o: spx.c spx.h
	$(CC) $(CFLAGS) -c -o $@ $<

tsi.o: tsi.c tsi.h alg.h err.h
	$(CC) $(CFLAGS) -c -o $@ $<

predicates.o: predicates.c predicates.h
	$(CC) $(CFLAGS) -c -o $@ $<
