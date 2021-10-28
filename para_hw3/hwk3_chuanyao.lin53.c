/******************************************************************************
Title : hwk3_chuanyao.lin53.c
Author : Chuanyao Lin
Created on : Oct 27, 2021
Description : Write a MPI program, when given two command line arguments,
the first of which is a string called the key, and the second of which is the
path name of a text file, searched for all occurrences of the key in the text file.
Purpose : Practice writing the sequential program as a MPI program and running the program parallelly.
Usage : hwk3_chuanyaolin53 <a string of the key> <path of the text file>
Build with : mpicc -o hwk3_chuanyaolin53 hwk3_chuanyao.lin53.c
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


int key_search(char* key, char* file_str, int key_length, int position);
/** key_search()
 *  Give the position of the file_str, this function will start to compare the following characters with the key.
 *  Once the key is found, it will return 1.
*/
void print_key(int* key_arr, int key_count, int id);
/** print_key()
 *  This function will print numbers stored in the key_arr based on the length of key_count.
*/
int main (int argc, char* argv[])
{       
    struct stat statbuf;/* size of the text file  */
    int id;             /* rank of executing process   */
    int p;              /* number of processes         */
    double elapsed_time;  /* Time to find, count solutions */
    
    FILE* input_file; /* target text file */
    char* file_str; /* char array to store the content of the text file */
    intmax_t file_size; /* size of the text file */
    int key_length; 
    int key_count; /* numbers of the positions that the key is found */
    int* key_arr; /* int array to store the positions that the key is found */
    MPI_Status status ;
    
    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    /* Start timer */
    elapsed_time = - MPI_Wtime();
    
    
    
    /* The root process read the both key and file size */
    if(ROOT == id)
    {
        /* Check the usage of the command line */
        if(3>argc) 
        {   
            printf("Usage: %s key filename\n", argv[0]);
            exit(1);
        }
        
        
        /* Call stat () to fill statbuf struct */
        /* Refer to the code of the instructions of Assignment 3 written by the Prof Stewart Weiss. 
               from line 75 to 89.
        */
        if(stat(argv[2], &statbuf)==-1) 
        {
            printf("Could not stat file %s\n", argv[2]);
            exit(1);
        }
        file_size=statbuf.st_size;
        if(file_size==0)
        {   
            printf("file has no contents inside\n");
            exit(1);
        }
        if(strlen(argv[1])<1)
        {
        printf("key should not be empty\n");
        exit(1);
        }
    }
    
    /* The root process will broadcast the number of file size to all other processes. */
    MPI_Bcast(&file_size, 1, MPI_UNSIGNED_LONG_LONG, ROOT, MPI_COMM_WORLD);
    
    /* Initialize a char array with size equal to the file size */
    file_str=malloc(file_size*sizeof(char));
    
    /* The root process will read the text file to the created char array */
    if(ROOT == id)
    {   
        input_file=fopen(argv[2], "r");
        fread(file_str,sizeof(file_str),file_size,input_file);
    }
    
    /* The root process will broadcast the char array which stored the contents of the text file
           to all other processes. */
    MPI_Bcast(&file_str[0], file_size, MPI_CHAR, ROOT, MPI_COMM_WORLD);
   
    /* Calculate the start,end position for each process to read the char arr,
           so each process will handle the length of char arr roughly equal.
    */
    int task_num=file_size/p; /* the least number of tasks handled by each process*/
    int task_remainder_num=file_size%p; /* the number of remaining tasks can't be distributed evenly */
    int start, end;
    if(id<task_remainder_num)
    {   
        start=id*(task_num+1);
        end=start+task_num;
    }
    else
    {
        start=id*task_num+task_remainder_num;
        end=start+task_num-1;
    }
    
    /* Initialize a int array with size equal to the task_num+1 */
    key_arr=malloc((task_num+1)*sizeof(int));
    key_count=0;
    key_length=strlen(argv[1]);
    
    /* Go through every specified position of the file_str to check if the key is founded.
           Once the key is founded, store the beginning position to the key_arr.
    */
    for(int i=start;i<=file_size-key_length && i<=end ;i++)
    {
        if(key_search(argv[1], file_str, key_length, i))
        {       
            key_arr[key_count++]=i;
        }
        
    }
    
    /* The root process will receive the results sent from other processes and 
           print all the positions where the key is found in ascending order.
    */
    if(ROOT==id)
    {
        printf("OUTPUT:\n");
        print_key(key_arr, key_count, id);
        if(p>1)
        {
            for(int i=1;i<p;i++)
            {
                MPI_Recv(key_arr, task_num+1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_INT, &key_count);
                print_key(key_arr, key_count, i);
            }
        }
        printf("Finished!!\n");
    }
    else
    {
        MPI_Send(key_arr, key_count, MPI_INT, ROOT, 1, MPI_COMM_WORLD);
    }
  
    
    MPI_Barrier (MPI_COMM_WORLD);
    
    /* Stop timer */
    elapsed_time += MPI_Wtime();
    if (ROOT == id) {
        printf ("Execution time %8.6f\n", elapsed_time);
    
    }

    MPI_Finalize(); 
    return 0;
}
int key_search(char* key, char* file, int key_length, int position)
{   
    for(int i=0;i<key_length;i++)
    {   
        if(key[i]!=file[position+i])
        {
            return 0;
        }
    }
    return 1; 
}
void print_key(int* key_arr, int key_count, int id)
{
    for(int i=0;i<key_count;i++)
    {
        
        printf("%d, ",key_arr[i]);
    }
}
/*
    The running time of this program running sequentially is O(n*m) where n is the size of text file, and m is the length of the key.
    Read the text file into the char arr with n time, iterate n positions of the text file, and iterate m length of the key to find whether the key is found.
    Total searching time is n*m. The worst case to print the result will be n. So, total running time is O(n*m). 
    
    The running time of this program running parallelly is O(n/p*m+nlog p+ n).
    n/p*m is the total searching time. nlog p is the mpi brodcast time. p*n/p is the mpi send time.
*/
