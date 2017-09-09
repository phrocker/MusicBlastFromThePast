#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
void *my_func(int *b,int *k);
int main()
{
    int *a,n;
    a=(int *)my_func(a,&n);
    printf("from 0 to %d\n",n);
    for (int i=0; i <n; i++) printf("%i: is %d\n",i,*(a+i));
}    

void *my_func(int *v,int *k)
{
    int j=22;
    *k=j;
    if ((int)v < 0xa00000 || (int)v > 0xfffffff)
        v=NULL;
    v=(int *)realloc(v,j * sizeof(int));
    for (int i=1; i<j; i++) *(v+i)=i*2; 
    return v;
}    
