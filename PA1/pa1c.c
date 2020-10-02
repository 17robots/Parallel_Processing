#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

void genArray(int num_nums, int low, int high, int *placedArray)
{
    srand(time(NULL));
    for (int i = 0; i < num_nums; ++i)
    {
        placedArray[i] = rand() % high + low;
    }
}

int factorial(int i)
{
    int result = 1;
    while (i > 0)
    {
        result *= i--;
    }
    return result;
}

int *calcFactorial(int *factArray, int elems)
{
    int *returnedArr = malloc(sizeof(int) * elems);
    for (int i = 0; i < elems; ++i)
    {
        returnedArr[i] = factorial(factArray[i]);
    }
    return returnedArr;
}

int main(void)
{
    // mpi vars
    int size, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // program vars
    double start, end, totalTime;
    int low, high;
    int *randArray, *localArray;

    if (rank == 0)
    {
        printf("Enter Upper Bound\n");
        scanf("%d", &high);

        printf("Enter Lower Bound\n");
        scanf("%d", &low);
    }

    for (int i = 1000; i < 16001; i *= 2)
    {
        if (rank == 0)
        {
            randArray = malloc(sizeof(int) * i);
            genArray(i, low, high, randArray);
        }

        localArray = malloc(sizeof(int) * i / size);
        MPI_Scatter(randArray, i / size, MPI_INT, localArray, i / size, MPI_INT, 0, MPI_COMM_WORLD);

        start = MPI_Wtime();
        int *resultArr = calcFactorial(localArray, i / size);
        end = MPI_Wtime() - start;
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Reduce(&end, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        if (rank == 0)
        {
            printf("Total Time for %d elements on %d processors: %f\n", i, size, totalTime);
        }
    }

    MPI_Finalize();
    return 0;
}