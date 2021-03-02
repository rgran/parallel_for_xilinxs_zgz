//============================================================================
// Name			: main.cpp
// Author		: Antonio Vilches
// Version		: 1.0
// Date			: 13 / 01 / 2015
// Copyright	: Department. Computer's Architecture (c)
// Description	: Main file of SPMV 
//============================================================================

//#define DEBUG
//#define DEEP_CPU_REPORT
//#define DEEP_GPU_REPORT
//#define DYNAMIC
//#define ORACLE
//#define FIXEDCHUNK
//#define CONCORD
//#define HDSSS
//#define LOGFIT
//#define PJTRACER
//#define OVERHEAD_STUDY
#define MULTIDYNAMIC

#include "kernelspmv.h"

#define NDEBUG

#ifndef ITERS
#define ITERS 1
#endif


long unsigned int bodies_C=0, bodies_F=0;

// compile in broadwell1 using:  
//            make -f Makefile.andres CFLAGS=-DMALAGA
//#define MALAGA  

#ifdef MALAGA
#define  sds_alloc_non_cacheable malloc
#else
#include <sds_lib.h> 
#endif

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "SPMV.h"

#ifdef ORACLE
#include "Oracle.h"
#endif
#ifdef DYNAMIC
#include "Dynamic.h"
#endif
#ifdef FIXEDCHUNK
#include "FixedChunkSize.h"
#endif
#ifdef CONCORD
#include "Concord.h"
#endif
#ifdef HDSSS
#include "HDSS.h"
#endif
#ifdef LOGFIT
#include "LogFit.h"
#endif
#ifdef MULTIDYNAMIC
#include "MultiDynamic.h"
#endif
#include "Body.h"

using namespace std;
using namespace tbb;

void matrixvector(DTYPE *A, DTYPE *y, DTYPE *x, int size)
{
	for (int i = 0; i < size; i++) {
		DTYPE y0 = 0;
		for (int j = 0; j < size; j++)
		{
			y0 += *(A + i*size + j) * x[j];
			//printf("result is %f and input mat %f and input vector %f\n", y0, *(A + i*size + j),x[j]);
		}
		y[i] = y0;
	}
}


/*****************************************************************************
 * Main Function
 * **************************************************************************/
