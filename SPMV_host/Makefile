
# SPMV

LIB_spmv = ./lib_spmv

CXX= g++

CFLAGS+=  -O2 -I . 

CFLAGS+= -Wno-unused-result -Wno-write-strings -Wno-deprecated-register

XI_CFLAGS = $(CFLAGS) -DXILINX -DOFFLOAD -mcpu=cortex-a53 -I $(LIB_spmv)  -I ${CURDIR}  -I ./Schedulers

XI_CFLAGS+= -DNEON  -funsafe-math-optimizations -ftree-vectorize -ftree-vectorizer-verbose=2 #If NEON SIMD instructions are to be used


XI_LDFLAGS = -L $(LIB_spmv)

LDFLAGS = -pthread -lpthread -ltbb

XI_LDFLAGS+= -lrt -lkernelspmv 

XI_PROGs= SPMV

SOURCE= spmv-top.cpp   kernelspmv.h

.PHONY: all clean odroid

all: odroid

odroid: $(XI_PROGs)


SPMV: $(SOURCE)
	$(CXX) -o $@ $<  $(XI_CFLAGS) $(XI_LDFLAGS)  $(LDFLAGS)
	

clean:
	rm -f  $(XI_PROGs)

