/******************************************************************************
Title : psrs_test.c
Author : Chuanyao Lin
Created on : Nov 29, 2021
Description : Write a MPI program to simulate the PSRS algorithm.
Usage : psrs_test <the number of random numbers to be generated>
Build with : mpicc -o psrs_test psrs_test.c
Modifications: Nov 29, 2021
******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

void random_array(int *arr, int num) {
    for(int i = 0; i < num; i++) {
        srand(clock());
        arr[i] = rand() % (num+1);
    }
}


int compare(const void * a, const void * b) {
  if (*(int*)a < *(int*)b) return -1;
  if (*(int*)a > *(int*)b) return 1;
  else return 0;
}


void print_array(int *arr, int num) {
    for(int i = 0; i < num; i++) {
        if (i % 20 == 0)
            printf("\n");
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void merge(int *arr, int front, int mid, int end) {
    int i, k;
    int *tmp = (int *)malloc((end-front+1)*sizeof(int));
    
    int left_front = front;
    int left_end = mid;
    int right_front = mid + 1;
    int right_end = end;
    
    for(k=0; left_front<=left_end && right_front<=right_end; k++){  
        if(arr[left_front]<=arr[right_front]){
            tmp[k] = arr[left_front++];
        }else{
            tmp[k] = arr[right_front++];
        }
    }
    if(left_front <= left_end){ 
  
        for(i=left_front;i<=left_end;i++)
            tmp[k++] = arr[i];
    }
    
    
    if(right_front <= right_end){

        for(i=right_front; i<=right_end; i++)
            tmp[k++] = arr[i];
    }
    for(i=0; i<end-front+1; i++)
        arr[front+i] = tmp[i];
    return;
}


void merge_sort(int *arr, int front, int end) {
    if (front < end) {
        int mid = front+(end-front)/2;
        merge_sort(arr, front, mid);
        merge_sort(arr, mid + 1, end);
        merge(arr, front, mid, end);
    }
}

int main(int argc, char *argv[])
{
    int *array; /*Store the input array*/
    int *sub_array;    /*Store the distributed array(sublist) of each process*/
    //int *result, 
    int *ans; /*Store the sorted array by the qsort() for double checking*/
    int n; /*The number of the elements in the input array*/
    int p, id; /*The number of the processes and thier id*/
    int group; /*The size of creating the sublist initially*/
    int group_len; /*The real size of the sublist after assigned*/
    int mod; /* The number of the remain elements*/
    
    int i, index;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Status status;
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (argc != 2) {
        fprintf(stderr, "Please input the num of array!\n");
        return 1;
    }
    n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Please, the size must > 0.\n");
        return 1;
    }
    
    /*Generate an array with n random numbers*/
    if (id == 0) {
        array = (int*)malloc((n+2)*sizeof(int));
        //result = (int*)malloc((n+2)*sizeof(int));
        ans = (int*)malloc((n+2)*sizeof(int));
        random_array(array, n);
        
        printf("Original unsorted list:");
        print_array(array,n);
        printf("\n");
        
        //Sort the whole input by qsort for reference.
        memcpy(ans, array, (n)*sizeof(int));
        qsort(ans, n, sizeof(int), compare);
        
        printf("Sorted list for checking:");
        print_array(ans,n);
        printf("\n");
    }
    
    /*Initialize the sub-array*/
    group = n / p;
    mod = n % p;
    sub_array = (int *)malloc((group+mod)*sizeof(int));
    
    int *send_counts; /*Store the size of the assgined sublist of each process*/
    int *send_index; /* Store the start index of the assgined sublist of each process*/
    send_counts = (int *)malloc((p)*sizeof(int));
    send_index = (int *)malloc((p)*sizeof(int));
    for(int i = 0; i < p; i++) {
        send_counts[i] = (((i+1)*n)/p)-((i*n)/p);
        send_index[i] = ((i*n)/p);
    }
    
    /*Assign the sublist from process 0 to each process.*/
    MPI_Scatterv(array, send_counts, send_index, MPI_INT, sub_array, group+mod, MPI_INT, 0, MPI_COMM_WORLD);
    group_len = send_counts[id];
    for(i = 0; i < p; i++){
        if(id==i){
            printf("Process %d assigned sublist:",i);
            print_array(sub_array,group_len);
            printf("\n");
        }
    }
    /*Each process sorts the sublist by qsort()*/
    qsort(sub_array, group_len, sizeof(int), compare);
    
    /*Initialize the regular sample array*/
    int *samples; /*Store p^2's elements(regular samples gathered from the array s of each process)*/
    int *samples_picked; /*Store the elements picked from the sublist of each process*/
    samples =(int *)malloc((p*p)*sizeof(int));
    samples_picked =(int *)malloc((p)*sizeof(int));   
    for (i = 1; i < p; i++) {
        samples_picked[i-1] = sub_array[i * group / p];
    }
    /*Gather the array samples_picked of each process into the array sample at the process 0*/
    MPI_Gather(samples_picked, p-1, MPI_INT, samples, p-1, MPI_INT, 0, MPI_COMM_WORLD);
    
    /*Initial the pivot array.*/
    int *pivot;
    pivot =(int *)malloc((p)*sizeof(int));
    
    /*Process 0 will sort the array sample than pick the pivots.*/
    if (id == 0) {
        qsort(samples, (p-1)*p, sizeof(int), compare);
        for (i = 1; i < p; i++) 
            pivot[i-1] = samples[i*(p-1)];
    }
    
    /*Process 0 broadcasts the pivots to the other process.*/
    MPI_Bcast(pivot, p-1, MPI_INT, 0, MPI_COMM_WORLD);
    //print_array(pivot,p-1);
    
    /*Initial the p_segment_counts array*/ 
    int *p_segment_counts; /*Store the size of segments sent to each process.*/
    p_segment_counts =(int *)malloc((p)*sizeof(int));
    index = 0;
    
    for(i = 0; i < p; i++)
        p_segment_counts[i] = 0;
    pivot[p-1] = INT_MAX; /*Set the last pivot to be INT_MAX, since the number of the pivots is only p-1.*/
    
    /*Divide the sublist by the pivots, check how many elements are smaller than the specific pivot, so that we'll know the size of each segments*/
    for(i = 0; i < group_len; i++) {
        if(sub_array[i] <= pivot[index])
            p_segment_counts[index]++;
        else {
            i--;
            index++;
        }
    }
    
    /*Initial the received_segement_counts array*/ 
    int *received_segement_counts; /*Store the size of segments after calling all to all processes of each process.*/
    received_segement_counts =(int *)malloc((p)*sizeof(int));
    
    /*Send data(the number of the segments) from all to all processes*/
    MPI_Alltoall(p_segment_counts, 1, MPI_INT, received_segement_counts, 1, MPI_INT, MPI_COMM_WORLD);
    
    /*Initial the receive_index array*/
    int *receive_index; /*Store the start index of each segments that are going to be receive from each process*/
    receive_index=(int *)malloc((p)*sizeof(int));
    send_index[0] = 0;
    receive_index[0] = 0;
    
    
    for(i = 1;i < p;i++) {
        send_index[i] = send_index[i-1] + p_segment_counts[i-1]; /*Calculate the start index of each segments that are going to be sent to each process*/
        receive_index[i] = receive_index[i-1] + received_segement_counts[i-1]; /*Calculate the start index of each segments that are going to be received from each process*/
    }
    
    /*Calculate the size of the sublist after calling all to all processes of each process.*/
    int totalcounts = 0;
    for(i = 0; i < p; i++)
        totalcounts += received_segement_counts[i];
    
    /*Initial the sub_array_all_to_all array.*/
    int *sub_array_all_to_all = (int *)malloc(totalcounts*sizeof(int));
    
    /*Send data(the segments) from all to all processes*/
    MPI_Alltoallv(sub_array, p_segment_counts, send_index, MPI_INT, sub_array_all_to_all, received_segement_counts, receive_index, MPI_INT, MPI_COMM_WORLD);
        
    /*Merge the sublist after exchanging the data*/
    merge_sort(sub_array_all_to_all, 0, totalcounts-1);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    /*Each process print their sorted sublist*/
    for(i = 0; i < p; i++){
        if(i==id){
            printf("Process %d sorted sublist:",i);
            print_array(sub_array_all_to_all,totalcounts);
            printf("\n");
        }
    }
    
    MPI_Finalize();
    return 0;
}