#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

#define NUM_WORKER_PROCESSES 4
#define NUM_THREADS_PER_PROCESS 4

int worker_pids[NUM_WORKER_PROCESSES];

void handle_sigchld(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0) {
        printf("Worker process %d terminated prematurely\n", pid);
        // Kill and clean up all worker processes
        for (int i = 0; i < NUM_WORKER_PROCESSES; i++) {
            if (worker_pids[i] > 0) {
                kill(worker_pids[i], SIGKILL);
            }
        }
        exit(1);
    }
}

void *worker_thread(void *arg) {
    // Worker thread code
    return NULL;
}

int main(void) {
    struct sigaction act;
    act.sa_handler = handle_sigchld;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &act, NULL);

    for (int i = 0; i < NUM_WORKER_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process (worker process)
            pthread_t threads[NUM_THREADS_PER_PROCESS];
            for (int j = 0; j < NUM_THREADS_PER_PROCESS; j++) {
                pthread_create(&threads[j], NULL, worker_thread, NULL);
            }
            for (int j = 0; j < NUM_THREADS_PER_PROCESS; j++) {
                pthread_join(threads[j], NULL);
            }
            exit(0);
        } else {
            // Parent process (controller)
            worker_pids[i] = pid;
        }
    }

    // Wait for all worker processes to complete
    for (int i = 0; i < NUM_WORKER_PROCESSES; i++) {
        waitpid(worker_pids[i], NULL, 0);
    }

    return 0;
}
