// https://www.geeksforgeeks.org/find-largest-among-three-different-positive-numbers-using-command-line-argument/

// C program for finding the largest integer
// among three numbers using command line arguments
#include <stdio.h>
#include <stdlib.h>
 
// Taking argument as command line
//int main(int argc, char *argv[])
int main(int argc,argv)
{
    int n,a,b,p;
    numInput=argv[0];
    n =(argv[1]);
    a =(argv[2]);
    b =(argv[3]);
    p = (argv[4]);


    // Checking if number of argument is
    // equal to 4 or not.
    if (numInput!=n*n+4)
    {
        printf("Invalid Input");
        return 0;
    }
 
    
    return 0;
}