# Author: Kris Nikov - kris.nikov@bris.ac.uk
# Date: 08 Apr 2019
# Description: Makefile used to compile SPMM with energy monitoring and FPGA accelerator support for the ultrascale+ devboard

hpc: LIB_spmm = ./lib_spmm/hpc
hpc: CONFIG_FLAG = -DHPC
     
hp: LIB_spmm = ./lib_spmm/hp
hp: CONFIG_FLAG = -DHP

#SCHEDULER = MULTIDYNAMIC
#SCHEDULER = MULTIHAP
SCHEDULER = MULTIFASTFIT
#SCHEDULER = MULTIORACLE


DEBUG_FLAGS = -DDEBUG 
DEBUG_FLAGS += -DDEBUG_CHUNKS

LIB_energy_arm =../lib_energy

CXX= g++

# old CFLAGS, -O2
CFLAGS +=  -O3 -g2 -I ./ 

CFLAGS += -Wno-unused-result -Wno-write-strings -Wno-deprecated-register

XI_CFLAGS = $(CFLAGS) -DXILINX -DOFFLOAD -DENERGY --std=gnu++11 -mcpu=cortex-a53 $(CONFIG_FLAG) -D$(SCHEDULER) -I$(LIB_spmm) -I$(LIB_energy_arm) -I${CURDIR}  -I./schedulers

XI_CFLAGS += -DNEON -march=armv8-a+crypto -funsafe-math-optimizations -ftree-vectorize -ftree-vectorizer-verbose=2 #If NEON SIMD instructions are to be used

#XI_CFLAGS+= $(DEBUG_FLAGS) 

LDFLAGS = -ltbbmalloc_proxy -pthread -lpthread -ltbb -z muldefs

XI_LDFLAGS = $(LDFLAGS) -L$(LIB_spmm) -L$(LIB_energy_arm)

XI_LDFLAGS += -lrt -lkernelspmm -lenergy_meter_arm

XI_PROGs = SPMM_fast_$(SCHEDULER)

SOURCE = spmm-top.cpp kernelspmm.h SPMM.h

ARMPL_CFLAGS:= -DARMPL_MULT -I$(ARMPL_DIR)/include
ARMPL_CLINKLIBS:= -L$(ARMPL_DIR)/lib -larmpl -lgfortran -lamath -lm

hpc hp: ultrascale

all: hp hpc

.PHONY: all

spmm_armpl: spmm_armpl.cpp SPMM.h
	$(CXX) -o $@ $< $(ARMPL_CFLAGS)	$(XI_CFLAGS) $(LDFLAGS) $(ARMPL_CLINKLIBS)

ultrascale: $(XI_PROGs) $(XI_PROGs)_armpl

$(XI_PROGs): $(SOURCE)
	$(CXX) -o $@ $< $(XI_CFLAGS) $(XI_LDFLAGS) $(LDFLAGS)

$(XI_PROGs)_armpl: $(SOURCE)
	$(CXX) -o $@ $< $(ARMPL_CFLAGS) $(XI_CFLAGS) $(XI_LDFLAGS) $(LDFLAGS) $(ARMPL_CLINKLIBS)
	
clean:
	${RM} -f  $(XI_PROGs)
#	${RM} -f *.txt
	${RM} -f spmm_fast 
#	${RM} -f *.dat


