#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

int Sum = 0;
int *numberArray;
int allowedRank;
sem_t semaphore;
pthread_rwlock_t lock_rw = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct args
{
  int start;
  int partitionAmount;
  int rank;
  int *arr;
};

void *sumNoProtect(void *arguments)
{
  struct args *arg = arguments;
  for (int i = arg->start; i < arg->partitionAmount; ++i)
  {
    Sum += arg->arr[i];
  }
  pthread_exit(NULL);
}

void *sumBusyWait(void *arguments)
{
  struct args *arg = arguments;
  for (int i = arg->start; i < arg->partitionAmount; ++i)
  {
    while (allowedRank != arg->rank)
    {
    }
    Sum += arg->arr[i];
  }
  allowedRank++;
}

void *sumMutex(void *arguments)
{
  struct args *arg = arguments;
  pthread_mutex_lock(&mutex);
  for (int i = arg->start; i < arg->partitionAmount; ++i)
  {
    Sum += arg->arr[i];
  }
  pthread_mutex_unlock(&mutex);
}

void *sumSem(void *arguments)
{
  struct args *arg = arguments;
  sem_wait(&semaphore);
  for (int i = arg->start; i < arg->partitionAmount; ++i)
  {
    Sum += arg->arr[i];
  }
  sem_post(&semaphore);
}

void *sumWRL(void *arguments)
{
  struct args *arg = arguments;
  if (pthread_rwlock_wrlock(&lock_rw))
  {
    printf("Error with locking\n");
  }
  for (int i = arg->start; i < arg->partitionAmount; ++i)
  {
    Sum += arg->arr[i];
  }
  if (pthread_rwlock_unlock(&lock_rw))
  {
    printf("Error with unlocking\n");
  }
  pthread_exit(NULL);
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

void runOp(void *(*function)(void *), int fileout, char *method)
{
  FILE *fp;
  double totalTime, singleThreadTime, speedup, efficiency;
  struct timespec begin, end;

  if (fileout)
    fp = fopen("performance2.txt", "a");
  else
    fp = stdout;

  fprintf(fp, "Protection Method: %s\n", method);
  for (int arrLength = 1000; arrLength < 16001; arrLength *= 2)
  {
    numberArray = fillArr(arrLength);
    singleThreadTime = 0;
    fprintf(fp, "\nArray Length: %d\n", arrLength);
    for (int threadCount = 1; threadCount < 9; threadCount *= 2)
    {
      totalTime = 0;
      fprintf(fp, "Thread Count: %d; ", threadCount);
      pthread_t threads[threadCount];
      fprintf(fp, "Each thread will process %d element(s)\n", arrLength / threadCount);
      allowedRank = 0;
      for (int i = 0; i < threadCount; ++i)
      {
        struct args arg;
        arg.arr = numberArray;
        arg.partitionAmount = arrLength / threadCount;
        arg.start = arg.partitionAmount * i;
        arg.rank = i;
        clock_gettime(CLOCK_MONOTONIC, &begin);
        int rc = pthread_create(&threads[i], NULL, function, (void *)&arg);
        clock_gettime(CLOCK_MONOTONIC, &end);
        totalTime += (end.tv_sec - begin.tv_sec);
        totalTime += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
      }

      if (threadCount == 1)
        singleThreadTime += totalTime;

      // close threads
      for (int i = 0; i < threadCount; ++i)
      {
        pthread_join(threads[i], NULL);
      }

      fprintf(fp, "Sum: %d\n", Sum);
      fprintf(fp, "Total Time Taken For %d thread(s): %f\n", threadCount, totalTime);
      speedup = singleThreadTime / totalTime;
      fprintf(fp, "Speedup from 1 to %d thread(s): %f / %f = %f\n", threadCount, singleThreadTime, totalTime, speedup);
      efficiency = speedup / threadCount;
      fprintf(fp, "Efficiency from 1 to %d thread(s): %f / %d = %f\n\n", threadCount, speedup, threadCount, efficiency);
    }
    Sum = 0;
  }
  fprintf(fp, "\n");
  fclose(fp);
}

void clearFile(char *filename)
{
  FILE *fp = fopen(filename, "w");
  if (!fp)
  {
    printf("Error Opening File\n");
    exit(1);
  }
  fclose(fp);
}

void printHelp()
{
  printf("PA2 Help\n");
  printf("Usage: ./pa2b [OPTION]\n");
  printf("OPTIONS\n");
  printf("1 - run with no protection methods\n");
  printf("2 - run with busy-wait protection\n");
  printf("3 - run with mutex protection\n");
  printf("4 - run with semaphore protection\n");
  printf("5 - run with read/write lock protection\n");
  printf("a - run all protection modes and then output to performance2.txt\n");
  printf("h - prints the help menu\n");
}
int main(int argc, char **argv)
{
  int all = 0;
  char *method;
  if (argc != 2 || argv[1] == "")
  {
    printf("Invalid Number Of Arguments. Please Run ./pa2b h for a list of arguments\n");
    return 0;
  }

  void *(*function)(void *);
  switch (argv[1][0])
  {
  case '1':
    function = sumNoProtect;
    method = "No Protection";
    break;
  case '2':
    function = sumBusyWait;
    method = "Busy Wait";
    break;
  case '3':
    function = sumMutex;
    method = "Mutex";
    break;
  case '4':
    function = sumSem;
    method = "Semaphore";
    break;
  case '5':
    function = sumWRL;
    method = "Read/Write Lock";
    break;
  case 'h':
    printHelp();
    return 0;
    break;
  case 'a':
    all = 1;
    break;
  default:
    printf("Invalid Option. Please Run ./pa2b h for a list of arguments\n");
    return 0;
  }

  // deal with semaphore
  sem_init(&semaphore, 0, 1);
  if (all)
  {
    clearFile("performance2.txt");
    runOp(sumNoProtect, all, "No Protection");
    runOp(sumBusyWait, all, "Busy Wait");
    runOp(sumMutex, all, "Mutex");
    runOp(sumSem, all, "Semaphore");
    runOp(sumWRL, all, "Read/Write Lock");
    FILE *fp = fopen("performance2.txt", "a");
    fprintf(fp, "Not Scalable\n");
    fclose(fp);
  }
  else
  {
    runOp(function, all, method);
  }
  sem_destroy(&semaphore);
  pthread_exit(NULL);
  return 0;
}