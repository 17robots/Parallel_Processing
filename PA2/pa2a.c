#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct arg_struct
{
  int *arr;
  int start;
  int elems;
};

int *genArray(int num_nums, int low, int high)
{
  int *placedArray = malloc(sizeof(int) * num_nums);
  for (int i = 0; i < num_nums; ++i)
  {
    placedArray[i] = rand() % high + low;
  }
  return placedArray;
}

int factorial(int num)
{
  // int result = 1;
  // for (int i = 2; i <= num; result *= i, ++i)
  // {
  // }
  // return result;

  return num * 2;
}

void *doFactCalcOps(void *args)
{
  struct arg_struct *arguments = args;
  for (int i = 0; i < arguments->elems; ++i)
  {
    arguments->arr[arguments->start + i] = factorial(arguments->arr[arguments->start + i]);
  }
  pthread_exit(NULL);
  return (NULL);
}

int main()
{
  double totalTime, singleThreadTime, speedup, efficiency;
  struct timespec begin, end;
  srand(time(NULL));
  int low, high;
  printf("Enter Upper Bound\n");
  scanf("%d", &high);

  printf("Enter Lower Bound\n");
  scanf("%d", &low);

  struct arg_struct args;

  for (int arrLength = 1000; arrLength < 16001; arrLength *= 2)
  {
    singleThreadTime = 0;
    printf("\nArray Length: %d\n", arrLength);
    args.arr = genArray(arrLength, low, high);
    for (int threadCount = 1; threadCount < 17; threadCount *= 2)
    {
      totalTime = 0;
      printf("Thread Count: %d; ", threadCount);
      pthread_t threads[threadCount];
      args.elems = arrLength / threadCount;
      printf("Each thread will process %d element(s)\n", args.elems);
      for (int i = 0; i < threadCount; ++i)
      {
        args.start = args.elems * i;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        int rc = pthread_create(&threads[i], NULL, &doFactCalcOps, (void *)&args);
        clock_gettime(CLOCK_MONOTONIC, &end);
        totalTime += (end.tv_sec - begin.tv_sec);
        totalTime += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
      }
      if (threadCount == 1)
        singleThreadTime += totalTime;

      printf("Total Time Taken For %d thread(s): %f\n", threadCount, totalTime);
      speedup = singleThreadTime / totalTime;
      printf("Speedup from 1 to %d thread(s): %f / %f = %f\n", threadCount, singleThreadTime, totalTime, speedup);
      efficiency = speedup / threadCount;
      printf("Efficiency from 1 to %d thread(s): %f / %d = %f\n\n", threadCount, speedup, threadCount, efficiency);
      for (int i = 0; i < threadCount; ++i)
      {
        pthread_join(threads[i], NULL);
      }
    }
  }
  printf("\n");
  pthread_exit(NULL);
  return 0;
}