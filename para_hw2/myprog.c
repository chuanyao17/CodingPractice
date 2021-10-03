#include <stdio.h>
int main  ( int argc , char * argv [])
{
    // char test[5]="HI";
    // printf("%s\n",test);
    int num_tasks;
    scanf("%d",&num_tasks);
    printf("%d\n",num_tasks);
    if (num_tasks<5)
    {
        printf("small\n");
    }
    return 0;
}