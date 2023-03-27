#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>
#include <CSF372.h>

#define MAX_THREADS 100

int n, a, b, p;
int x[100][100];
int thapx[100];

void *worker_thread(void *arg) {
    int *val = (int *)arg;
    int t = pow(*val, p);
    thapx[*val] = t;
    pthread_exit(0);
}

void worker_process(int i) {
    int row = i;
    pthread_t worker_threads[MAX_THREADS];
    for (int j = 0; j < n; j++) {
        int *val = malloc(sizeof(*val));
        *val = x[row][j];
        pthread_create(&worker_threads[j], NULL, worker_thread, val);
    }

    for (int j = 0; j < n; j++) {
        pthread_join(worker_threads[j], NULL);
    }

    int wpapx = 0;
    for (int j = 0; j < n; j++) {
        wpapx += thapx[j];
    }
    wpapx /= n;

    write(1, &wpapx, sizeof(wpapx));
    close(1);
    exit(0);
}

void sigchld_handler(int signo) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {
        printf("Error: child process terminated prematurely\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 6) {
        printf("Error: Not enough arguments\n");
        return 1;
    }
    numInput=atoi(argv[0]);
    n = atoi(argv[1]);
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    p = atoi(argv[4]);

    int index = 5;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            x[i][j] = atoi(argv[index++]);
        }
    }

    int fd[2];
    pipe(fd);

    signal(SIGCHLD, sigchld_handler);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            close(1);
            dup(fd[1]);
            worker_process(i);
        }
    }

    close(fd[1]);

    int wpapx_sum = 0;
    int wpapx;
    for (int i = 0; i < n; i++) {
        read(fd[


