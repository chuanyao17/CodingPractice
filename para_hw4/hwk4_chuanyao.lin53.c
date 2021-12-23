/******************************************************************************
Title : hwk4_chuanyao.lin53.c
Author : Chuanyao Lin
Created on : Nov 25, 2021
Description : Write a MPI program to simulate the traffic circle. The goal is to
determine some steady state properties of this traffice circle. The order of both input
and output will be N,W,S,E.
Usage : hwk4_chuanyaolin53 <path of the text file>
Build with : mpicc -o hwk4_chuanyaolin53 hwk4_chuanyao.lin53.c
Modifications: Nov 29, 2021
******************************************************************************/
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mpi.h"
#include <fcntl.h>
#include <limits.h>
#include <math.h>

#define ROOT 0


typedef struct{
    int val;
    int rank;
}FlagsInfo;
/**This struct is to store the flags of each processes.
*  Once one of the processes has reached the steady state,
*  its val will be set to 1. rank indicates the id of the process.
*/

int choose_exit(int* enter_dir, int* offset, float d[][4]);
/**choose_exit()
*  Once the car entered the circle, compute its exit. 
*/
void reset_queue_max_min(double queue_avg_max[],double queue_avg_min[]);
/**reset_queue_max_min()
*  Rest the maxium and minium average queue lenth. 
*/
int check_steady_state(double queue_avg_max[],double queue_avg_min[]);
/**check_steady_state()
*  If the difference of maxium avg queue length and minimum avg queue length
   is at most 0.01 at all entrances, return 1 to indicate the simulation has reached the steady
   state, return 0 means not steady state yet. 
*/


