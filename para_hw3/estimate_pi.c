/*******************************************************************************
  Title          : estimate_pi.c
  Author         : Stewart Weiss
  Created on     : January 16, 2014
  Description    : Interactively estimate pi 
  Purpose        : To illustrate MPI broadcast and MPI reduce 
  Usage          : estimate_pi                   
  Build with     : mpicc -Wall -o estimate_pi estimate_pi.c -lm
  Modifications  : 
 
*******************************************************************************/




#include <string.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define ROOT 0

double approximate_pi ( int num_segments, int id, int p);
/** approximate_pi()
 *  This returns an approximation of pi based on the mathematical identity
 *      tan(pi/4) = 1.0
 *  which is equivalent to 
 *      4.0 * arctan(1.0) = pi
 *  To compute pi, we can approximate arctan(1.0). We use the fact that 
 *      d/dx(arctan(x) = 1/(1 + x*x) 
 *  and compute the area under the curve of 1/(1 = x*x) in the interval
 *  from 0 to 1.0.  The area under this curve is arctan(1.0) because the
 *  the integral from 0 to 1 of 1/(1 = x*x) is arctan(1.0) - arctan(0) = pi/4.
 *  We multiply this by 4 to get pi.
 *  We use the trapezoid method to approximate the area under the curve. 
 *  We divide [0,1.0] into n segments of length 1/n each, and compute the 
 *  value of 1/(1 + x*x) at the midpoint of each segment. By summing these
 *  values and multiplying by 4 times 1/n, we have an approximation.
*/


int main( int argc, char *argv[] )
{
    int id;             /* rank of executing process   */
    int p;              /* number of processes         */
    double pi_estimate; /* estimated value of pi       */
    double local_pi;    /* each process's contribution */
    int num_intervals;      /* number of terms in series   */

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &id );
    MPI_Comm_size (MPI_COMM_WORLD, &p);

    /* repeat until user enters a 0 */
    while ( 1 ) {
        if (ROOT == id ) { 
            printf("Enter the number of intervals: [0 to quit] "); 
            fflush(stdout);
            scanf("%d",&num_intervals); 
        } 

        /* The root proces will broadcast the number of intervals entered
           to all other processes. Each process must call MPI_Bcast though.
           The data to be broadcast is num_intervals; the count = 1, it is of
           type MPI_INT; ROOT is the sender */
        MPI_Bcast(&num_intervals, 1, MPI_INT, ROOT, MPI_COMM_WORLD); 

        if (0 == num_intervals ) 
             break; 

        local_pi = approximate_pi(num_intervals, id, p);

        /* MPI_Reduce collects the local estimate from each process into 
           a global value, pi_estimate. The ROOT is the process that receives 
           all values in the reduction. The reduce operator is MPI_SUM. */
        MPI_Reduce(&local_pi, 
                   &pi_estimate, 1, MPI_DOUBLE, 
                   MPI_SUM, ROOT, MPI_COMM_WORLD);

        /* ROOT does the printing. The error is caculated by comparing to
           the math library's value for PI, M_PI. */
        if (ROOT == id  ) {
            printf("pi is approximated to be %.16f. The error is %.16f\n", 
            pi_estimate, fabs(pi_estimate - M_PI));
            fflush(stdout); 
        }
    }
    MPI_Finalize(); 
    return 0;
}

double approximate_pi ( int num_segments, int id, int p)
{
    double dx, sum, x;
    int i;

    /* Set dx to the width of each segments */
    dx = 1.0 / (double) num_segments; 

    /* Initialize sum */
    sum = 0.0; 

    /* Each process will compute its share of the segments. If the
       segments are numbered 1, 2, 3, ...,n, from left to right, then
       process id computes segment k if id = (k-1) % p, or equivalently
       it computes segments id+1, id+p+1, id+2p+1, ... up to id+mp+1, 
       where m is the largest number such that id+mp+1 <= num_segments. */
    for (i = id + 1; i <= num_segments; i += p) { 
        x = dx * ((double)i - 0.5); /* x is midpoint of segment i */ 
        sum += 4.0 / (1.0 + x*x);   /* add new area to sum */
    } 
    return  dx * sum;  /* we multiply sum by dx because we are computing
                          an integral and dx is the differential */
}




