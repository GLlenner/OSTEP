#include<stdio.h>
#include<unistd.h>



int main(){
    int pipefd[2];
    char buf[256];

    if(pipe(pipefd) == -1){
        perror("error");
    }
    int fk = fork();
    if(fk < 0){
        perror("error");
        return -1;
    }
    else if(fk == 0){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execlp("ls","ls","-l",NULL);
        perror("error");
        return -1;
    }

    int pid_wc = fork();
    if (pid_wc == 0) { 
        // 子进程：wc -l
        close(pipefd[1]);        // 关闭写端
        dup2(pipefd[0], STDIN_FILENO);   // 标准输入重定向到管道读端
        close(pipefd[0]);        // 关闭原始读端描述符

        execlp("wc", "wc", "-l", NULL);  // 执行命令
        perror("execlp wc failed");
        exit(-1);
    } else if (pid_wc < 0) {
        perror("fork wc failed");
        exit(-1);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(fk, NULL, 0);
    waitpid(pid_wc, NULL, 0);
    return 0;
}