#ifndef KERNELSPMV_H_
#define KERNELSPMV_H_

typedef float DTYPE;

void kernelspmv1(
int *rowPtr, 
int *columnIndex,
DTYPE *values, 
DTYPE *y, 
DTYPE *x, 
int row_size, 
int nnz,
int file_desc,
unsigned int begin,
unsigned int end);

void kernelspmv2(
int *rowPtr, 
int *columnIndex,
DTYPE *values, 
DTYPE *y, 
DTYPE *x, 
int row_size, 
int nnz,
int file_desc,
unsigned int begin,
unsigned int end);

void kernelspmv3(
int *rowPtr, 
int *columnIndex,
DTYPE *values, 
DTYPE *y, 
DTYPE *x, 
int row_size, 
int nnz,
int file_desc,
unsigned int begin,
unsigned int end);

void kernelspmv4(
int *rowPtr, 
int *columnIndex,
DTYPE *values, 
DTYPE *y, 
DTYPE *x, 
int row_size, 
int nnz,
int file_desc,
unsigned int begin,
unsigned int end);
#endif 
