/******************************************************************************
Title : myprog.c
Author : Chuanyao Lin
Created on : Oct 3, 2021
Description : Write a sequential program, in C or C++, that simulates the sequence of messages that would be
passed in a reduction operation that uses the binomial tree model of communication.
Purpose : Demostrates how the massages sended among each tasks.
Usage : myprog <n>
The number <n> is the number of tasks.
Build with : gcc -o myprog myprog.c 
Modifications: Oct 3, 2021
******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


void communication_simulation(int num_tasks, int num_nodes);

int main  ( int argc , char * argv [])
{
    int num_tasks;    //the number of tasks
    num_tasks=atoi(argv[1]);
    if(num_tasks<=0)
    {
        printf("The number of tasks should be a positive number!");
    return -1;
    }
    if(num_tasks==1)
    {
        printf("Only 1 task so no other tasks need to communicate with the root!");
        return 0;
    }
    communication_simulation(num_tasks,1);    //start with a root task
    return 0;
}
void communication_simulation(int num_tasks,int num_nodes)
{
    if (num_nodes>=num_tasks)return; //recusion will stop when the number of nodes is bigger or equal than the number of tasks
    num_nodes*=2; //partiton the tasks by doubling the existing nodes
    communication_simulation(num_tasks, num_nodes);
    int r=num_nodes-num_tasks; // if the number of tasks is not the power of 2, r represents the imaginary nodes of the leaf nodes
    int leaf_nodes=num_nodes/2; // the number of leaf nodes of the binomial tree
    for(int i=num_nodes;i>leaf_nodes;i--) // Simulate the communications from the leaf nodes to their receiver.
    {
        int sender=i-1;
        int receiver=i-leaf_nodes-1;
        if(r>0)
        {
            r--;
            continue;
        }
        printf("task %d sends a value to task %d\n",sender,receiver);
    }
    return;
}