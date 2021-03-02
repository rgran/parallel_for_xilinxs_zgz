#include <stdio.h>
#include <stdlib.h>
#include <sds_lib.h>
#include "spmv.h"

int kernelspmv1(
int *rowPtr, 
int *columnIndex,
DTYPE *values, 
DTYPE *y, 
DTYPE *x, 
int row_size, 
int nnz,
unsigned int begin,
unsigned int end)
{


	int line_count = end-begin;
	int nnz_int, nnz_done;
	int *rowPtr_int;
	int *columnIndex_int;
	int error_condition;

	DTYPE *values_int;
	DTYPE *y_int;
	
        rowPtr_int = rowPtr + begin;
	nnz_done = *(rowPtr + begin);
	columnIndex_int = columnIndex +nnz_done;
	values_int = values +nnz_done;
	nnz_int = *(rowPtr + end) - *(rowPtr + begin);
	y_int = y + begin;

	error_condition = 0;
	#pragma SDS resource(1)
	spmv_simple7(rowPtr_int, columnIndex_int, values_int, y_int, x, row_size,line_count, nnz_int, error_condition);
	if (error_condition)
	{
		printf("Error: too many nnz in R rows\n");
		exit(1);
	}
}

int kernelspmv2(
int *rowPtr, 
int *columnIndex,
DTYPE *values, 
DTYPE *y, 
DTYPE *x, 
int row_size, 
int nnz,
unsigned int begin,
unsigned int end)
{


	int line_count = end-begin;
	int nnz_int, nnz_done;
	int *rowPtr_int;
	int *columnIndex_int;
	int error_condition;

	DTYPE *values_int;
	DTYPE *y_int;
	
        rowPtr_int = rowPtr + begin;
	nnz_done = *(rowPtr + begin);
	columnIndex_int = columnIndex +nnz_done;
	values_int = values +nnz_done;
	nnz_int = *(rowPtr + end) - *(rowPtr + begin);
	y_int = y + begin;

	error_condition = 0;
	#pragma SDS resource(2)
	spmv_simple7(rowPtr_int, columnIndex_int, values_int, y_int, x, row_size,line_count, nnz_int, error_condition);
	if (error_condition)
	{
		printf("Error: too many nnz in R rows\n");
		exit(1);
	}
}


int kernelspmv3(
int *rowPtr, 
int *columnIndex,
DTYPE *values, 
DTYPE *y, 
DTYPE *x, 
int row_size, 
int nnz,
unsigned int begin,
unsigned int end)
{


	int line_count = end-begin;
	int nnz_int, nnz_done;
	int *rowPtr_int;
	int *columnIndex_int;
	int error_condition;

	DTYPE *values_int;
	DTYPE *y_int;
	
        rowPtr_int = rowPtr + begin;
	nnz_done = *(rowPtr + begin);
	columnIndex_int = columnIndex +nnz_done;
	values_int = values +nnz_done;
	nnz_int = *(rowPtr + end) - *(rowPtr + begin);
	y_int = y + begin;

	error_condition = 0;
	#pragma SDS resource(3)
	spmv_simple7(rowPtr_int, columnIndex_int, values_int, y_int, x, row_size,line_count, nnz_int, error_condition);
	if (error_condition)
	{
		printf("Error: too many nnz in R rows\n");
		exit(1);
	}
}

int kernelspmv4(
int *rowPtr, 
int *columnIndex,
DTYPE *values, 
DTYPE *y, 
DTYPE *x, 
int row_size, 
int nnz,
unsigned int begin,
unsigned int end)
{


	int line_count = end-begin;
	int nnz_int, nnz_done;
	int *rowPtr_int;
	int *columnIndex_int;
	int error_condition;

	DTYPE *values_int;
	DTYPE *y_int;
	
        rowPtr_int = rowPtr + begin;
	nnz_done = *(rowPtr + begin);
	columnIndex_int = columnIndex +nnz_done;
	values_int = values +nnz_done;
	nnz_int = *(rowPtr + end) - *(rowPtr + begin);
	y_int = y + begin;

	error_condition = 0;
	#pragma SDS resource(4)
	spmv_simple7(rowPtr_int, columnIndex_int, values_int, y_int, x, row_size,line_count, nnz_int, error_condition);
	if (error_condition)
	{
		printf("Error: too many nnz in R rows\n");
		exit(1);
	}
}