int main (int argc, char * argv[]){
    int fail=0;
    int row_size, col_size,data_size;
    int check=0;
    DTYPE *M;
	DTYPE *y_sw;
	DTYPE *x_sw;	
	//variables
	Body body;
	Params p;
	//end variables

	if (argc < 5) {
		fprintf(stderr, "arguments: inFile numcpus numgpus [ratio|chunkGPU] \n");
		exit(-1);
	}
	
	if(argc==6) check=1;

	p.numcpus			= atoi(argv[2]);
	p.numgpus			= atoi(argv[3]);
	
	sprintf(p.benchName, "SPMV");
	sprintf(p.kernelName, "spmv_csr_scalar_kernel");

/*Initializing scheduler*/
#ifdef ORACLE
	p.ratioG = atof(argv[4]);
	Oracle * hs = Oracle::getInstance(&p);
#endif
#ifdef DYNAMIC
	p.gpuChunk = atoi(argv[4]);
	Dynamic * hs = Dynamic::getInstance(&p);
#endif
#ifdef MULTIDYNAMIC
	p.gpuChunk = atoi(argv[4]);
	Dynamic * hs = Dynamic::getInstance(&p);
#endif
#ifdef FIXEDCHUNK
	p.cpuChunk = atoi(argv[4]);
	p.gpuChunk = atoi(argv[5]);
	FixedChunkSize * hs = FixedChunkSize::getInstance(&p);
#endif
#ifdef CONCORD
	Concord * hs = Concord::getInstance(&p);
#endif
#ifdef HDSSS
	HDSS * hs = HDSS::getInstance(&p);
#endif
#ifdef LOGFIT
	LogFit * hs = LogFit::getInstance(&p);
#endif
	
	cerr << "SPMV Simulation: "<< argv[1] << ", " << p.numcpus << ", " << p.numgpus << endl;

	cerr << "Reading Matrix Input File" << endl;
	char filename[FIELD_LENGTH];
        readMatrix(argv[1], &h_val, &h_cols, &h_rowDelimiters, &nItems, &numRows, &numColumns);
	
	h_vec = (DTYPE *)sds_alloc_non_cacheable(numRows * sizeof(DTYPE));
	h_out = (DTYPE *)sds_alloc_non_cacheable(numRows * sizeof(DTYPE));
	
	  for (int row=0;row<numRows;row++)
	{
	h_vec[row] = row;
	}

	cerr << "Number of Rows: " << numRows << endl; 
	numNonZeroes = nItems;
	cerr << "NNZ: " << numNonZeroes << endl;
	
	if(check)
	{
		FILE *fp_mat;
	char line[1000];
	

  fp_mat = fopen(argv[5], "r");
	fgets(line, sizeof line, fp_mat); // read a line from a file dense
	sscanf(line, "%d %d %d", &row_size, &col_size, &data_size);
	DTYPE v;

	M = (DTYPE *)malloc((col_size*row_size)* sizeof(DTYPE));
	y_sw = (DTYPE *)malloc(row_size * sizeof(DTYPE));
	x_sw = (DTYPE *)malloc(row_size * sizeof(DTYPE));


	for (int row_index = 0; row_index < row_size; row_index++)
	{
		for (int col_index = 0; col_index < col_size; col_index++)
		{
			if (fgets(line, sizeof(line), fp_mat) == NULL)
			{
				printf("error reading dense mat\n");
				exit(1);// read a line from a file
			}
			sscanf(line, "%f ", &v);
		//	printf("read value %f\n", v);
			*(M + row_size*row_index + col_index) = v;
		//	printf("%d,%d = %f\n",row_index, col_index, v);
		}
	}
	
	printf("Done reading dense mat\n");

//init vector
     for (int row=0;row<row_size;row++)
	{
	x_sw[row] = h_vec[row];
	// printf("%d = %f\n",row, x_sw[row]);
	}

 }
	
	
	iters = ITERS;
	
     //Interrupt device drivers defined in Body.h
     
	if (file_desc_1 < 0) {
		printf("Can't open driver file: %s\n", DRIVER_FILE_NAME_1);
		exit(-1);
	} else {
		printf("Driver successfully opened: %s\n", DRIVER_FILE_NAME_1);
	};
     if (file_desc_2 < 0) {
		printf("Can't open driver file: %s\n", DRIVER_FILE_NAME_2);
		exit(-1);
	} else {
		printf("Driver successfully opened: %s\n", DRIVER_FILE_NAME_2);
	};
     if (file_desc_3 < 0) {
		printf("Can't open driver file: %s\n", DRIVER_FILE_NAME_3);
		exit(-1);
	} else {
		printf("Driver successfully opened: %s\n", DRIVER_FILE_NAME_3);
	};
     if (file_desc_4 < 0) {
		printf("Can't open driver file: %s\n", DRIVER_FILE_NAME_4);
		exit(-1);
	} else {
		printf("Driver successfully opened: %s\n", DRIVER_FILE_NAME_4);
	};
	
	// Begin iterations
	hs->startTimeAndEnergy();
	//iters = 200;
	for (int step = 0; step < iters; step++){
		//cerr << "Iteration: " << step << endl;
		hs->heterogeneous_parallel_for(0, numRows, &body);
		
	}
	hs->endTimeAndEnergy();
	hs->saveResultsForBench();
	
	//Close interrupt drivers
	printf("Closing driver: %s\n", DRIVER_FILE_NAME_1);
	close(file_desc_1);
     printf("Closing driver: %s\n", DRIVER_FILE_NAME_2);
	close(file_desc_2);
     printf("Closing driver: %s\n", DRIVER_FILE_NAME_3);
	close(file_desc_3);
     printf("Closing driver: %s\n", DRIVER_FILE_NAME_4);
	close(file_desc_4);
	
	
   printf("Total n. rows : %d\n", bodies_F+bodies_C);
   printf("Total n. rows on CPU: %d\n", bodies_C);
   printf("Total n. rows on FPGA: %d\n", bodies_F);
   printf("Actual percentage of work offloaded to the FPGA:%d%% \n",bodies_F*100/(bodies_F+bodies_C));
if(check)
{
	matrixvector(M, y_sw, x_sw,row_size);
	
	for(int i = 0; i < row_size; i++)
	if (round(y_sw[i]) != round(h_out[i]))
	{
		fail = 1;
		printf("mat result %f and sparse result %f at %d\n", y_sw[i], h_out[i],i);
		//exit(1);
	}
	if(fail == 1)
		printf("FAILED\n");
	else
		printf("PASS\n");
}
	return fail;
}
