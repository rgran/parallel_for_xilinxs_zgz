# Author: Kris Nikov - kris.nikov@bris.ac.uk
# Date: 23 Jul 2019
# Description: Makefile used to compile the mssleep program with the energy_meter library

DEBUG_FLAGS = -DDEBUG

LIB_energy_arm = ../lib_energy

CXX= g++

CFLAGS+=  -O2 -I . 

CFLAGS+= -Wno-unused-result -Wno-write-strings -Wno-deprecated-register

XI_CFLAGS = $(CFLAGS) -mcpu=cortex-a53 -I $(LIB_energy_arm) -I ${CURDIR}

#XI_CFLAGS+= $(DEBUG_FLAGS) 

XI_LDFLAGS = -L $(LIB_energy_arm)

LDFLAGS = -pthread -lpthread -z muldefs

XI_LDFLAGS+= -lenergy_meter_arm

XI_PROGs = MSSLEEP

SOURCE= mssleep-top.cpp

all: ultrascale

.PHONY: all

ultrascale: $(XI_PROGs)

$(XI_PROGs): $(SOURCE)
	$(CXX) -o $@ $<  $(XI_CFLAGS) $(XI_LDFLAGS)  $(LDFLAGS)
	
clean:
	${RM} -f  $(XI_PROGs)
	${RM} -f *.txt
	${RM} -f *.dat

