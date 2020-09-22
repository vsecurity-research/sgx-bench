## Aisha Hasan
# 
# Taking lmbench's Makefile (src/Makefile) and paring it down
# to compile with SGX programs.
# 

SHELL=/bin/sh

CC=`../scripts/compiler`
MAKE=`../scripts/make`
AR=ar
ARCREATE=cr

# base of installation location
O= ../bin/unknown
OS=`../scripts/os`
BINDIR=../bin/$(OS)
UTILS=../scripts/os \
		../scripts/compiler \
		../scripts/make \



COMPILE=$(CC) $(CFLAGS)

INCS =	bench.h timing.h


SRCS =  getopt.c lib_timing.c bench.h timing.h

OPT_EXES=$O/lat_rand $O/lat_syscall $O/bw_mem

LIBOBJS= $O/lib_timing.o $O/getopt.o



CFLAGS=-O -DRUSAGE -DHAVE_uint=1 -DHAVE_int64_t=1 -DHAVE_DRAND48 -DHAVE_SCHED_SETAFFINITY=1
LDLIBS=-lm
${O}=${BINDIR}



lmbench: $(UTILS)
	$(info "building this first")
	if [ ! -d ${BINDIR} ]; then mkdir -p ${BINDIR}; fi
	${MAKE} OS="${OS}" CC="${CC}" CFLAGS="${CFLAGS}" LDLIBS="${LDLIBS}" O="${BINDIR}" opt

	

#all: $(EXES) $O/lmbench
opt: $(OPT_EXES)


clean:
	/bin/rm -f ../bin/*/CONFIG ../bin/*/*.[oas]
	/bin/rm -f *.[oas]
	-bk clean


.PHONY: lmbench all clean 


$O/lmbench.a: $(LIBOBJS)
	/bin/rm -f $O/lmbench.a
	$(AR) $(ARCREATE) $O/lmbench.a $(LIBOBJS)
	-ranlib $O/lmbench.a

$O/lib_timing.o : lib_timing.c $(INCS)
	$(COMPILE) -c lib_timing.c -o $O/lib_timing.o
$O/getopt.o : getopt.c $(INCS)
	$(COMPILE) -c getopt.c -o $O/getopt.o

# $(UTILS) :
# 	-cd ../scripts; make get

$O/bw_mem: bw_mem.c timing.h bench.h $O/lmbench.a
	$(COMPILE) -o $O/bw_mem bw_mem.c $O/lmbench.a $(LDLIBS)

$O/lat_rand:  lat_rand.c timing.h bench.h $O/lmbench.a
	$(COMPILE) -o $O/lat_rand lat_rand.c $O/lmbench.a $(LDLIBS)

$O/lat_syscall:  lat_syscall.c timing.h bench.h $O/lmbench.a
	$(COMPILE) -o $O/lat_syscall lat_syscall.c $O/lmbench.a $(LDLIBS)
