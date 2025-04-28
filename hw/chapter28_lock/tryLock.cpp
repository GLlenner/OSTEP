//（练习）  锁的基本使用

#include<pthread.h>
#include<iostream>
using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//或者另一种初始化方式 pthread_mutex_init(&mutex,nullptr)

int sum = 0;
void* addSum(void* para){
    for(int i = 0;i < 1000000;i++){
        pthread_mutex_lock(&mutex);
        sum++;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main(){

    pthread_t t0,t1;

    pthread_create(&t0,nullptr,addSum,nullptr);
    pthread_create(&t1,nullptr,addSum,nullptr);

    pthread_join(t0,nullptr);
    pthread_join(t1,nullptr);

    cout << "sum = " << sum << endl;
 
}




