#include <stdio.h>
#include <stdlib.h>
#include "kernelspmv.h"

void cpuspmv(
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
long i, j, ckey;
	DTYPE tmp;
		for (i=begin; i<end; i++ ){
			tmp=0;
			for (ckey=rowPtr[i]; ckey<rowPtr[i+1]; ckey++) {
				j = columnIndex[ckey];
			   tmp +=  values[ckey]*x[j];
			}
			y[i]=tmp;
		}
}

void kernelspmv1(
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
	long i, j, ckey;
	DTYPE tmp;
		for (i=begin; i<end; i++ ){
			tmp=0;
			for (ckey=rowPtr[i]; ckey<rowPtr[i+1]; ckey++) {
				j = columnIndex[ckey];
			   tmp +=  values[ckey]*x[j];
			}
			y[i]=tmp;
		}
}

void kernelspmv2(
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
	long i, j, ckey;
	DTYPE tmp;
		for (i=begin; i<end; i++ ){
			tmp=0;
			for (ckey=rowPtr[i]; ckey<rowPtr[i+1]; ckey++) {
				j = columnIndex[ckey];
			   tmp +=  values[ckey]*x[j];
			}
			y[i]=tmp;
		}
}


void kernelspmv3(
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
    
    long i, j, ckey;
	DTYPE tmp;
		for (i=begin; i<end; i++ ){
			tmp=0;
			for (ckey=rowPtr[i]; ckey<rowPtr[i+1]; ckey++) {
				j = columnIndex[ckey];
			   tmp +=  values[ckey]*x[j];
			}
			y[i]=tmp;
		}
}

void kernelspmv4(
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

long i, j, ckey;
	DTYPE tmp;
		for (i=begin; i<end; i++ ){
			tmp=0;
			for (ckey=rowPtr[i]; ckey<rowPtr[i+1]; ckey++) {
				j = columnIndex[ckey];
			   tmp +=  values[ckey]*x[j];
			}
			y[i]=tmp;
		}
}