int main (int argc, char* argv[])
{       
    int id;             /* rank of executing process   */
    int p;              /* number of processes         */
    
    FILE * infile; /* target text file */
    int   i,j;     
    float f[4];    /* The mean time between arrivals at each of the four entrances(N,W,S,E) */
    float d[4][4]; /* The probability that a verhicle entering (N,W,S,E) at entrace i will leave at exit j */
    float sum_check; 
    int id_pos;    /* Store the id of the process which finished the simulation first */
    
    int circle[16]; /*Circle represents the traffic circle with 16 segments, each segments contain -1 means empty or holds one vehicle.*/
    int new_circle[16]; /* Store the status of each segements after moving 1 iteration.*/
    
    int offset[4]={0,4,8,12}; /* Represents the exits. 0 for N, 4 for W, 8 for S, 12 for E.*/
    int arrival[4]; /*Boolean for 4 entrances, when the vehicles arrived, set to 1.*/
    int wait_cnt[4]; /* Count the number of cars which can't enter the circle immediately of each 4 entrances. */
    int arrival_cnt[4]; /* Count the number of cars which has arrived of each 4 entrances. */
    int queue[4]; /* Track the queue length of each 4 entrances. */
    int queue_accum[4]; /* Track the total, over all time steps of the simulation, of the values in queue. */
    double queue_avg[4]; /* Store the average of the queue length. */
    double queue_avg_max[4]; /* Store the maxium of the average queue length of the 100 iterations. */
    double queue_avg_min[4]; /* Store the miniium of the average queue length of the 100 iterations. */
    double queue_avg_diff[4]; /* Store the difference of maxium avg queue length and minimum avg queue length. */
    
    FlagsInfo *local_flags; /*Store the flags of each processe */
    FlagsInfo *global_flags;
    

    MPI_Status status ;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    
    /* The root process reads the file */
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
        /* Check all the inputs are positive fixed decimal values*/
        for ( i = 0; i < 4; i++ )
        {
            if(fscanf(infile, " %f ", &f[i])!=1 || f[i]<=0)
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
                if(fscanf(infile, " %f ", &d[i][j])!=1 || d[i][j]<=0)
                {
                    printf("The input file should be positive fixed decimal values.\n");
                    exit(1);
                }
                sum_check+=d[i][j];
                d[i][j]=sum_check;
            }
            /* Check rows of the 4x4 matrix must add to 1.0*/
            if(fabs(sum_check-1)>0.0001)
            {
                printf("Rows of the 4x4 matrix must add to 1.0.\n");
                exit(1);
            }
        }
    }
    
    /* Broadcast f and d matrix to other processes.*/
    MPI_Bcast(&f, 4, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&d, 4*4, MPI_INT, 0, MPI_COMM_WORLD);
    
    /* Set different random seeds for each processes.*/
    srandom(id+1);    
    
    /*Initialize*/
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
    
    /* Initialize the flags array with val to be 0,
    *  and rank to be the id of the process.
    */
    local_flags=malloc(p*sizeof(FlagsInfo));
    global_flags=malloc(p*sizeof(FlagsInfo));
    for(i=0;i<p;i++)
    {
        local_flags[i].val = 0;
        local_flags[i].rank = id;
        
    }
    
    /*Simulation start*/
    int iteration=1;
    int stop=1; /* Keep running the simulation until reaching to the steady state than set stop to 0.*/
    double uniform_random_number;
    while(stop)
    {
        /*New cars arrive at entrances*/
        for(i=0;i<4;i++)
        {
            uniform_random_number=(double) (random()) / RAND_MAX;
            if(uniform_random_number<=1/f[i]) 
            {
                arrival[i]=1;
                arrival_cnt[i]+=1;
            }
            else
            {
                arrival[i]=0;
            }
            
        }
       
        /*Cars inside circle advance simultaneously*/
        for(i=0;i<16;i++)
        {
            j=(i+1)%16;
            if(circle[i]==-1 || circle[i]==j) /*If the segement is empty, or the car is leaving*/
            {
                new_circle[j]=-1;
            }
            else
            {
                new_circle[j]=circle[i];
            }
        }
        for(i=0;i<16;i++)
        {
            
            circle[i]=new_circle[i];
            
        }
        
        /*Cars enter circle*/
        for(i=0;i<4;i++)
        {
            /*There is space for car to enter*/
            if(circle[offset[i]]==-1)
            {
                /*Car waiting in queue enters circle*/
                if(queue[i]>0)
                {
                    queue[i]--;
                    circle[offset[i]]=choose_exit(&i, offset, d);
                }
                /*Newly arrived car enters circle*/
                else if(arrival[i]>0)
                {
                    arrival[i]=0;
                    circle[offset[i]]=choose_exit(&i, offset, d);
                }
            }
            /*Newly arrived car queues up*/
            if(arrival[i]>0)
            {
                wait_cnt[i]++;
                queue[i]++;
            }
        }

        /*Accumulated the queue length*/
        for(i=0;i<4;i++)
        {
            queue_accum[i]+=queue[i];
        }
        
        /*Calculate the average queue length in every 10 iterations. */
        if(iteration%10==0)
        {
            for(i=0;i<4;i++)
            {
                queue_avg[i]=(double)queue_accum[i]/(double)iteration;
                if(queue_avg[i]>queue_avg_max[i])
                {
                    queue_avg_max[i]=queue_avg[i];
                }
                if(queue_avg[i]<queue_avg_min[i])
                {
                    queue_avg_min[i]=queue_avg[i];
                }
            }
            
        }
        /* Check whether the simulation has reached the steady state or not in every 100 iterations.*/
        if(iteration%100==0)
        {
            if(check_steady_state(queue_avg_max,queue_avg_min)) /*Once the simuation has reached the steady state, set its flag to 1*/
            {
                local_flags[id].val=1;
                
            }
            /* Root receives the flags of all the processes. */
            MPI_Reduce(local_flags,global_flags,p,MPI_2INT,MPI_MAXLOC,0,MPI_COMM_WORLD);
            
            /* Root checks which process has reached the steady state */ 
            if(id==ROOT)
            {
                for(i=0;i<p;i++)
                {
                    /* If one of the processes has finished, set stop to 0 to stop the simulation, 
                    *  and set id_pos to the id of the process which has reached the steady state first.
                    */
                    if(global_flags[i].val==1)
                    {
                        stop=0;
                        id_pos=i;
                        break;
                    } 
                }
            }
            
            /* Root broadcasts stop to 0 to all the processes to stop the simulations. */
            MPI_Bcast(&stop, 1, MPI_INT, 0, MPI_COMM_WORLD);
            /* Root broadcasts id id of the process which has reached the steady state first. */
            MPI_Bcast(&id_pos, 1, MPI_INT, 0, MPI_COMM_WORLD);
            
            /* If no process has finished in every 100 iterations, reset the maxium and minimum of the average queue length. */
            reset_queue_max_min(queue_avg_max,queue_avg_min);
        }
        iteration++;
    }
    
    /* The process which has finished first will output the results, the steady state probability and the average queue length
    *  at each entrances in the sequence of N, W, S, E.
    */
    if(id==id_pos)
    {   
        for(i=0;i<4;i++)
        {
            printf("%.2lf %.2lf\n",(double)wait_cnt[i]/(double)arrival_cnt[i],queue_avg[i]);
        }
    }
    MPI_Finalize(); 
    return 0;
}

int choose_exit(int* enter_dir, int* offset, float d[][4])
{
    double random_num=(double) (random()) / RAND_MAX;
    for(int i=0;i<4;i++)
    {
        if(d[*enter_dir][i]>=random_num) /*If the generated random number bigger or equal than the cumulative probabilities of array d[entrance][exit], return the offset[exit]. */
        {
            return offset[i];
        }
    }
}

void reset_queue_max_min(double queue_avg_max[],double queue_avg_min[])
{
    for(int i=0;i<4;i++)
    {
        queue_avg_max[i]=INT_MIN;
        queue_avg_min[i]=INT_MAX;
    }
}

int check_steady_state(double queue_avg_max[],double queue_avg_min[])
{
    int i;
    for(i=0;i<4;i++)
    {
        if(queue_avg_max[i]-queue_avg_min[i]>0.01) break;
    }
    if(i==4) return 1;
    return 0;
}
