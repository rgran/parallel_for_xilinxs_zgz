#include <stdio.h>
#include <sds_lib.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/mman.h>
#include "tbb/task_scheduler_init.h"
#include "tbb/tbb.h"

#include "Body.h"

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE-1)


#ifdef MULTIDYNAMIC
     #include "MultiDynamic.h"
#endif
#ifdef MULTIHAP
     #include "MultiHap.h"
#endif


#ifdef MULTIFASTFIT
#include "MultiFastFit.h"
#endif
#ifdef MULTIORACLE
#include "MultiOracle.h"
#endif

using namespace std;
using namespace tbb;

// Euclidean algorithm
// Dario, not tested
static int gcd(int a, int b)
{
  while(b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }
  return a;
}

//least common multiple
int lcm( int num1, int num2)
{

   int i, gcd_val, lcm;
#if 1
   //calculation of gcd
   for(i=1; i <= num1 && i <= num2; ++i)
   {
      if(num1 % i == 0 && num2 % i == 0)
      gcd_val = i;
   }
#else 
  gcd_val =gcd(num1, num2);
#endif
   //calculation of lcm using gcd
   lcm = (num1 * num2) / gcd_val;
   cout << "LCM: " << lcm << endl;

   return lcm;
}

int main(int argc, char* argv[]) {

     printf("Starting\n");
     
     //variables	
     Body body;
	Params p;
     char *tfile, *pfile;
    	
     if (argc < 11)
		usage(argc, argv);
	if(  atoi(argv[1])<=0 ||
          ( atoi(argv[4]) != 0 && atoi(argv[4]) != 1 ) ||
          atoi(argv[5]) < 0 || atoi(argv[5]) > 4 
          || atoi(argv[6]) < 0 
          || atoi(argv[7]) < 0
          || atoi(argv[8]) < 0 
          || ( atoi(argv[9]) != 0 && atoi(argv[9]) != 1 ) )
		usage(argc, argv);
     else
     grid_rows = atoi(argv[1]);
     grid_cols = grid_rows;
     tfile = argv[2];
     pfile = argv[3];
	cont_read = atoi(argv[4]);
     
     p.numcpus = atoi(argv[5]);
     numhpacc = atoi(argv[6]);
     numhpcacc = atoi(argv[7]);	
     p.numgpus	= numhpacc + numhpcacc;
     int freq_cpu = atoi(argv[11]);
     int freq_fpga = atoi(argv[12]);
     
          strcpy(p.benchName,"HOTSPOT"); 
     
      #ifdef MULTIFASTFIT
        p.initiationInterval=1;
        p.WARMUPFPGA=true;
        p.frequency = 200; //(float) atof(argv[8]);
      //  p.thobjective =90;
		//if (argc > 10)
		//	p.thobjective = (float) atof(argv[10]);
	    FastFit * hs = FastFit::getInstance(&p);
	 #endif

	#ifdef MULTIDYNAMIC
  		p.gpuChunk = atoi(argv[8]);
  		Dynamic * hs = Dynamic::getInstance(&p);
  	#endif
     #ifdef MULTIHAP // new multihap
          p.chunkGPU_initratio = (float) atof(argv[8]);
          // lcm == least common multiple
          // the scheduler will tune chunk size to lay in a 64 bytes offset block
          int alignment = lcm(2, grid_cols * sizeof(float))/(grid_cols * sizeof(float)); //find best alignment for chunnk sizes
          cout << "ALING CHUNK SIZE: " << alignment << endl;
          p.CHUNKCPUALIGN = alignment; // set to 1 if no alignment required
          p.CHUNKGPUALIGN = alignment; // set to 1 if no alignment required
          Hap * hs = Hap::getInstance(&p);
     #endif
     
     
     ioctl_flag = atoi(argv[9]);
     
      
	 #ifdef MULTIORACLE
	    p.frequency = freq_fpga*1000000; 
        p.ioctl_flag=ioctl_flag;
		p.ratioG = (float) atof(argv[8]);
	    Oracle * hs = Oracle::getInstance(&p);
	 #endif
 
	std::cerr << "Starting clock setup" << std::endl;

     int  value_clkps;
     int  value_clkpl;
     int  value_clkwp;

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







   
     size=(grid_rows+2)*(grid_cols+2);
     array_temp = (float *)sds_alloc(size*sizeof(float));
     array_power = (float *)sds_alloc(size*sizeof(float));
     array_out = (float *)sds_alloc(size*sizeof(float));
     if(!array_temp || !array_power || !array_out)
     {
        cerr << "ERROR: Unable to allocate memory\n" <<endl;
        exit(1);
     }

     //Only allocate noncache memory if HP enabled accelerators are selected
     if (numhpacc > 0){    
          array_temp_noncache =  (float *)sds_alloc_non_cacheable(size*sizeof(float));
          array_power_noncache = (float *)sds_alloc_non_cacheable(size*sizeof(float));
          array_out_noncache = (float *)sds_alloc_non_cacheable(size*sizeof(float));
          if(!array_temp_noncache)
          {
             cerr << "ERROR: Unable to allocate noncache memory\n" <<endl;
             freemem();
             exit(1);
          }          
     }
     (debug_flag) && (printf("Memories allocated\n"));
    	
     // Read input data from disk
     readinput(array_temp, (grid_rows+2), (grid_cols+2), tfile);
     readinput(array_power, (grid_rows+2), (grid_cols+2), pfile); 
        
     //Interrupt device drivers defined in HOTSPOT.h
     //Only open if ioctl_flag is enabled
     if (ioctl_flag > 0){
     /*Interrupt drivers*/
          file_desc_1 = open(DRIVER_FILE_NAME_1, O_RDWR);	//Open interrupt driver 1
          if (file_desc_1 < 0) {
               fprintf(stderr,"Can't open driver file: %s\n", DRIVER_FILE_NAME_1);
               exit(-1);
          } else {
               (debug_flag) && (fprintf(stderr,"Driver successfully opened: %s\n", DRIVER_FILE_NAME_1));
          }
          file_desc_2 = open(DRIVER_FILE_NAME_2, O_RDWR);	//Open interrupt driver 2
          if (file_desc_2 < 0) {
               fprintf(stderr,"Can't open driver file: %s\n", DRIVER_FILE_NAME_2);
               exit(-1);
          } else {
               (debug_flag) && (fprintf(stderr,"Driver successfully opened: %s\n", DRIVER_FILE_NAME_2));
          }
          file_desc_3 = open(DRIVER_FILE_NAME_3, O_RDWR);	//Open interrupt driver 3
          if (file_desc_3 < 0) {
               fprintf(stderr,"Can't open driver file: %s\n", DRIVER_FILE_NAME_3);
               exit(-1);
          } else {
               (debug_flag) && (fprintf(stderr,"Driver successfully opened: %s\n", DRIVER_FILE_NAME_3));
          }
          file_desc_4 = open(DRIVER_FILE_NAME_4, O_RDWR);	//Open interrupt driver 4
          if (file_desc_4 < 0) {
               fprintf(stderr,"Can't open driver file: %s\n", DRIVER_FILE_NAME_4);
               exit(-1);
          } else {
               (debug_flag) && (fprintf(stderr,"Driver successfully opened: %s\n", DRIVER_FILE_NAME_4));
          }    
     }

     grid_height = chip_height / grid_rows;
	grid_width = chip_width / grid_cols;
	Cap = FACTOR_CHIP * SPEC_HEAT_SI * t_chip * grid_width * grid_height;
	Rx = grid_width / (2.0 * K_SI * t_chip * grid_height);
	Ry = grid_height / (2.0 * K_SI * t_chip * grid_width);
	Rz = t_chip / (K_SI * grid_height * grid_width);
	max_slope = MAX_PD / (FACTOR_CHIP * t_chip * SPEC_HEAT_SI);
	step = PRECISION / max_slope;
     frame_width = grid_rows;
     
     cout << "HOTSPOT Simulation: "<< grid_rows << ", " << p.numcpus << ", " << p.numgpus << endl;
     
   	
	// Perform the computation    
	
	     int iters = atoi(argv[10]);
     //hs->startTimeAndEnergy();
     hs->startTimeAndEnergy(true, 5, 0);
     //hs->startTimeAndEnergy(true, 2, 0);
    
	for (int step = 0; step < iters; step++){
          (debug_flag) && (fprintf(stderr,"Iteration: %d\n", step));
		hs->heterogeneous_parallel_for(0, grid_rows, &body);
	}
	
	 
  //   hs->startTimeAndEnergy();
     
  //   hs->heterogeneous_parallel_for(0, grid_rows, &body, atoi(argv[10]));
     
     hs->endTimeAndEnergy();

     hs->saveResultsForBench();

     if (ioctl_flag > 0){
	     //Close interrupt drivers
	     (debug_flag) && (fprintf(stderr,"Closing driver: %s\n", DRIVER_FILE_NAME_1));
	     close(file_desc_1);
	     (debug_flag) && (fprintf(stderr,"Closing driver: %s\n", DRIVER_FILE_NAME_2));
	     close(file_desc_2);
	     (debug_flag) && (fprintf(stderr,"Closing driver: %s\n", DRIVER_FILE_NAME_3));
	     close(file_desc_3);
	     (debug_flag) && (fprintf(stderr,"Closing driver: %s\n", DRIVER_FILE_NAME_4));
	     close(file_desc_4);
     }
     
    #ifdef MULTIHAP
          printf("Final FPGA Chunk: %d\n", finalGPUchunk/p.numgpus);
     #endif
     printf("Total workload : %lu\n", bodies_F+bodies_C);
     printf("Total workload on CPU: %lu\n", bodies_C);
     printf("Total workload on FPGA: %lu\n", bodies_F);
     printf("Percentage of work done by the FPGA: %lu \n",bodies_F*100/(bodies_F+bodies_C));
     
     // Write final output to output file
	if (argv[13]) {
          writeoutput(array_out, (grid_rows+2), (grid_cols+2), argv[13]);
     }
     
     freemem();

	return 0;
}
