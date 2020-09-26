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
      local_y[i] += local_a[i * cols + j] * x[j];
    }
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

    printf("Process %d allocating memory\n", rank);
    for (int i = 0; i < size; ++i)
    {
      MPI_Send(&rows, 1, MPI_INT, i, i, MPI_COMM_WORLD);
      MPI_Send(&cols, 1, MPI_INT, i, i, MPI_COMM_WORLD);
      MPI_Send(x, cols, MPI_INT, i, i, MPI_COMM_WORLD);
    }
  }

  if (rank != 0)
  {
    MPI_Recv(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&cols, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    x = malloc(sizeof(int) * cols);
    MPI_Recv(x, cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  local_a = malloc(sizeof(int) * cols);
  local_y = malloc(sizeof(int) * rows / size);
  MPI_Scatter(a, cols, MPI_INT, local_a, cols, MPI_INT, 0, MPI_COMM_WORLD);

  mat_mult(local_a, local_y, x, rows / size, cols);

  y = malloc(sizeof(int) * rows);
  MPI_Gather(local_y, rows / cols, MPI_INT, y, rows / cols, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    printf("[ ");
    for (int i = 0; i < size; ++i)
      printf("%d ", y[i]);
    printf("]\n");
  }

  MPI_Finalize();
  return 0;
}