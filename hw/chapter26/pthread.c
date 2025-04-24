#include<pthread.h>
#include<stdio.h>

int sum = 0;
void* add(){
    int n = 1000000;
    while(n-- > 0){
        sum++;
    }
    printf("%d\n",sum);
}


int main(){
    pthread_t t1,t2;

    int rs1,rs2;
    rs1 = pthread_create(&t1, NULL, add, NULL);
    rs2 = pthread_create(&t2, NULL, add, NULL);

    rs1 = pthread_join(t1,NULL);
    rs2 = pthread_join(t2,NULL);

    printf("------%d\n",sum);
    return 0;
}