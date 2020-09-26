#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <sys/types.h> /* typedefs */
#include <sys/stat.h>

int main(void)
{
  int comm_size, my_rank;
  char output[100];
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  if (my_rank == 0)
  {
    FILE *fptr;
    char *contents;
    fptr = fopen("hello.html", "r");
    if (fptr)
    {
      fseek(fptr, 0, SEEK_END);
      long size = ftell(fptr);
      contents = (char *)calloc(size, sizeof(char));
      fseek(fptr, 0L, SEEK_SET);
      fread(contents, sizeof(char), size, fptr);
      fclose(fptr);
    }
    else
    {
      printf("No file\n");
    }

    for (int i = 1; i < comm_size; ++i)
    {
      MPI_Send((char *)contents, strlen(contents) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD); // send the message to the other threads
    }
  }
  else
  {
    MPI_Recv(&output, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d's Message\n", my_rank);
    printf("%s\n", output);
  }
  MPI_Finalize();
  return 0;
}
