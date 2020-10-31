#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>

int sum = 0;
pthread_mutex_t mutex;
sem_t semaphore;
pthread_rwlock_t lock_rw = PTHREAD_RWLOCK_INITIALIZER;

void *sumNoProtect()
{
}

void *sumBusyWait(void *rank)
{
}

void *sumMutex()
{
}

void *sumSem()
{
}

void *sumWRL()
{
}

int *fillArr(int totalItems)
{
  int *arr = malloc(sizeof(int) * totalItems);
  for (int i = 0; i < totalItems; ++i)
  {
    arr[i] = 1;
  }
  return arr;
}

void doSumNoProtect(bool fileOut, int *ints)
{
}

void doSumBusyWait(bool fileOut)
{
}

void doSumMutex(bool fileOut)
{
}

void doSumSem(bool fileOut)
{
}

void doSumWRL(bool fileOut)
{
}

void printHelp()
{
  printf("Help Menu\n");
  printf("\nUsage:\n");
  printf("./pa2b [option]\n");
  printf("\nOptions:\n");
  printf("No option - run timing for all locks and output to performance2.txt\n");
  printf("1 - run using no protection\n");
  printf("2 - run using busy-waiting protection\n");
  printf("3 - run using mutex protection\n");
  printf("4 - run using semaphore protection\n");
  printf("5 - run using read-write-lock protection\n");
}

int main(int argc, char **argv)
{
  if (argc > 2)
  {
    printf("Invalid usage. Use ./pa2b help for details\n");
    return -1;
  }

  if (argc == 1)
  {
    // calculate the performance of doing all security methods
    printf("Here is where we run all methods and print the time to performance2.txt\n");
  }
  else
  {
    // we have 1 extra arg that we need to process
    if (strcmp(argv[1], "1") == 0)
    {
      printf("1 was chosen\n");
    }
    else if (strcmp(argv[1], "2") == 0)
    {
      printf("2 was chosen\n");
    }
    else if (strcmp(argv[1], "3") == 0)
    {
      printf("3 was chosen\n");
    }
    else if (strcmp(argv[1], "4") == 0)
    {
      printf("4 was chosen\n");
    }
    else if (strcmp(argv[1], "5") == 0)
    {
      printf("5 was chosen\n");
    }
    else if (strcmp(argv[1], "help") == 0)
    {
      printHelp();
    }
    else
    {
      printf("Invalid Option. Run ./pa2b help to get a list of options\n");
    }
  }
  return 0;
}