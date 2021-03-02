LIBRARY = libkernelspmv.a

PLATFORM = zcu102

SDSFLAGS = -sds-proc a53 -sds-pf ${PLATFORM} \
	 -sds-hw spmv_simple7 spmv.cpp -clkid 4 -sds-end\
	-Wno-unused-label -Wno-unused-function

#clkid 3 200MHz
#clkid 4 300mhz
#clkid 5 400mhz

DEPS = spmv.h

#SDSFLAGS = -sds-pf ${PLATFORM} \
#	-Wno-unused-label -Wno-unused-function


#CC = sdscc ${SDSFLAGS}
CC = sds++ ${SDSFLAGS}

CFLAGS = -Wall -O3 -c 
LFLAGS = -O3

.PHONY: all

all: ${LIBRARY}

spmv.o: spmv.cpp $(DEPS)
	${CC} ${CFLAGS} $< -o $@

kernelspmv.o: kernelspmv.cpp
	${CC} ${CFLAGS} $< -o $@

# Create dummy linked file to build the hardware
kernelspmv.linked: spmv.o kernelspmv.o
	${CC} $^ -nostartfiles -o $@ 

# Add the interface functions to the library
${LIBRARY}: kernelspmv.linked
	cp _sds/swstubs/${LIBRARY} .
	arm-linux-gnueabihf-ar rcs ${LIBRARY} _sds/swstubs/kernelspmv.o
	ranlib libkernelspmv.a

clean:
	${RM} ${LIBRARY} spmv.o kernelspmv.o  kernelspmv.linked

ultraclean: clean
	${RM}  kernelspmv.linked.bit 
	${RM} -rf _sds sd_card

# LocalWords:  cpp
