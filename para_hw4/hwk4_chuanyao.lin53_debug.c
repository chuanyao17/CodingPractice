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
#include <limits.h>


#define ROOT 0

typedef struct{
    int val;
    int rank;
}FlagsInfo;


int choose_exit(int* enter_dir, int* offset, float d[][4]);
void reset_queue_max_min(double queue_avg_max[],double queue_avg_min[]);
int check_steady_state(double queue_avg_max[],double queue_avg_min[]);


void read_circle(int circle[])
{
    printf("circle_array= ");
    for(int i=0;i<16;i++)
    {
        printf("%2d ",circle[i]);
    }
    printf("\n");
}

void read_arrival(int arrival[], int arrival_cnt[])
{
    printf("arrival_array= ");
    for(int i=0;i<4;i++)
    {
        printf("%d ",arrival[i]);
    }
    printf("cnt= ");
    for(int i=0;i<4;i++)
    {
        printf("%d ",arrival_cnt[i]);
    }
    printf("\n");
}

void read_queue(int queue[])
{
    printf("queue_array= ");
    for(int i=0;i<4;i++)
    {
        printf("%d ",queue[i]);
    }
    printf("\n");
}


void read_queue_avg(double queue_avg[])
{
    printf("queue_avg= ");
    for(int i=0;i<4;i++)
    {
        printf("%lf ",queue_avg[i]);
    }
    printf("\n");
}

void print_index()
{
    printf("indexe_array= ");
    for(int i=0;i<16;i++)
    {
        printf("%2d ",i);
    }
    printf("\n");
    printf("\n");
}


void print_result(double queue_avg[], int wait_cnt[], int arrival_cnt)
{

}


