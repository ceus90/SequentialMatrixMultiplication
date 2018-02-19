#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <memory.h>
#include <malloc.h>

double get_cur_time();
void dgemm1 (int n, double *A, double *B, double *C);
void dgemm2 (int n, double *A, double *B, double *C);
void dgemm3 (int n, double *A, double *B, double *C);
void dgemm4 (int n, double *A, double *B, double *C);
void dgemm5 (int n, double *A, double *B, double *C);
void dgemm6 (int n, double *A, double *B, double *C);

int main(int argc, char** argv)
{
    clock_t start, end;
    double btime, etime, sum_e_time, ggflops, sum_gflops; //To record elapsed time and gflops
    int i, j, n, y, narr[19] = {10,20,30,40,50,60,70,80,90,100,200,300,400,500,600,700,800,900,1000}; //Input size of the matrices
    const char *sstr[6]; //Six loops
    sstr[0] = "IJK";
    sstr[1] = "IKJ";
    sstr[2] = "JIK";
    sstr[3] = "JKI";
    sstr[4] = "KIJ";
    sstr[5] = "KJI";
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
        exit(1);
    srand((unsigned)time(NULL));
    for(j = 0; j < 19; j++) //Executing the functions once for all the input sizes
    {
        n = narr[j];
        //Allocating the matrix memory dynamically
        double *A = malloc(n*n*sizeof(double));
        double *B = malloc(n*n*sizeof(double));
        double *C = malloc(n*n*sizeof(double));
        printf("\nExecuting 20 times for input N = %d\n",n);
        for(int x = 0; x < 6; x++)//Executing the functions for all six different versions
        {
            int r = x + 1;
            //Initializing sum to zero
            sum_e_time = 0;
            sum_gflops = 0;
            printf("\nLoop-%s information\n",sstr[x]);
            for(i = 0; i < n; i++)
            {
                //Assigning random double values to matrices of size N * N
                A[i] = rand();
                B[i] = rand();
                C[i] = rand();
            }
            switch(r)
            {
                case 1://Loop IJK
                    btime = get_cur_time();
                    for(y = 0; y < 20; y++)
                        dgemm1(n,A,B,C);//Performing matrix multipication
                    etime = get_cur_time();
                    ggflops = (2*n*n*n)/(etime-btime)/1000000000;
                    sum_e_time += (etime-btime);
                    sum_gflops += ggflops;
                    printf("Average elapsed time\t: %16.9f seconds\n",sum_e_time/20);
                    printf("Average gflops\t\t:\t%16.9f giga flops per second\n",sum_gflops/20);
                    break;
                case 2://Loop IKJ
                    btime = get_cur_time();
                    for(y = 0; y < 20; y++)
                        dgemm2(n,A,B,C);//Performing matrix multipication
                    etime = get_cur_time();
                    ggflops = (2*n*n*n)/(etime-btime)/1000000000;
                    sum_e_time += (etime-btime);
                    sum_gflops += ggflops;
                    printf("Average elapsed time\t: %16.9f seconds\n",sum_e_time/20);
                    printf("Average gflops\t\t:\t%16.9f flops per second\n",sum_gflops/20);
                    break;
                case 3://Loop JIK
                    btime = get_cur_time();
                    for(y = 0; y < 20; y++)
                        dgemm3(n,A,B,C);//Performing matrix multipication
                    etime = get_cur_time();
                    ggflops = (2*n*n*n)/(etime-btime)/1000000000;
                    sum_e_time += (etime-btime);
                    sum_gflops += ggflops;
                    printf("Average elapsed time\t: %16.9f seconds\n",sum_e_time/20);
                    printf("Average gflops\t\t:\t%16.9f flops per second\n",sum_gflops/20);
                    break;
                case 4://Loop JKI
                    btime = get_cur_time();
                    for(y = 0; y < 20; y++)
                        dgemm4(n,A,B,C);//Performing matrix multipication
                    etime = get_cur_time();
                    ggflops = (2*n*n*n)/(etime-btime)/1000000000;
                    sum_e_time += (etime-btime);
                    sum_gflops += ggflops;
                    printf("Average elapsed time\t: %16.9f seconds\n",sum_e_time/20);
                    printf("Average gflops\t\t:\t%16.9f flops per second\n",sum_gflops/20);
                    break;
                case 5://Loop KIJ
                    btime = get_cur_time();
                    for(y = 0; y < 20; y++)
                        dgemm5(n,A,B,C);//Performing matrix multipication
                    etime = get_cur_time();
                    ggflops = (2*n*n*n)/(etime-btime)/1000000000;
                    sum_e_time += (etime-btime);
                    sum_gflops += ggflops;
                    printf("Average elapsed time\t: %16.9f seconds\n",sum_e_time/20);
                    printf("Average gflops\t\t:\t%16.9f flops per second\n",sum_gflops/20);
                    break;
                case 6://Loop KJI
                    btime = get_cur_time();
                    for(y = 0; y < 20; y++)
                        dgemm6(n,A,B,C);//Performing matrix multipication
                    etime = get_cur_time();
                    ggflops = (2*n*n*n)/(etime-btime)/1000000000;
                    sum_e_time += (etime-btime);
                    sum_gflops += ggflops;
                    printf("Average elapsed time\t: %16.9f seconds\n",sum_e_time/20);
                    printf("Average gflops\t\t:\t%16.9f flops per second\n",sum_gflops/20);
                    break;
            }
        }
    }
    return 0;
}

