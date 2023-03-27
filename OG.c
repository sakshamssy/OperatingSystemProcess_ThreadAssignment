#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
int wp=-1;
 
 
void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
    if (num == 0){
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}
 
 
int main()
{
    wp=getpid();
    int fd[2];
    char buf[1000];
    char snum[1000];
    pipe(fd);
    int n=10;
    if(getpid()==wp){
        printf("Parent Process\n");
        for(int i=0; i<n && getpid()==wp; i++){
            tostring(snum, i);
            write(fd[1],snum,1000);
            // printf("i: %d\n",i);
            fork();
 
            wait(NULL);
        }if(getpid()==wp){
            printf("Parent Done\n");
        }
        // }if(getpid()==wp){
        //     for(int i=0; i<n; i++){
        //         wait(NULL);
        //     }
        //     printf("Parent Done\n");
        // }
    }if(getpid()!=wp){
        int ch_no = read(fd[0],buf,1000);
        // int ch_no=(int)(buf[0]);
        //printf("%d\n",ch_no);
        int row_num = atoi(buf);
        printf("Child Process%d\n",row_num);
 
    }
    return 0;
}
