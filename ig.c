// Ruchir Raj
// Rishabh Sahai
// Saksham Subhash Yadav
// Shoeb Ahmed Khan
// Aditya R. Goyal
// Sourabh Dilraj
// Vishnupriya Vijayan

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
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  ##__VA_ARGS__); \
}

#define PRINT_ERROR(MSG, ...) { \
	printf ( "%s ERROR %d:%d %ld %s %s %d : [%d] " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  errno, ##__VA_ARGS__);	\
	}
	
#define PRINT_ERR_EXIT(MSG, ...) { \
	printf ( "%s ERROR %d:%d %ld %s %s %d : [%d] " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  errno, ##__VA_ARGS__);	\
	_exit(-1); \
	}
	


int n, a, b, p;
pid_t *process;


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
    //clock_t start,end;
    //start=clock();
    
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
    for(int i=a-1;(i>=2)&&(up<limit);i--)
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
    PRINT_INFO("px has been created for %d", a);
    //printf("All prime number for %d found\n", a);
    int *ptr = px;
    for (int l = 0; l < (numIsPrime+up+down);l++) 
    {
        sum += *(px+l);
    }
    avg=sum/(numIsPrime+up+down);
   // end = clock();
   // printf("------------------------------------------------------Total time for %d is {%.10f} \n", a, (double)(end - start)/CLOCKS_PER_SEC);
    return avg;
}

void handle_sigint(int sig)
{

    PRINT_ERROR("SIGCHILD Caught.... Code {%d}", sig);
    for (int i = 0; i < n; i++)
    {
        kill(*(process + i), SIGKILL);
    }
    exit(-1);
}

void* ElementFunction(void* d)
{
    PRINT_INFO("Worker Thread Executing");
    //printf("Worker Thread Executing \n");
    int o = *(int*)d;
    int i = o;
    o = primeAvg(o, p);
    //printf("thapx calculated : {%d}\n", o);
    PRINT_INFO("thapx calculated : {%d} for {%d}", o, i);
    *(int*)d = o;
    return d;
}

int main(int argc, char *argv[]) {
    int numInput=argc;                      //argc-number of Input //argv -- 0,n,a,b,p,e,x1,x2,...........
    if (argc < 6) {
        PRINT_ERROR("Error: Not enough arguments");
        //printf("Error: Not enough arguments\n");
        return 1;
    }
    n = atoi(argv[1]);
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    p = atoi(argv[4]);
    
    int x[n][n]; 
    
    if (numInput!=n*n+5 || !(n>=4 && n<=10)){
        PRINT_INFO("Incorrect number of Inputs %d %d", n, numInput);
        //printf("Incorrect number of Inputs %d %d", n, numInput);
        return 1;
    }
    PRINT_INFO("Enough argument, Correct Input read successfully ");
    //printf("Enough argument, Correct Input read successfully \n");
    
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
        PRINT_ERR_EXIT("Error in creating pipe");
		// perror("Error in creating pipe\n");
		// exit(1);
	}
    PRINT_INFO("Pipe Created");
	//printf("\nPipe Created \n\n");
	write(pfds[1], &buf, sizeof(int));
	pid_t child_pid[n], wpid;
    process = &child_pid[0];
	signal(SIGINT, handle_sigint);
    int status = 0;
    int fapx = 0;
	for(int i = 0; i < n; i++)
    {
        child_pid[i] = fork();
        
        if(child_pid[i] == 0)
        {
            clock_t s,e;
            s=clock();
            PRINT_INFO("Child Process Created Row : {%d} ", i);
            //printf("Child Process Created Row : {%d} \n", i);
            for(int j = 0; j < n; j++)
            {
                if(x[i][j] < a || x[i][j] > b){
                    //printf("Value out of range for [{%d},{%d}] \n", i, j);
                    PRINT_ERR_EXIT("Value out of range for [{%d},{%d}] ", i, j);
                    //exit(3);
                    //break;
                }
            }
            
            {
                int wpapx = 0;
                pthread_t t[n];
                for(int j =0; j<n; j++)
                {
                    
                    if(pthread_create(&t[j], NULL, ElementFunction, (void*)&x[i][j]))
                    {
                        PRINT_ERR_EXIT("Error in pthread_create for %d", i);
                        // printf("Error in pthread_create for %d\n", i);
                        // exit(5);
                    }else{
                        PRINT_INFO("Worker Thread Created ");
                            //printf("Worker Thread Created \n");
                    }
                }
                
                
                for(int j = 0; j< n; j++){
                    int* a;
                    
                    if(pthread_join(t[j], (void **) &a) != 0)
                    {
                        PRINT_ERR_EXIT("Failed to join threads for row :{%d}", i);
                        // printf("Failed to join threads for row :{%d}", i);
                        // exit(2);
                    }
                }
                //printf("All threads have joined \n");
                PRINT_INFO("All threads have joined ");
                PRINT_INFO("All thapx calculated for row {%d} ", i);
        
                for(int j = 0; j < n; j++){
                    wpapx += x[i][j];
                }
                wpapx /= n;
                PRINT_INFO("wpapx for Row {%d} is {%d} ", i,wpapx);
                // printf("wpapx for Row {%d} is {%d} \n", i,wpapx);

                PRINT_INFO("wpapx calculated for Row {%d}", i);
                read(pfds[0], &buf, sizeof(int));
                wpapx += buf;
                write(pfds[1], &wpapx, sizeof(int));
                PRINT_INFO("wpapx written to the controller/pipe ");
                // printf("wpapx written to the pipe \n");
                e=clock();
                PRINT_INFO("Total time for Row {%d} = %.10f", i,(double)(e - s)/CLOCKS_PER_SEC);
                // printf("Total time for Row {%d} = %.10f\n", i,(double)(e - s)/CLOCKS_PER_SEC);
                pthread_exit(NULL);
                // printf("All threads exited for Row {%d} \n", i);
                return 0;
            }
        }

    }
    int valid = 1;
    while ((wpid = wait(&status)) > 0){
        if(WIFEXITED(status)){
            int statusCode = WEXITSTATUS(status);
            //printf("\n\n %d \n\n", statusCode);
            if(statusCode != 0){
                
                for(int i =0; i < n; i++)
                    kill(child_pid[i], SIGKILL);
                // while ((wpid = wait(&status)) > 0);

                //printf("\n\n Failure with error code : {%d} \n \n", statusCode);
                PRINT_ERR_EXIT(" Failure with error code : {%d} ", statusCode);
                return 0; 

            }
        }
    }
    printf("\n");
    if(valid == 1){
        PRINT_INFO("All thapx calculated");
        read(pfds[0], &buf, sizeof(int));
        PRINT_INFO("wpapx captured from the controller/pipe ");
        //printf("wpapx read from the pipe \n");
        buf /= n;
        PRINT_INFO("fapx : {%d} ", buf);
        //printf("Final Value : {%d} \n", buf);
    }
    return 0;
}
