#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <sys/types.h> /* typedefs */
#include <sys/stat.h>

// void mat_vect_mult(int local_A[], int local_x[], int local_y[], int local_rows, int cols, int local_n, MPI_Comm comm)
// {
//   double *x;
//   int local_i, j;
//   int local_ok = 1;
//   x = malloc(cols * sizeof(double));
//   MPI_Allgather(local_x, local_n, MPI_INT, x, local_n, MPI_INT, comm);

//   for (local_i = 0; local_i < local_rows; ++local_i)
//   {
//     local_y[local_i] = 0.0;
//     for (j = 0; j < cols; ++j)
//     {
//       local_y[local_i] += local_A[local_i * cols + j] * x[j];
//     }
//   }
//   free(x);
// }

void mat_vect_mult(int local_A[], int n[], int local_y[], int local_rows, int cols, MPI_Comm comm)
{
  for (int i = 0; i < local_rows; ++i)
  {
    local_y[i] = 0;
    for (int j = 0; j < cols; ++j)
    {
      local_y[i] += local_A[i * local_rows + j] * n[j];
    }
  }
  free(x);
}

int main(void)
{
  int rows, cols;
  int *a, *n, *y, *local_a, *local_x, *local_y;
  int rank, size;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    FILE *file;
    file = fopen("mv-data.txt", "r");

    if (!file)
    {
      printf("Cannot open file\n");
      return 1;
    }

    fscanf(file, "%d", &rows);
    fscanf(file, "%d", &cols);

    a = malloc(sizeof(int) * rows * cols);
    n = malloc(cols * sizeof(int));
    y = malloc(sizeof(int) * cols);

    for (int i = 0; i < rows * cols; ++i)
    {
      fscanf(file, "%d", &a[i]);
    }

    for (int i = 0; i < cols; ++i)
    {
      fscanf(file, "%d", &n[i]);
    }
    MPI_Scatter(a, cols, MPI_INT, local_a, cols, MPI_INT, 0, comm);
  }

  mat_vect_mult(local_a, n, local_y, rows / size, cols, MPI_WORLD_COMM);

  if (rank == 0)
  {
    MPI_Gather(local_y, size, MPI_INT, y, size, MPI_INT, MPI_WORLD_COMM);
    for (int i = 0; i < size; ++i)
    {
      printf("%d ", y[i]);
    }
  }

  MPI_Finalize();
  return 0;
}