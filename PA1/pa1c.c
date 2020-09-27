#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

// int *genArray(int num_nums, int low, int high)
// {
//     srand(time(NULL));
//     int *returnedArray = malloc(sizeof(int) * num_nums);
//     for (int i = 0; i < num_nums; ++i)
//     {
//         returnedArray[i] = rand() % high + low;
//     }
//     return returnedArray;
// }

int main(void)
{
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int low, high;
    printf("Hello world\n");

    if (rank == 0)
    {
        int scanNum = 0;
        printf("Enter a value: ");
        fflush(stdout);
        while ((scanNum = scanf("%d", &low)) < 1 || low <= 0)
        {
            printf("Error Putting In Number. Make it a positive integer: \n");
            fflush(stdout);
        }
    }
    MPI_Finalize();
    return 0;
}