int main (int argc, char* argv[])
{       
    int id;             /* rank of executing process   */
    int p;              /* number of processes         */
    double elapsed_time;  /* Time to find, count solutions */
    
    FILE * infile; /* target text file */
    int            i,j;
    float f[4];
    float d[4][4];
    float sum_check;
    int id_pos;
    
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
            //printf("%.50f in row %d\n ",sum_check, i);
            if(sum_check!=1.0)
            {
                printf("Rows of the 4x4 matrix must add to 1.0.\n");
                printf("~~~~~~~%.50f in row %d\n ",sum_check-1, i);
                exit(1);
            }
        }
        
        
        
    }
    MPI_Bcast(&f, 4, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&d, 4*4, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process=%d f_array \n",id);
    for ( i = 0; i < 4; i++ )
    {
        printf(" %f ", f[i]);
    }
    printf("\n");
    for ( i = 0; i < 4; i++ ) 
    {
        for ( j = 0; j < 4; j++ ) 
        {
            printf(" %f ", d[i][j]);
        }
        printf("\n");
    }
    srandom(id+1);    
    
    local_flags=malloc(p*sizeof(FlagsInfo));
    global_flags=malloc(p*sizeof(FlagsInfo));
    //printf("Process %d :",id);
    for(i=0;i<p;i++)
    {
        local_flags[i].val = 0;
        local_flags[i].rank = id;
        //printf(" %d",local_flags[i].val);
        
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
    double queue_avg[4];
    double queue_avg_max[4];
    double queue_avg_min[4];
    double queue_avg_diff[4];
    //double waiting_probability[4];
    
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
    
    /*simulation start*/
    int iteration=1;
    int stop=1;
    double uniform_random_number;
    while(stop)
    {
        /*new cars arrive at entrances*/
        //printf("===========================================interation=%d =============================\n",iteration);
        
        for(i=0;i<4;i++)
        {
            uniform_random_number=(double) (random()) / RAND_MAX;
            //printf("dir=%d first random=%lf f[]=%lf\n",i,uniform_random_number,1/f[i]);
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
        read_arrival(arrival,arrival_cnt);
        /*cars inside circle advance simultaneously*/
        for(i=0;i<16;i++)
        {
            j=(i+1)%16;
            //if(circle[i]==j)printf("car leaves at %d\n",j);
            if(circle[i]==-1 || circle[i]==j)
            {
                new_circle[j]=-1;
            }
            else
            {
                new_circle[j]=circle[i];
            }
        }
        //print_index();
        
        for(i=0;i<16;i++)
        {
            
            circle[i]=new_circle[i];
            
        }
        //read_circle(circle);
        /*cars enter circle*/
        
        for(i=0;i<4;i++)
        {
            /*there is space for car to enter*/
            if(circle[offset[i]]==-1)
            {
                /*car waiting in queue enters circle*/
                if(queue[i]>0)
                {
                    queue[i]--;
                    circle[offset[i]]=choose_exit(&i, offset, d);
                    //printf("queue enters circle, dir=%d circle[%d]=%d\n",i,offset[i],circle[offset[i]]);
                }
                /*newly arrived car enters circle*/
                else if(arrival[i]>0)
                {
                    arrival[i]=0;
                    circle[offset[i]]=choose_exit(&i, offset, d);
                    //printf("newly arrived car enters circle, dir=%d circle[%d]=%d\n",i,offset[i],circle[offset[i]]);
                }
            }
            /*newly arrived car queues up*/
            if(arrival[i]>0)
            {
                wait_cnt[i]++;
                queue[i]++;
                printf("wait dir=%d wait_cnt[%d]=%d\n",i,i,wait_cnt[i]);
                
            }
            
        }
        read_queue(queue);
        read_circle(circle);
        /*accumulated the queue length*/
        for(i=0;i<4;i++)
        {
            queue_accum[i]+=queue[i];
        }
        
        if(iteration%10==0)
        {
            for(i=0;i<4;i++)
            {
                queue_avg[i]=(double)queue_accum[i]/(double)iteration;
                //printf("dir=%d avg_queue=%lf   itr=%d    queue_accum=%d \n",i,queue_avg[i],iteration,queue_accum[i]);
                if(queue_avg[i]>queue_avg_max[i])
                {
                    queue_avg_max[i]=queue_avg[i];
                }
                if(queue_avg[i]<queue_avg_min[i])
                {
                    queue_avg_min[i]=queue_avg[i];
                }
                //printf("dir=%d queue_avg_max=%lf  queue_avg_min=%lf\n",i,queue_avg_max[i],queue_avg_min[i]);
            }
            
        }
        if(iteration%100==0)
        {
            printf("Check Process %d\n",id);
            if(check_steady_state(queue_avg_max,queue_avg_min))
            {
                printf("Process %d iteration=%d\n",id,iteration);
                local_flags[id].val=1;
                
            }
            MPI_Reduce(local_flags,global_flags,p,MPI_2INT,MPI_MAXLOC,0,MPI_COMM_WORLD);
            
            if(id==ROOT)
            {
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
            }
            
            MPI_Bcast(&stop, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&id_pos, 1, MPI_INT, 0, MPI_COMM_WORLD);
            reset_queue_max_min(queue_avg_max,queue_avg_min);
        }
        
        
        iteration++;
        
    }
    if(id==id_pos)
    {
        printf("Process %d is the fatest!!\n",id);
        read_queue_avg(queue_avg);
        
        printf("%.2lf %.2lf\n",(double)wait_cnt[0]/(double)arrival_cnt[0],queue_avg[0]);
        printf("%.2lf %.2lf\n",(double)wait_cnt[3]/(double)arrival_cnt[3],queue_avg[3]);
        printf("%.2lf %.2lf\n",(double)wait_cnt[2]/(double)arrival_cnt[2],queue_avg[2]);
        printf("%.2lf %.2lf\n",(double)wait_cnt[1]/(double)arrival_cnt[1],queue_avg[1]);
    }
    MPI_Finalize(); 
    return 0;
}

int choose_exit(int* enter_dir, int* offset, float d[][4])
{
    double random_num=(double) (random()) / RAND_MAX;
    //printf("check random=%lf\n",random_num);
    for(int i=0;i<4;i++)
    {
        if(d[*enter_dir][i]>=random_num)
        {
            //printf("%f i=%d\n",d[*enter_dir][i],i);
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
        //printf(" %lf\n",queue_avg_max[i]-queue_avg_min[i]);
        if(queue_avg_max[i]-queue_avg_min[i]>0.01) break;
    }
    if(i==4) return 1;
    return 0;
}
