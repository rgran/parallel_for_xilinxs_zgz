#include "spmv.h"


#pragma SDS data zero_copy(rowPtr[0:(row_size+1)])
#pragma SDS data zero_copy(columnIndex[0:(nnz)])
#pragma SDS data zero_copy(values[0:(nnz)])
#pragma SDS data zero_copy(y[0:(row_size)])
#pragma SDS data zero_copy(x[0:(row_size)])
void spmv_simple7(int *rowPtr, int *columnIndex,
	DTYPE *values, DTYPE *y, DTYPE *x, int row_size, int line_count, int nnz, int & error_condition)
{
	//declare internal variables to buffer data. We need the whole rowptr and the whole input X.  We try only to buffer column and values for R rows to save blockram.


	int rowPtr_int[40001];
	DTYPE x_int[40000];
	//DTYPE y_int[100];
	int columnIndex_int[max_nnz];
	DTYPE values_int[max_nnz];

	//load data internally

	Init :
	for (int i = 0; i<line_count; i++)
	{
		#pragma HLS pipeline II=1
		rowPtr_int[i] = rowPtr[i] - rowPtr[0];
	}
	rowPtr_int[line_count] = rowPtr[line_count] - rowPtr[0];

	for (int i = 0; i<row_size; i++)
	{
		#pragma HLS pipeline II=1
		x_int[i] = x[i];
	}
	
        #ifdef PRELOAD //preload all the other data in one go. Buffer must be big enough to store all non-zeros

	if (nnz > max_nnz)
	{
		//Error since we try to load more values than the possible max_nnz
		error_condition=1;
	}

	for (int i = 0; i < nnz; i++)
	{
		#pragma HLS pipeline II=1
		values_int[i] = values[i];
		columnIndex_int[i] = columnIndex[i];
	}


	#endif
	
	//start computing in blocks of R rows. This R tries to avoid the latency of changing rows. 	

	L1: for (int i = 0; i < line_count; i += R) {
		DTYPE y_int[R];
		#pragma HLS array_partition variable=y_int complete
		int RLAST; 

		//check if there are R rows left or not. 
		if ((line_count - i) > R)
			RLAST = R;
		else
			RLAST = (line_count - i);

		//init y_int
		for (int i4 = 0; i4 < RLAST; i4++)
		{
			#pragma HLS unroll complete
			y_int[i4] = 0;
		}

		int l=0;

		#ifndef PRELOAD
		//load enough data to for the R rows
		L3: for (int z=rowPtr_int[i]; z<rowPtr_int[i+RLAST]; z++){
			 #pragma HLS pipeline II=1
			 values_int[l] = values[z];
			 columnIndex_int[l] = columnIndex[z];
			 l++;
		}

		if (l > max_nnz)
		{
			//Error since we try to load more values than the possible max_nnz
			error_condition = 1;
		}
		
		#endif

		int nl = 0;
		//main processing loop S operations scheduled at a time. Scheduler tries to fit them all within the II that is 5 so S=5 seems optimal.
	   	L2_1: for (int k = rowPtr_int[i]; k < rowPtr_int[i + RLAST]; k += S)
		{
			#pragma HLS pipeline II=8 //if you try II < 5 it will just try, fail and end up with 5.
			DTYPE yt[R];
			#pragma HLS array_partition variable=yt complete
			//init yt
			for (int i3 = 0; i3 < R; i3++)
			{
				#pragma HLS unroll complete
				yt[i3] = 0;
			}
			//go over S and see at which yt the multiplication result should be added depending on which interval we are in the rowPtr.
			// rowptr stores the number of non-zeros per row incrementally: |  2  |  6   |  9 (4 non-zeros and then 3 non-zeros etc)
			// break the loop once we assigned the value to an interval. 
			
			L2_2: for (int j = 0; j < S; j++) 
			{
				bool flag_done = 0;
				for (int nr = 0; nr < R; nr++) 
				{
					if (((nl + j) < (rowPtr_int[i+nr+1] - rowPtr_int[i])) && (flag_done == 0)) 
					//if ((nl + j) < (rowPtr_int[i+nr+1] - rowPtr_int[i])) 
					{
						#ifdef PRELOAD
							yt[nr] += values_int[k + j] * x_int[columnIndex_int[k + j]];
						#else
							yt[nr] += values_int[nl + j] * x_int[columnIndex_int[nl + j]];
						#endif
						flag_done = 1;
						//break; //with break aproach fails to meet timing with R=16
					}
				}
			}
			nl += S;
			//store partial results yt
			for (int i1 = 0; i1 < R; i1++)
			{
				#pragma HLS unroll complete
				y_int[i1] += yt[i1];
			}
	}
	//write the output y to main memory
	for (int i2 = 0; i2 < RLAST; i2++)
	{
		#pragma HLS pipeline II=1
		y[i+i2] = y_int[i2];
	}
  }

}


