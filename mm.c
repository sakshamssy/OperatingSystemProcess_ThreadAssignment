#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
//Taking Input and Validation and Printing 2d array works

///trying to finalise this

#define PRINT_INFO(MSG, ...) { \
	printf ( "%s INFO %d:%d %ld %s %s %d : " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), _FILE, __FUNCTION_, \
	_LINE,  ##__VA_ARGS_); \
}

#define PRINT_ERROR(MSG, ...) { \
	printf ( "%s ERROR %d:%d %ld %s %s %d : [%d] " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), _FILE, __FUNCTION_, \
	_LINE,  errno, ##__VA_ARGS_);	\
	}
	
#define PRINT_ERR_EXIT(MSG, ...) { \
	printf ( "%s ERROR %d:%d %ld %s %s %d : [%d] " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), _FILE, __FUNCTION_, \
	_LINE,  errno, ##__VA_ARGS_);	\
	_exit(-1); \
	}

int n, a, b, p;


int isPrime(int p) //Checks whether a number is prime 
{
    if (p<2){return 0;}
    for (int i=2;i*i<p;i++)
    {
        if(p%i==0)
        {
            return 0;
        }
    }
    return 1;
}
int primeAvg(int a,int limit)
{
    int sum=0;
    int avg=0;
    int up=0;
    int down=0;
    int numIsPrime=0;
    int actlimit=(2*limit+1);
    int px[actlimit];
    if(isPrime(a)==1)
    {
        px[0]=a;
        numIsPrime++;
    }
    for(int i=a-1;(i<=2)||(up<limit);i--)
    {
        if(isPrime(i)==1)
        {
           px[numIsPrime+up]=i;
            up++;
        }
    }
    for(int i=a+1;(down<limit);i++)
    {
        if(isPrime(i)==1)
        {
           px[numIsPrime+up+down]=i;
            down++;
        }
    
    }
    printf("All prime number for %d found", a);
    int *ptr = px;
    for (int l = 0; l < (numIsPrime+up+down);l++) 
    {
        sum += *(px+l);
    }
    avg=sum/(numIsPrime+up+down);
    return avg;
}

void* ElementFunction(void* d)
{
    printf("Worker Thread Executing \n");
    int o = *(int*)d;
    o = primeAvg(o, p);
    printf("thapx calculated : {%d}\n", o);

    *(int*)d = o;
    return d;
}

int main(int argc, char *argv[]) {
    int numInput=argc;                      //argc-number of Input //argv -- 0,n,a,b,p,e,x1,x2,...........
    n = atoi(argv[1]);
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    p = atoi(argv[4]);
    
    int x[n][n]; 
    if (argc < 6) {
        printf("Error: Not enough arguments\n");
        return 1;
    }
    if (numInput!=n*n+5 || !(n>=4 && n<=10)){
        printf("Invalid Input-1 %d %d", n, numInput);
        return 1;
    }
    printf("Enough argument, input read \n");
    
    for(int i = 0; i <n; i++)
        for(int j =0; j<n; j++)
            x[i][j] = atoi(argv[5 + n * i + j]);
            
    for(int i = 0; i <n; i++)
    {
        for(int j =0; j<n; j++)
            printf("%d ", x[i][j]);
        printf("\n");
        
    }
    
    int pfds[2]; //pipe file descriptors
	int buf = 0;
	
	if(pipe(pfds)==-1)
	{
		perror("Error in creating pipe\n");
		exit(1);
	}
	
	write(pfds[1], &buf, sizeof(int));
	pid_t child_pid, wpid;
	
    int status = 0;
    int fapx = 0;
	for(int i = 0; i < n; i++)
    {
        child_pid = fork();
        
        if(child_pid == 0)
        {
            printf("Child Process Created Row : {%d} \n", i);

            int wpapx = 0;
            pthread_t t[n];
            for(int j =0; j<n; j++)
            {
                int* a = malloc(sizeof(int));
                *a = x[i][j];
                if(pthread_create(&t[j], NULL, ElementFunction, (void*)a))
                {
                    printf("Error in pthread_create for %d\n", i);
                    exit(-1);
                }else{
                        printf("Worker Thread Created \n");
                }
            }
            
            
            for(int j = 0; j< n; j++){
                int* a;
                
                if(pthread_join(t[j], (void **) &a) != 0)
                {
                    printf("Failed to join threads for row :{%d}", i);
                    exit(0);
                }
                wpapx += *(int*)a;
                free(a);
            }
            printf("All threads have joined \n");
            
            wpapx /= n;
            printf("thapx for Row {%d} is {%d} \n", i,wpapx);
            read(pfds[0], &buf, sizeof(int));
            wpapx += buf;
            write(pfds[1], &wpapx, sizeof(int));
            pthread_exit(NULL);
        }

    }
    while ((wpid = wait(&status)) > 0);
    printf("\n");
    read(pfds[0], &buf, sizeof(int));
    printf("Final Value : {%d}", buf);
}
