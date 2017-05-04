/* -  The input matrix is overwritten by the matrix of lengths of shortest
      paths.
-  No error checking is done on the input.
-  The adjacency matrix is stored as a 1-dimensional array and subscripts
      are computed using the formula:  the entry in the ith row and jth
     column is mat[i*n + j]
-  The number of vertices (n) must be evenly divisible by the number of
 	   processes for this program to work correctly--i.e., n must be a multiple
 	   of p.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int INFINITY = 9999;

int getNumberVertices();
char* getFilename();
void readMatrix(char filename[], int mat[], int n);
void printMatrix(int mat[], int n);
int min(int x, int y);
void floyd(int p, int n, int local_mat[], int my_rank);


int main(int argc, char* argv[])
{
    int p;
    int my_rank;
    int n;
    int* mat;
    int* local_mat;
    int* temp_mat;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    // Gets  data from user
    if (my_rank == 0)//master
    {
        printf("How many vertices? ");
        scanf("%d", &n);

        mat = malloc(n * n * sizeof(int));

        char filename[40];
        printf("Enter the filename: ");
        scanf("%s", filename);

        readMatrix(filename, mat, n);
    }

    // Buffer allocation for local rows
    local_mat = malloc(n * (n/p) * sizeof(int));

    // Buffer allocation for revised matrix
    temp_mat = malloc(n * n * sizeof(int));

    // Broadcasts n (number of locations) to each processor
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //Distributes matrix to each processor
    MPI_Scatter(mat, n * (n/p), MPI_INT,
                local_mat, n * (n/p), MPI_INT, 0, MPI_COMM_WORLD);

    // Uses Floyd's alg to compute least cost between locations
    floyd(p, n, local_mat, my_rank);

    // Gathers the data
    MPI_Gather(local_mat, n * (n/p), MPI_INT, temp_mat,
               n * (n/p), MPI_INT, 0, MPI_COMM_WORLD);

    // Prints the matrix
    if (my_rank == 0)
    {
        printf("The solution is:\n");
        printf("\n");
        printMatrix(temp_mat, n);
        printf("\n");
    }

    MPI_Finalize();
    return(0);
}

// Creates adjacency matrix from file specified by user
void readMatrix(char filename[], int mat[], int n)
{
    FILE *file;
    file = fopen(filename, "r");
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            fscanf(file, "%d", &mat[i * n + j]);

    fclose(file);
}

void printMatrix(int mat[], int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (mat[i * n + j] == INFINITY)
                printf("i ");
            else
                printf("%d ", mat[i * n + j]);
        }
        printf("\n");
    }
}

// Returns minimum of two int s->used in Floyd's algorithm
int min(int x, int y)
{
    if (x < y)
        return x;
    else
        return y;
}

//Floyd's Algorithm
void floyd(int p, int n, int local_mat[], int my_rank)
{
    int* row_int_city;
    int local_int_city;
    int root;

    row_int_city = malloc(n * sizeof(int));

    int int_city;
    for (int_city = 0; int_city < n; int_city++)
    {
        root = int_city / (n / p);
        if (my_rank == root)
        {
            local_int_city = int_city % (n / p);//n has to be a multiple of p
            int j;
            for (j = 0; j < n; j++)
                row_int_city[j] = local_mat[local_int_city * n + j];
        }
        MPI_Bcast(row_int_city, n, MPI_INT, root, MPI_COMM_WORLD);
        int local_city1;
        for (local_city1 = 0; local_city1 < n / p; local_city1++)
        {
            int city2;
            for (city2 = 0; city2 < n; city2++)
            {
                local_mat[local_city1 * n + city2] =
                    min(local_mat[local_city1 * n + city2],
                        local_mat[local_city1*n + int_city] + row_int_city[city2]);
            }
        }
    }
    free(row_int_city);
}

