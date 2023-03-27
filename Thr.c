#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_WORKER_PROCESSES 4
#define NUM_THREADS_PER_PROCESS 4

void *worker_thread_function(void *arg) {
  // Perform some task in this thread
  printf("Worker thread with ID %ld is running\n", (long)arg);

  // Terminate the thread
  pthread_exit(NULL);
}

int main(void) {
  pthread_t worker_threads[NUM_WORKER_PROCESSES][NUM_THREADS_PER_PROCESS];
  int rc;
  long t;

  // Create the worker processes
  for (int p = 0; p < NUM_WORKER_PROCESSES; p++) {
    // Create the threads for each worker process
    for (t = 0; t < NUM_THREADS_PER_PROCESS; t++) {
      printf("Creating worker thread %ld in worker process %d\n", t, p);
      rc = pthread_create(&worker_threads[p][t], NULL, worker_thread_function, (void *)t);
      if (rc) {
        printf("Error: unable to create thread, error code is %d\n", rc);
        exit(-1);
      }
    }
  }

  // Wait for all worker threads to finish
  for (int p = 0; p < NUM_WORKER_PROCESSES; p++) {
    for (t = 0; t < NUM_THREADS_PER_PROCESS; t++) {
      rc = pthread_join(worker_threads[p][t], NULL);
      if (rc) {
        printf("Error: unable to join thread, error code is %d\n", rc);
        exit(-1);
      }
    }
  }

  // All worker threads have finished
  printf("Controller: all worker threads have finished\n");
  pthread_exit(NULL);
}