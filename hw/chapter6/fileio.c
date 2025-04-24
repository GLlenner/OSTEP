#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>


int main(){
    FILE* fp = fopen("./sample.txt","r");
    char buf[64];
    fgets(buf,sizeof(buf),fp);
    printf("%s",buf);
    int n = fread(buf, 1, sizeof(buf), fp);
    printf("%s\n",buf);
    printf("%d\n",n);
    fclose(fp);

    int fd = open("./sample.txt",O_RDONLY);
    int m = read(fd, buf, sizeof(buf));
    buf[m] = '\0'; // 补上字符串结束符
    printf("%s\n", buf);
    return 0;
}