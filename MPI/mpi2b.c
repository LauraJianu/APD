#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define SIZE 100
#define KEY 70


int main(int argc, char *argv[])
{
    int rank, size, k, index, found[SIZE], finalFound[SIZE + 10], poz;
    int arr[SIZE], segment[SIZE];
    bool ok = false;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    k = SIZE / size;
    if (SIZE % size != 0)
    {
        k++;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)//array init
    {
        printf("Array elements are:\n");
        for (int i = 0; i < SIZE; i++)
        {
            arr[i] = rand() % 100;
            printf("%d, ", arr[i]);

        }

    }

    MPI_Scatter(arr, k, MPI_INT, segment, k, MPI_INT, 0, MPI_COMM_WORLD);//arr to segment

    for (int i = 0; i < k; i++)
        found[i] = 0;

    index = 0;
    for (int i = 0; i < k; i++)
    {
        if (segment[i] == KEY)//search for KEY in different positions
        {
            found[index++] = i + rank * k;
        }
    }

    MPI_Gather(found, k, MPI_INT, finalFound, k, MPI_INT, 0, MPI_COMM_WORLD);//gathers the positions in finalFound

    if (rank == 0)
    {
        for (int i = 0; i < SIZE; i++)
            if (finalFound[i] > 0)
                ok = true;
            if (ok)
            {
            printf("\n Number found on the following positions: ");
            for (int i = 0; i < SIZE; i++)
                if (finalFound[i] != 0)
                    printf("%d ", finalFound[i]);
            }


    }


    MPI_Finalize();
    return 0;
}
