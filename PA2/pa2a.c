#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// we need to go for each number of threads 1, 2, 4, 8 and then we need to go from threads 1000, 2000, 4000, 8000, 16000

void *calculateFactorial(int numElems, int startIndex, int *numArr)
{
}

int factorial(int n)
{
  int result = 1;
  for (int i = 2; i <= n; result *= i, ++i)
  {
  }
  return result;
}

int *generateArray(int low, int high, int nums)
{
  int *returnedArr;
  srand(time(NULL));
  for (int i = 0; i < nums; ++i)
  {
    returnedArr[i] = rand() % high + low;
  }
  return returnedArr;
}

int main()
{
  int for (int threadCount = 1; threadCount < 8; threadCount *= 2)
  {
    // set up the threads
    pthread_t threads[threadCount];
    pthread_attr_t attr;

    for (int arrLength = 1000; arrLength < 16001; arrLength *= 2)
    {
      int *randArr =
    }

    // clear attributes
    pthread_attr_destroy(&attr);

    // kill all threads
    for (int i = 0; i < threadCount; ++i)
    {
      pthread_join(&threads[i], NULL);
    }
  }
  pthread_exit(NULL);
  return 0;
}