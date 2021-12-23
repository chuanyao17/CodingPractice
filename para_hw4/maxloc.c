#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <malloc.h>
#include <stdlib.h>

typedef struct{
    int val;
    int rank;
}DATA;
int main(int argc, char ** argv){
    int rank,size,i;
    DATA rbuf[3],sbuf[3];
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
    srandom(time(NULL)+rank);
    printf("Process %d :",rank);
    for(i = 0;i< 3;i++){
        sbuf[i].val = random()%100;
        sbuf[i].rank = rank;
        printf(" %d",sbuf[i].val);
    }
    putchar('\n');
    MPI_Reduce(sbuf,rbuf,3,MPI_2INT,MPI_MAXLOC,0,comm);
    if(rank == 0){
        printf("Max value and location are :\n");
        for(i = 0;i< 3;i++)
            printf("value = %d, location = %d\n", rbuf[i].val, rbuf[i].rank);
        putchar('\n');
    }
    MPI_Finalize();
    return 0;
}