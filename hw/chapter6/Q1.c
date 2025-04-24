#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/time.h>
#define TESTIMES 1000000

int main(){
    struct timeval start,end;
    int fd = open("./sample.txt",O_RDONLY);
    gettimeofday(&start,NULL);
    for(int i = 0;i < TESTIMES;i++){
        read(fd,NULL,0);
    }
    gettimeofday(&end,NULL);
    close(fd);
    printf("系统调用时间为%f微秒",(end.tv_usec - start.tv_usec) *1.0 / TESTIMES);

}