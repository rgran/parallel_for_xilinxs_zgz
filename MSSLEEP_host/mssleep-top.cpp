// Author: Kris Nikov - kris.nikov@bris.ac.uk
// Date: 23 Jul 2019
// Description: Simple program to put the system to sleep and measure OS overhead for the ultrascale+ devboard

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include <time.h>
#include "../../energy_meter/energy_meter.h"


struct energy_sample *sample1;
struct em_t final_em;
long unsigned int bodies_C=0, bodies_F=0;

using namespace std;


/*****************************************************************************
 * Main Function
 * **************************************************************************/
int main (int argc, char * argv[]){

     if (argc != 2) {
          fprintf(stderr, "arguments: sleep time (ms)\n");
		exit(-1);
     }
     
     int sleep_ms = atoi(argv[1]);
     if (sleep_ms < 50){
          fprintf(stderr, "Please input a value larger than 50 (sensor sampling rate). Ideally it should be a multiple of 50.\n");
		exit(-1);
     }
     
     
     sample1=energy_meter_init(50, 0  /*0=no debug*/);  // sample period 50 miliseconds
     energy_meter_start(sample1);  // starts sampling thread
               
     long sleep_ns = sleep_ms*1000000;
     
     struct timespec tim;
     tim.tv_sec = 0;
     tim.tv_nsec = sleep_ns;

     nanosleep(&tim, NULL); //sleep for X ms -> use to get OS overhead
     
     energy_meter_read(sample1,&final_em);  // final & total
     energy_meter_stop(sample1);  	// stops sampling
     energy_meter_printf(sample1, stdout);  // print total results
     energy_meter_destroy(sample1);     // clean up everything
     
	printf("Total n. rows : %lu\n", bodies_F+bodies_C);
     printf("Total n. rows on CPU: %lu\n", bodies_C);
     printf("Total n. rows on FPGA: %lu\n", bodies_F);
     printf("Actual percentage of work offloaded to the FPGA: %lu \n",bodies_F*100/(bodies_F+bodies_C));
}