double get_cur_time()
{
    return PAPI_get_real_usec() / 1000000.0;
}

void dgemm1(int n, double* A, double* B, double* C)
{
    int i = 0, j = 0, k = 0;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
        {
            double cij = C[i*n+j];
            for(k = 0; k < n; k++ )
                cij += A[i*n+k] * B[k*n+j];
            C[i*n+j] = cij;
        }
}

void dgemm2(int n, double* A, double* B, double* C)
{
    int i = 0, j = 0, k = 0;
    double cij;
    for (i = 0; i < n; ++i)
        for (k = 0; k < n; ++k)
            for(j = 0; j < n; ++j)
            {
                cij = C[i*n+j];
                cij += A[i*n+k] * B[k*n+j];
                C[i*n+j] = cij;
            }
}

void dgemm3(int n, double* A, double* B, double* C)
{
    int i = 0, j = 0, k = 0;
    double cij;
    for (j = 0; j < n; j++)
    {
        for (i = 0; i < n; i++)
        {
            cij = C[n*i+j];
            for (k = 0; k < n; k++)
            {
                cij += A[n*i+k] * B[n*k+j];
                C[n*i+j] = cij;
            }
        }
    }
}

void dgemm4(int n, double* A, double* B, double* C)
{
    int i = 0, j = 0, k = 0;
    double cij;
    for (j = 0; j < n; ++j)
        for (k = 0; k < n; ++k)
            for(i = 0; i < n; ++i)
            {
                cij = C[i*n+j];
                cij += A[i*n+k] * B[k*n+j];
                C[i*n+j] = cij;
            }
}

void dgemm5(int n, double* A, double* B, double* C)
{
    int i = 0, j = 0, k = 0;
    double cij;
    for (k = 0; k < n; ++k)
        for (i = 0; i < n; ++i)
            for(j = 0; j < n; ++j)
            {
                cij = C[i*n+j];
                cij += A[i*n+k] * B[k*n+j];
                C[i*n+j] = cij;
            }
}

void dgemm6(int n, double* A, double* B, double* C)
{
    int i = 0, j = 0, k = 0;
    double cij;
    for (k = 0; k < n; ++k)
        for (j = 0; j < n; ++j)
            for(i = 0; i < n; ++i)
            {
                cij = C[i*n+j];
                cij += A[i*n+k]
                C[i*n+j] = cij;
            }
}
