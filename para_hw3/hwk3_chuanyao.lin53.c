#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
// #include "mpi.h"

#define ROOT 0



// int main (int argc, char* argv[])
// {
//     struct stat statbuf;
//     int id;             /* rank of executing process   */
//     int p;              /* number of processes         */

//     FILE *input_file;
//     char *str;
//     intmax_t file_size;
  
//     MPI_Init      ( &argc, &argv);
//     MPI_Comm_rank ( MPI_COMM_WORLD, &id);
//     MPI_Comm_size ( MPI_COMM_WORLD, &p );
    
    
//     if (2>argc) 
//     {
//         printf("Usage: %s filename\n", argv[0]);
//         exit(1);
//     }

    
//     if(ROOT == id)
//     {
//         /* Call stat () to fill statbuf struct */
//         if(stat(argv[1], &statbuf)==-1) 
//         {
//             printf("Could not stat file %s\n", argv[1]);
//             exit(1);
//         }
//         /* Print size of file . intmax_t is a type that holds big numbers */
//         file_size=statbuf.st_size;
//         printf("id=%d%8jd\n", id,file_size);
//         input_file=fopen("test.txt", "r");
//         str=malloc(file_size*sizeof(char));
//         fread(str,sizeof(str),file_size,input_file);
//     }
//     MPI_Bcast(str, strlen(str), MPI_CHAR, ROOT, MPI_COMM_WORLD);

//     for(int i=id;i<strlen(str);i+=p);
//     {
//         printf("id=%d .. %c\n",id,str[i])
//     } 

//     MPI_Finalize(); 
//     return 0;
// }

int main (int argc, char* argv[])
{
    struct stat statbuf;
    int id;             /* rank of executing process   */
    int p;              /* number of processes         */

    FILE *input_file;
    char *str;
    intmax_t file_size;
  
    
    if (2>argc) 
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }
    /* Call stat () to fill statbuf struct */
    if(stat(argv[1], &statbuf)==-1) 
    {
        printf("Could not stat file %s\n", argv[1]);
        exit(1);
    }
    /* Print size of file . intmax_t is a type that holds big numbers */
    file_size=statbuf.st_size;
    printf("%8jd\n", file_size);
    input_file=fopen("test.txt", "r");
    str=malloc(file_size*sizeof(char));
    fread(str,sizeof(str),file_size,input_file);
    printf("str_length=%d\n",strlen(str));

    printf("start=%p\n",str);
    for(int i=0;i<strlen(str);i++)
    {
        printf("%d=%c %p \n",i,str[i],&str[i]);
        printf("byte=%d\n",(&str[i]-str)/sizeof(char));
    } 


    return 0;
}