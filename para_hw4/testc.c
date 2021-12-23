/******************************************************************************
Title : hwk3_chuanyao.lin53.c
Author : Chuanyao Lin
Created on : Oct 27, 2021
Description : Write a MPI program, when given two command line arguments,
the first of which is a string called the key, and the second of which is the
path name of a text file, searched for all occurrences of the key in the text file.
Purpose : Practice writing the sequential program as a MPI program and running the program parallelly.
Usage : hwk3_chuanyaolin53 <a string of the key> <path of the text file>
Build with : mpicc -o hwk4_chuanyaolin53 hwk4_chuanyao.lin53.c
Modifications: Oct 27, 2021
******************************************************************************/
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mpi.h"
#include <fcntl.h>


#define ROOT 0

typedef struct{
    int val;
    int rank;
}FlagsInfo;



int choose_exit(int enter_dir, double *[4] d);

int main (int argc, char* argv[])
{       
    int id;             /* rank of executing process   */
    int p;              /* number of processes         */
    double elapsed_time;  /* Time to find, count solutions */
    
    FILE * infile; /* target text file */
    int            i,j;
    double f[4];
    double d[4][4];
    double sum_check;
    int flag=1;
    double x;
    
    FlagsInfo *local_flags;
    FlagsInfo *global_flags;
    

    MPI_Status status ;
    
    MPI_Init(&argc, &argv);
    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    /* Start timer */
    elapsed_time = - MPI_Wtime();
    
    
    
    /* The root process read the both key and file size */
    if(ROOT == id)
    {
        
        /* Check the usage of the command line */
        if(2>argc) 
        {   
            printf("Usage: %s filename\n", argv[0]);
            exit(1);
        }
        
        infile = fopen(argv[1], "r");
        if ( NULL == infile )
        {
            fprintf(stderr, "Failed to open %s\n", argv[1] );
            exit(1);
        }
        
        /* Read the data from the file into the matrix */
        for ( i = 0; i < 4; i++ )
        {
            if(fscanf(infile, " %lf ", &f[i])!=1 || f[i]<=0)
            {
                printf("The input file should be positive fixed decimal values.\n");
                exit(1);
            }
                
        }
        
        for ( i = 0; i < 4; i++ ) 
        {
            sum_check=0;
            for ( j = 0; j < 4; j++ ) 
            {
                if(fscanf(infile, " %lf ", &d[i][j])!=1 || d[i][j]<=0)
                {
                    printf("The input file should be positive fixed decimal values.\n");
                    exit(1);
                }
                sum_check+=d[i][j];
                d[i][j]=sum_check;
            }
            if(sum_check!=1)
            {
                printf("Rows of the 4x4 matrix must add to 1.0.\n");
                exit(1);
            }
        }
        printf("f_array \n");
        for ( i = 0; i < 4; i++ )
        {
            printf(" %lf ", f[i]);
        }
        printf("\n");
        for ( i = 0; i < 4; i++ ) 
        {
            for ( j = 0; j < 4; j++ ) 
            {
                printf(" %lf ", d[i][j]);
            }
            printf("\n");
        }
    }
    
    srandom(id+1);    
    
    local_flags=malloc(p*sizeof(FlagsInfo));
    global_flags=malloc(p*sizeof(FlagsInfo));
    printf("Process %d :",id);
    for(i=0;i<p;i++)
    {
        local_flags[i].val = 0;
        local_flags[i].rank = id;
        printf(" %d",local_flags[i].val);
        
    }
    printf("\n Ready to go!\n");
    
    //MPI_Barrier (MPI_COMM_WORLD);
    
    /* Stop timer */
    /*elapsed_time += MPI_Wtime();
    if (ROOT == id) {
        printf ("Execution time %8.6f\n", elapsed_time);
    
    }*/
    
    
    /*int stop=1;
    int count=0;
    int id_pos=0;
    while(stop)
    { 
        printf("Process %d run %d,  stop=%d \n",id,count, stop);
        sleep(id+1);
        if(count==4 && id==1)
        {
            local_flags[id].val=1;
        }
        if(count>0 && count%2==0)
        {
            printf("Process %d check!!\n",id);
            MPI_Reduce(local_flags,global_flags,p,MPI_2INT,MPI_MAXLOC,0,MPI_COMM_WORLD);
            for(i=0;i<p;i++)
            {
                printf("Process %d check!! id=%d value=%d \n",id, i, global_flags[i].val);
                if(global_flags[i].val==1)
                {
                    stop=0;
                    id_pos=i;
                    
                    break;
                } 
            }
            MPI_Bcast(&stop, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&id_pos, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
        count++;
    }
  
    printf("Process %d stop!! value = %d, location = %d  id_pos=%d\n",id, global_flags[id_pos].val, global_flags[id_pos].rank,id_pos);*/
    int circle[16]; 
    int new_circle[16];
    
    int offset[4]={0,4,8,12};
    int arrival[4];
    int wait_cnt[4];
    int arrival_cnt[4];
    int queue[4];
    int queue_accum[4];
    
    /*initialize*/
    for(i=0;i<16;i++)
    {
        circle[i]=-1;
    }
    for(i=0;i<4;i++)
    {
        arrival_cnt[i]=0;
        wait_cnt[i]=0;
        queue[i]=0;
        queue_accum[i]=0;
    }
    
    int res;
    res=choose_exit(0, d);
    MPI_Finalize(); 
    return 0;
}

int choose_exit(int enter_dir, double*[4] d)
{
    double random_num=random()/RAND_MAX;
    for(int i=0;i<4;i++)
    {
        if(d[enter_dir][i]>=random_num)
        {
            return i;
        }
    }
}