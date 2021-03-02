#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/mman.h>
#include "tbb/task_scheduler_init.h"
#include "tbb/tbb.h"

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE-1)

using namespace std;
using namespace tbb;




int  main (int argc, char *argv[]){

	std::cerr << "Starting clock setup" << std::endl;

	int  value_clkps;
	int  value_clkpl;
	int  value_clkwp;

	if (argc != 3){
		std::cerr << "no hay suficientes argumentos: comando freq_fpga=[25|40|50|100|150|200] freq_cpu=[100|200|300|400|600|1200]" << std::endl;
	       exit(0);
	}

	int freq_fpga = atoi(argv[1]);
	int freq_cpu = atoi(argv[2]);

	//open clkwp write protect register
	int memfd_clkwp;
	void *mapped_base_clkwp, *mapped_dev_base_clkwp;
	off_t dev_base_clkwp = 0xFD1A001C;

	memfd_clkwp = open("/dev/mem", O_RDWR | O_SYNC);

	if (memfd_clkwp == -1) {
		std::cerr << "Can't open /dev/mem clkwp." << std::endl;
		exit(0);
	}

	std::cerr << "/dev/mem opened for clkwp." << std::endl;
	// Map one page of memory into user space such that the device is in that page, but it may not
	// be at the start of the page.

	mapped_base_clkwp = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_clkwp, dev_base_clkwp & ~MAP_MASK);

	if (mapped_base_clkwp == (void *) -1) {
		std::cerr << "Can't map the memory to user space clkwp." << std::endl;
		exit(0);
	}

	std::cerr << "CLKWP mapped at address " << mapped_base_clkwp << std::endl;
	// get the address of the device in user space which will be an offset from the base
	// that was mapped as memory is mapped at the start of a page

	mapped_dev_base_clkwp = mapped_base_clkwp + (dev_base_clkwp & MAP_MASK);
	value_clkwp = *((volatile unsigned int *) (mapped_dev_base_clkwp));

	std::cerr << "CLKWP value is " << value_clkwp << std::endl;

	//munmap (mapped_base_clkwp, MAP_SIZE);
	//close (memfd_clkwp);

	std::cerr <<"Finished clock setup" << std::endl;
	//open clkpl

	int memfd_clkpl;
	void *mapped_base_clkpl, *mapped_dev_base_clkpl;
	off_t dev_base_clkpl = 0xFF5E00C0;
	memfd_clkpl = open("/dev/mem", O_RDWR | O_SYNC);

	if (memfd_clkpl == -1) {
		printf("Can't open /dev/mem clkpl.\n");
		exit(0);
	}

	printf("/dev/mem opened for clkpl.\n");
	
	// Map one page of memory into user space such that the device is in that page, but it may not
	// be at the start of the page.
	mapped_base_clkpl = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_clkpl, dev_base_clkpl & ~MAP_MASK);

	if (mapped_base_clkpl == (void *) -1) {
		printf("Can't map the memory to user space clkpl.\n");
		exit(0);
	}

	std::cerr << "CLKPL mapped at address " <<  mapped_base_clkpl << std::endl;
	
	// get the address of the device in user space which will be an offset from the base
	// that was mapped as memory is mapped at the start of a page
	mapped_dev_base_clkpl = mapped_base_clkpl + (dev_base_clkpl & MAP_MASK);

	value_clkpl = *((volatile unsigned int *) (mapped_dev_base_clkpl));
	std::cerr << "CLKPL value is " << value_clkpl << std::endl;

	switch(freq_fpga){
		case 100:
			*((volatile unsigned int *) (mapped_dev_base_clkpl)) = 0x1011E00; //100MHz div1 21:16 div2 13:8
			break;
		case 150:
			*((volatile unsigned int *) (mapped_dev_base_clkpl)) = 0x1011400; //150MHz
			break;
		case 200:
			*((volatile unsigned int *) (mapped_dev_base_clkpl)) = 0x1010F00; //200MHz
			break;
		case 40:
			*((volatile unsigned int *) (mapped_dev_base_clkpl)) = 0x1031900; //40MHz 
			break;
		case 50:
			*((volatile unsigned int *) (mapped_dev_base_clkpl)) = 0x1013C00; //50MHz
			break;
		case 25:
			*((volatile unsigned int *) (mapped_dev_base_clkpl)) = 0x1023C00; //25MHz
			break;
		default:
			std::cerr << "no se reconoce velocidad de la fpga" << std::endl;
	}


	
	//0x1031900 
	value_clkpl = *((volatile unsigned int *) (mapped_dev_base_clkpl));
	
	std::cerr << "CLKPL value is " << value_clkpl << std::endl;

	//munmap(mapped_base_clkpl, MAP_SIZE);
	//close(memfd_clkpl);
	//open clkps

        int memfd_clkps;
	void *mapped_base_clkps, *mapped_dev_base_clkps;
	off_t dev_base_clkps = 0xFD1A0060;

	memfd_clkps = open("/dev/mem", O_RDWR | O_SYNC);
	
	if (memfd_clkps == -1) {
		printf("Can't open /dev/mem clkps.\n");
		exit(0);
	}
	std::cerr << "/dev/mem opened for clkps." << std::endl;

	// Map one page of memory into user space such that the device is in that page, but it may not
	// be at the start of the page.
	mapped_base_clkps = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_clkps, dev_base_clkps & ~MAP_MASK);

	if (mapped_base_clkps == (void *) -1) {
		printf("Can't map the memory to user space clkps.\n");
		exit(0);
	}

	std::cerr << "CLKPS mapped at address " <<  mapped_base_clkps << std::endl;
	
	// get the address of the device in user space which will be an offset from the base
	// that was mapped as memory is mapped at the start of a page
	mapped_dev_base_clkps = mapped_base_clkps + (dev_base_clkps & MAP_MASK);
	value_clkps = *((volatile unsigned int *) (mapped_dev_base_clkps));

	printf("CLKPS value is %x \n",value_clkps);

	switch (freq_cpu){
		case 1200:
			*((volatile unsigned int *) (mapped_dev_base_clkps)) = 0x03000100; // 1200 MHz clock divider from bits 8 to 13
			break;
		case 600:
			*((volatile unsigned int *) (mapped_dev_base_clkps)) = 0x03000200; // 600 MHz clock divider from bits 8 to 13
			break;
		case 400:
			*((volatile unsigned int *) (mapped_dev_base_clkps)) = 0x03000300; // 400 MHz clock divider from bits 8 to 13
			break;
		case 300:
			*((volatile unsigned int *) (mapped_dev_base_clkps)) = 0x03000400; // 300 MHz clock divider from bits 8 to 13
			break;
		case 200:
			*((volatile unsigned int *) (mapped_dev_base_clkps)) = 0x03000600; // 200 MHz clock divider from bits 8 to 13
			break;
		case 100:
			*((volatile unsigned int *) (mapped_dev_base_clkps)) = 0x03000C00; // 100 MHz clock divider from bits 8 to 13
			break;
		default:
			std::cerr << "no se reconoce velocidad de cpu" << std::endl;
	}

	value_clkps = *((volatile unsigned int *) (mapped_dev_base_clkps));
	
	std::cerr << "CLKPS value is " << value_clkps << std::endl;
	
	//munmap(mapped_base_clkps, MAP_SIZE);
	//close(memfd_clkps);
	// done clock mapping

	exit(0);

}
