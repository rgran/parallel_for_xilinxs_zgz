#include <cstdlib>
#include <chrono>
#include <iostream>
#include <fstream>
#include <cstring>

#include "armpl.h"

using DTYPE = float;

namespace {

int numRows;
int numColumns;
int numNonZeroes;
int nItems;
float * h_val;
int * h_cols;
int * h_rowDelimiters;
float * h_vec;
float * h_out;
float * h_vec_trans;
float * h_out_trans;

static const float MAX_RANDOM_VAL = 10.0f;

struct Coordinate {
    int x;
    int y;
    float val;
};

inline int coordcmp(const void *v1, const void *v2)
{
    struct Coordinate *c1 = (struct Coordinate *) v1;
    struct Coordinate *c2 = (struct Coordinate *) v2;

    if (c1->x != c2->x)
    {
        return (c1->x - c2->x);
    }
    else
    {
        return (c1->y - c2->y);
    }
}

template <typename DTYPE>
void readMatrix(char *filename, DTYPE **val_ptr, int **cols_ptr,
                int **rowDelimiters_ptr, int *n, int *size, int *columns)
{

    // length of array for reading fields of mtx header
    static const int FIELD_LENGTH = 128;

    std::string line;
    char id[FIELD_LENGTH];
    char object[FIELD_LENGTH];
    char format[FIELD_LENGTH];
    char field[FIELD_LENGTH];
    char symmetry[FIELD_LENGTH];

    std::ifstream mfs( filename );
    if( !mfs.good() )
    {
        std::cerr << "Error: unable to open matrix file " << filename << std::endl;
        exit( 1 );
    }

    int symmetric = 0;
    int pattern = 0;

    int nRows, nCols, nElements;

    struct Coordinate *coords;

    std::cerr << "Reading header" << std::endl;

    // read matrix header
    if( getline( mfs, line ).eof() )
    {
        std::cerr << "Error: file " << filename << " does not store a matrix" << std::endl;
        exit( 1 );
    }

    sscanf(line.c_str(), "%s %s %s %s %s", id, object, format, field, symmetry);

    if (strcmp(object, "matrix") != 0)
    {
        fprintf(stderr, "Error: file %s does not store a matrix\n", filename);
        exit(1);
    }

    if (strcmp(format, "coordinate") != 0)
    {
        fprintf(stderr, "Error: matrix representation is dense\n");
        exit(1);
    }

    if (strcmp(field, "pattern") == 0)
    {
        pattern = 1;
    }

    if (strcmp(symmetry, "symmetric") == 0)
    {
        symmetric = 1;
    }

    while (!getline( mfs, line ).eof() )
    {
        if (line[0] != '%')
        {
            break;
        }
    }

    std::cerr << "Reading data" << std::endl;

    // read the matrix size and number of non-zero elements
    sscanf(line.c_str(), "%d %d %d", &nRows, &nCols, &nElements);
	*columns=nCols;

    int valSize = nElements;// * sizeof(struct Coordinate);
    if (symmetric)
    {
        valSize*=2;
    }
    std::cerr << "Setting number of elements: "<< valSize << std::endl;
    coords = new Coordinate[valSize];

    std::cerr << "Reading numbers from file" << std::endl;
    int index = 0;
    while (!getline( mfs, line ).eof() )
    {
		//cerr << index << " ";
        if (pattern)
        {
            sscanf(line.c_str(), "%d %d", &coords[index].x, &coords[index].y);
            // assign a random value
            coords[index].val = ((DTYPE) MAX_RANDOM_VAL *
                                 (rand() / (RAND_MAX + 1.0)));
        }
        else
        {
            // read the value from file
            sscanf(line.c_str(), "%d %d %f", &coords[index].x, &coords[index].y,
                   &coords[index].val);
        }

        // convert into index-0-as-start representation
        coords[index].x--;
        coords[index].y--;
        index++;

        // add the mirror element if not on main diagonal
        if (symmetric && coords[index-1].x != coords[index-1].y)
        {
            coords[index].x = coords[index-1].y;
            coords[index].y = coords[index-1].x;
            coords[index].val = coords[index-1].val;
            index++;
        }
    }

    nElements = index;
    // sort the elements
    qsort(coords, nElements, sizeof(struct Coordinate), coordcmp);

    std::cerr << "Allocating buffers for host data" << std::endl;

    // create CSR data structures
    *n = nElements;
    *size = nRows;

    // FIXME chapuza
    *val_ptr =  (DTYPE *)malloc(nElements * sizeof(DTYPE));
    *cols_ptr =(int *)malloc(nElements * sizeof(int));
    *rowDelimiters_ptr = (int *)malloc((nRows+1) * sizeof(int));

    printf("nRows->%d nElements->%d\n",nRows,nElements);

    DTYPE *val = *val_ptr;
    int *cols = *cols_ptr;
    int *rowDelimiters = *rowDelimiters_ptr;

    rowDelimiters[0] = 0;
    rowDelimiters[nRows] = nElements;
    int r=0;
    for (int i=0; i<nElements; i++)
    {
        while (coords[i].x != r)
        {
            rowDelimiters[++r] = i;
        }
        val[i] = coords[i].val;
        cols[i] = coords[i].y;

    }

    r = 0;

    delete[] coords;
}
} // namespace anonymous


