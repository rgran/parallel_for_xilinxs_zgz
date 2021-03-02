//============================================================================
// Name			: Body.h
// Author		: Antonio Vilches
// Version		: 1.0
// Date			: 13 / 01 / 2015
// Copyright	: Department. Computer's Architecture (c)
// Description	: This file contains the Body class
//============================================================================

#ifndef _BODY_TASK_
#define _BODY_TASK_

#ifdef USEBARRIER
#include "barrier.h"
#endif

#include "tbb/parallel_for.h"
#include "tbb/task.h"
#include "tbb/tick_count.h"
using namespace tbb;


#include <stdio.h>
#include <sds_lib.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <linux/fs.h>


/*Interrupt drivers*/
#define DRIVER_FILE_NAME_1 "/dev/intgendriver1"
int file_desc_1 = open(DRIVER_FILE_NAME_1, O_RDWR);	/*Open interrupt driver 1*/
#define DRIVER_FILE_NAME_2 "/dev/intgendriver2"
int file_desc_2 = open(DRIVER_FILE_NAME_2, O_RDWR);	/*Open interrupt driver 2*/
#define DRIVER_FILE_NAME_3 "/dev/intgendriver3"
int file_desc_3 = open(DRIVER_FILE_NAME_3, O_RDWR);	/*Open interrupt driver 3*/
#define DRIVER_FILE_NAME_4 "/dev/intgendriver4"
int file_desc_4 = open(DRIVER_FILE_NAME_4, O_RDWR);	/*Open interrupt driver 4*/

/*****************************************************************************
 * NbodyTask
 * **************************************************************************/
class Body
{
public:
	bool firsttime;
public:

	void OperatorGPU(int begin, int end, int id) {
		//cerr << "GPU: " << begin << " " << end << " id: " << id << endl;
		
		/*int file_desc;
		//file_desc = open("/dev/my_driver",O_RDWR);
		file_desc = open(DRIVER_FILE_NAME, 0);
		if (file_desc < 0) {
			printf("Can't open driver file: %s\n", DRIVER_FILE_NAME);
			exit(-1);
		} else {
			printf("Opening driver: %s\n", DRIVER_FILE_NAME);
		}*/
		
		
		bodies_F+=end-begin;
		switch(id)
		{
			case 1 : kernelspmv1(h_rowDelimiters, h_cols, h_val, h_out, h_vec, numRows,nItems,file_desc_1,begin,end); break;
			case 2 : kernelspmv2(h_rowDelimiters, h_cols, h_val, h_out, h_vec, numRows,nItems,file_desc_2,begin,end); break;
			case 3 : kernelspmv3(h_rowDelimiters, h_cols, h_val, h_out, h_vec, numRows,nItems,file_desc_3,begin,end); break;
			case 4 : kernelspmv4(h_rowDelimiters, h_cols, h_val, h_out, h_vec, numRows,nItems,file_desc_4,begin,end); break;
		}
		
		/*close(file_desc);*/
	}

	void OperatorCPU(int begin, int end) {
		//cerr << "CPU: " << begin << " " << end  << endl; 
		bodies_C+=end-begin;
		spmvCpu/*_vilches*/(h_val, h_cols, h_rowDelimiters, h_vec, h_out, begin, end);
	}

};
//end class

#endif
