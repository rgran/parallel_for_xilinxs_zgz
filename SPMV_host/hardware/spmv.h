#ifndef __SPMV_H__
#define __SPMV_H__


#include <stdio.h>


const static int S = 11; //how many operations are scheduled in the L2_1 loop. More operations uses more DSPs but increases latency. 
// Latency is a real problem if the row contains only a few elements different from zero and the latency must be paid when a new row starts.
// This problem is addressed somehow by the value R
// It seems that the ideal value is 5 that matches the II of the L2_1 loop so a single DSP unit is always busy. 
const static int R = 16; //how many rows are processed in one main loop iteration.
//#define PRELOAD; //load all the data in one go
const static int max_nnz = 10000; //max nnz values that can be loaded in internal buffer. Internal buffer must have enough capacity to store the nnz values that are in the R lines

typedef float DATA_TYPE;
typedef float DTYPE;
void spmv_simple(int *rowPtr, int *columnIndex,
		  DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);

void spmv_simple2(int *rowPtr, int *columnIndex,
		  DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);

void spmv_simple3(int *rowPtr, int *columnIndex,
		  DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);

void spmv_simple4(int *rowPtr, int *columnIndex,
		  DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);

void spmv_simple5(int *rowPtr, int *columnIndex,
		  DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);

void spmv_simple6(int *rowPtr, int *columnIndex,
		  DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);

void spmv_simple7(int *rowPtr, int *columnIndex,
		  DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int line_count, int nnz, int & error_condition);


void spmv_restructure(int *rowPtr, int *columnIndex,
	DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);
void spmv_interleave(int *rowPtr, int *columnIndex,
	DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);
void spmv_interleave2(int *rowPtr, int *columnIndex,
	DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);
void spmv_interleave3(int *rowPtr, int *columnIndex,
	DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int nnz);

//void spmv(int *rowPtr[NUM_ROWS + 1], int columnIndex[NNZ],
//	DTYPE values[NNZ], DTYPE y[SIZE], DTYPE x[SIZE]);

#endif // __MATRIXMUL_H__ not defined