int main(int argc, char *argv[])
{
    if(argc != 2) {
        std::cerr << "Invalid syntax" << std::endl;
        exit(1);
    }

    readMatrix(argv[1], &h_val, &h_cols, &h_rowDelimiters, &nItems, &numRows, &numColumns);

    armpl_spmat_t armpl_mat;
    int creation_flags = 0;

    auto start_create = std::chrono::steady_clock::now();
    armpl_status_t info = armpl_spmat_create_csr_s(&armpl_mat, numRows, numColumns, h_rowDelimiters, h_cols, h_val, creation_flags);
    auto stop_create = std::chrono::steady_clock::now();
    double ex_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop_create-start_create).count();
    std::cout << "Time create csr: " << ex_time << "ms" << std::endl;

    auto start = std::chrono::steady_clock::now();
    /* 3a. Supply any pertinent information that is known about the matrix */
    info = armpl_spmat_hint(armpl_mat, ARMPL_SPARSE_HINT_STRUCTURE, ARMPL_SPARSE_STRUCTURE_UNSTRUCTURED);
    if (info!=ARMPL_STATUS_SUCCESS) printf("ERROR: armpl_spmat_hint returned %d\n", info);

    /* 3b. Supply any hints that are about the SpMV calculations to be performed */
    info = armpl_spmat_hint(armpl_mat, ARMPL_SPARSE_HINT_SPMV_OPERATION, ARMPL_SPARSE_OPERATION_NOTRANS);
    if (info!=ARMPL_STATUS_SUCCESS) printf("ERROR: armpl_spmat_hint returned %d\n", info);

    info = armpl_spmat_hint(armpl_mat, ARMPL_SPARSE_HINT_SPMV_INVOCATIONS, ARMPL_SPARSE_INVOCATIONS_SINGLE);
    if (info!=ARMPL_STATUS_SUCCESS) printf("ERROR: armpl_spmat_hint returned %d\n", info);

    /* 4. Call an optimization process that will learn from the hints you have previously supplied */
    info = armpl_spmv_optimize(armpl_mat);
    if (info!=ARMPL_STATUS_SUCCESS) printf("ERROR: armpl_spmv_optimize returned %d\n", info);

    auto stop = std::chrono::steady_clock::now();
    ex_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count();
    std::cout << "Time optimize: " << ex_time << "ms" << std::endl;

    const double alpha = 1.0, beta = 0.0;

    // sparse times non-sparse matrix multiplication
    // initialize variables

    // for width
    const int x_width = 100;
    DTYPE* h_vec = new DTYPE[numRows * x_width];
    DTYPE* h_out = new DTYPE[numRows * x_width];

    for (int row=0;row<numRows;row++)
        for (int xw=0;xw<x_width;xw++)
        {
            *(h_vec+row*x_width+xw)=static_cast<DTYPE>(row);
            *(h_out+row*x_width+xw)=DTYPE(0);
        }

    start = std::chrono::steady_clock::now();
    for(int xw = 0; xw < x_width; ++xw) {
        info = armpl_spmv_exec_s(ARMPL_SPARSE_OPERATION_NOTRANS, alpha, armpl_mat, h_vec+xw*numRows, beta, h_out+xw*numRows);
        if (info!=ARMPL_STATUS_SUCCESS) { printf("ERROR: armpl_spmv_exec_s returned %d\n", info); }
        stop = std::chrono::steady_clock::now();
    }
    ex_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count();
    std::cout << "Time spmv (all columns): " << ex_time << "ms" << std::endl;

    for (int i = 0; i < 5; ++i) {
        std::cout << h_out[i] << std::endl;
    }

delete[] h_vec;
delete[] h_out;
}
