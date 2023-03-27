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
    return p;
}
int variadic_average (int count,...)
{
  va_list args;
  int i, sum,avg;

  va_start (args, count);         /* Save arguments in list. */
  sum = 0;
  for (i = 0; i < count; i++)
    sum += va_arg (args, int);    /* Get the next argument value. */
avg=sum/count;

  va_end (args);                  /* Stop traversal. */
  return sum;
}

int main(int argc, char *argv[]) {
    int numInput=argc;                      //argc-number of Input //argv -- 0,n,a,b,p,e,x1,x2,...........
    n = atoi(argv[1]);
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    p = atoi(argv[4]);
    // PRINT_INFO("TAKING INPUT");
    printf("%d ", numInput);
    if (argc < 6) {
        printf("Error: Not enough arguments\n");
        return 1;
    }
    if (numInput!=n*n+5 || !(n>=4 && n<=10) || (n>=4 && n<=10) ){
        printf("Invalid Input");
        return 1;
    }
 printf("Enough argument\n");


     printf("%d,%d,%d,%d",n,a,b,p);

    int index = 5;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            x[i][j] = atoi(argv[index++]);
            if (!(x[i][j]>=a && x[i][j]<=b)){
                    printf("Invalid Input");
                    return 1;
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
