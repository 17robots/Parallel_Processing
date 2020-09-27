#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void mat_mult(int *local_a, int *local_y, int *x, int loc_rows, int cols)
{
  for (int i = 0; i < loc_rows; ++i)
  {
    local_y[i] = 0;
    for (int j = 0; j < cols; ++j)
    {
      printf("local_y[%d] = %d + %d * %d\n", i, local_y[i], local_a[i * cols + j], x[j]);
      local_y[i] += local_a[i * cols + j] * x[j];
    }
    printf("local_y[%d] = %d\n", i, local_y[i]);
  }
}

int main()
{
  int size, rank;
  int rows, cols;
  int *y, *local_y, *a, *local_a, *x;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    FILE *file;
    file = fopen("mv-data.txt", "r");
    fscanf(file, "%d", &rows);
    fscanf(file, "%d", &cols);

    a = malloc(sizeof(int) * rows * cols);
    for (int i = 0; i < rows * cols; ++i)
      fscanf(file, "%d", &a[i]);

    x = malloc(sizeof(int) * cols);
    for (int i = 0; i < cols; ++i)
      fscanf(file, "%d", &x[i]);

    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(x, cols, MPI_INT, 0, MPI_COMM_WORLD);
  }

  if (rank != 0)
  {
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    x = malloc(sizeof(int) * cols);
    MPI_Bcast(x, cols, MPI_INT, 0, MPI_COMM_WORLD);
  }

  local_a = malloc(sizeof(int) * cols);
  local_y = malloc(sizeof(int) * rows / size);
  MPI_Scatter(a, cols, MPI_INT, local_a, cols, MPI_INT, 0, MPI_COMM_WORLD);

  printf("\nCalculating Process %d's Y value(s)\n", rank);
  mat_mult(local_a, local_y, x, rows / size, cols);
  MPI_Barrier(MPI_COMM_WORLD);
  y = malloc(sizeof(int) * rows);
  MPI_Gather(local_y, rows / size, MPI_INT, y, rows / size, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    printf("\nResult:\n");
    printf("[ ");
    for (int i = 0; i < size; ++i)
      printf("%d ", y[i]);
    printf("]\n");
    free(a);
    free(y);
  }
  free(x);
  free(local_a);
  free(local_y);
  MPI_Gather(NULL, 0, MPI_BYTE, NULL, 0, MPI_BYTE, 0, MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}