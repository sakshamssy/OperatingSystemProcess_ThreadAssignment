#include <stdio.h>

int isPrime(int x)
{
    if (x<2){return 0;}
    for (int i=2;i*i<x;i++)
    {
        if(x%i==0)
        {
            return 0;
        }
    }
    return x;
}
int main() {
    int x = 5;

    if (isPrime(x)) {
      printf("%d is a prime number ",x);
    } else {
      printf("%d is not a prime number ",x);
    }
    
   return 0;
}