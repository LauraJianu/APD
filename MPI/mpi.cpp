#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define SIZE 100
#define value 42


int main(int argc, char *argv[])
{
    int rank, size, k, startP, endP, found = -1, finalFound = 0;
    int array[SIZE];

    srand(time(NULL) + 1);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        for (int i = 0; i < SIZE; i++)
        {
            array[i] = rand() % 100;
        }
    }

    k = SIZE / size;// each one gets its part of SIZE
    if (SIZE % size != 0)
        k++;

    MPI_Bcast(array, SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    startP = rank * k;
    endP = startP + k;

    for (int i = startP; i <= endP; i++)
    {

        if (array[i] == value)
        {
            found = i;
        }
    }

    MPI_Reduce(&found, &finalFound, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        if (finalFound == 0)
            printf("\nThe number wasn`t found\n");
        else printf("\nNumber found on position %d", finalFound);
    }


    MPI_Finalize();

}

