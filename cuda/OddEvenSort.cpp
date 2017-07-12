#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
#include<cuda.h>
#include <device_launch_parameters.h>
#include <cuda_runtime_api.h>

__global__ void even(int *darr, int n)
{
    int k = threadIdx.x;
    int t;
    k = k * 2;//even positions
    if (k <= n )
    {
        if (darr[k] > darr[k + 1])//swap elements
        {
            t = darr[k];
            darr[k] = darr[k + 1];
            darr[k + 1] = t;
        }
    }
}

__global__ void odd(int *darr, int n)
{
    int k = threadIdx.x;
    int t;
    k = k * 2 + 1;//odd positions
    if (k <= n )
    {
        if (darr[k] > darr[k + 1])
        {
            t = darr[k];
            darr[k] = darr[k + 1];
            darr[k + 1] = t;
        }
    }

}

void main()
{
    int *arr, *darr;
    int n = 200, i;
    arr = (int*)malloc(n * sizeof(int));
    srand(time(NULL));
    for (i = 0; i < n; i++)
    {
        arr[i] = rand() % 100;
    }
    clock_t begin = clock();
    cudaMalloc((void **)&darr, n * sizeof(int));
    cudaMemcpy(darr, arr, n * sizeof(int), cudaMemcpyHostToDevice);//copy the device array to the cpu array
    for (i = 0; i <= n / 2; i++)
    {
        even << <1, n >> >(darr, n);
        odd << <1, n >> >(darr, n);
    }
    cudaMemcpy(arr, darr, n * sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(darr);
    clock_t end = clock();
    printf("Sorted array: ");
    for (i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("%ld", (double)(end - begin) / CLOCKS_PER_SEC);
    getch();
}
