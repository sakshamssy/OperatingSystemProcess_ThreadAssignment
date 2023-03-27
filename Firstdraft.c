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

int x[100][100]; 
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
    int *ptr = px;
    for (int l = 0; l < (numIsPrime+up+down);l++) 
    {
        sum += *(px+l);
    }
    avg=sum/(numIsPrime+up+down);
    printf("Sum=%d ",sum);
    printf("Avg=%d ",avg);
    return avg;
    // for(int j=0;j<=actlimit;j++) //FOr Printing the 2d Matrix
    // {
    //     printf("%d ",px[j]);
    // }
}


int main(int argc, char *argv[]) {
    int numInput=argc;                      //argc-number of Input //argv -- 0,n,a,b,p,e,x1,x2,...........
    n = atoi(argv[1]);
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    p = atoi(argv[4]);
    // printf("n %d\n",n);
    // printf("a %d \n ",a);
    // printf("b %d \n ",b);
    // printf("p %d \n ",p);
    // PRINT_INFO("TAKING INPUT");
    // printf("%d ", numInput);
    //Parent Process validation
    if (argc < 6) {
        printf("Error: Not enough arguments\n");
        return 1;
    }
    if (numInput!=n*n+5 || !(n>=4 && n<=10)){
        printf("Invalid Input-1");
        return 1;
    }
    printf("Enough argument \n");
    int index=5;
    int thapx;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) { 
            //Child Process Validation
            x[i][j] = atoi(argv[index++]);
            if (!(x[i][j]>=a && x[i][j]<=b)){
                    printf("Invalid Input-2");
                    return 1;
                }
            else{
               thapx=primeAvg(x[i][j],p);
            //    printf(" thapx=%d \n ",thapx);
               printf("thapx of x[%d][%d] = %d \n",i,j,thapx);
               thapx=0;
            }
            
        }
    }


// for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//            int z=x[i][j];
//                 if (isPrime(z)) {
//                     printf("%d is a prime number ",z);
//                 }
//                 else {
//       printf("%d is not a prime number ",z);
//     }
//         }
//         printf("\n");
//     }



}
