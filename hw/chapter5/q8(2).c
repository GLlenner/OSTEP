#include<stdio.h>
#include<unistd.h>


int main(){
    int pipefd[2];
    pipe(pipefd);
    char buf[256];

    int fk = fork();
    if(fk < 0){
        perror("error");
        return -1;
    }
    else if(fk == 0){
        close(pipefd[0]);
        write(pipefd[1],"Hello",sizeof("Hello"));
        close(pipefd[1]);
    }
    else{
        close(pipefd[1]);
        read(pipefd[0],buf,sizeof(buf)-1);
        printf("i recvice your message:\n");
        printf("%s\n",buf);
        close(pipefd[0]);
    }
    return 0;
}