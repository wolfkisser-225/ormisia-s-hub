#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_STR_LEN 256
#define NUM_STRings 16

void bubble_sort(void *base, size_t num, size_t size, int (*compare)(const void *,const void *)){
    if(num<=1||size==0||compare==NULL) return;
    //wrong input
    char *temp=(char *)malloc(size);
    if(temp == NULL) return;
    // memory allocation failed

    for(size_t i=0;i<num-1;i++){
        int swapped = 0;

        for(size_t j=0;j<num-i-1;j++){

            void *current =(char *)base + j*size;
            void *next = (char *)base + (j+1)*size;
            if(compare(current,next)>0){
                memmove(temp,current,size);
                memmove(current,next,size);
                memmove(next,temp,size);
                swapped=1;
            }
        }
        if(!swapped) break;
    }

    free(temp);
}

int compare_int(const void  *a,const void *b){
    return (*(int *)a-*(int *)b);
}

int compare_double(const void *a,const void  *b){
    return (*(double *)a-*(double *)b);
}

int main(){
    double double_arr[]={12.12312,445.1232,621,6.456,3,123,6,5.7898,232,6.01,23,6};
    size_t double_size=sizeof(double_arr)/sizeof(double_arr[0]);

    printf("原始数组\n");
    for(size_t i=0;i<double_size;i++)
        printf("%lf ",double_arr[i]);
    printf("\n");
    bubble_sort(double_arr,double_size,sizeof(double),compare_double);
    printf("排序后数组\n");
    for(size_t i=0;i<double_size;i++)
        printf("%lf ",double_arr[i]);
    printf("\n");
    return 0;
}
