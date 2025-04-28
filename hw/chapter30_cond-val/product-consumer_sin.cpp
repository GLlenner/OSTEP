// 实现单生产者，单消费者，单缓存区
// 主要在于如何传递条件变量，只用单一条件变量即可，生产者传给消费者，消费者传给生产者
// 无法对多生产消费的情况复用，多生产消费，见后面的程序实现

#include<pthread.h>
#include<assert.h>
#include<iostream>
#include<unistd.h>
using namespace std;

int buf;
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void product(int i){
    assert(count == 0);
    cout << "生产" <<endl;
    count = 1;
    buf = i;
}

int consume(){
    assert(count == 1);
    cout << "消费" <<endl;
    count = 0;
    return buf;
}

void* producter(void* agrv){
    while(true){
        sleep(1);
        pthread_mutex_lock(&mutex);
        while(count == 1){
            pthread_cond_wait(&cond,&mutex);
        }
        product(520);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

void* consumer(void* argv){
    while(true){
        pthread_mutex_lock(&mutex);
        while(count == 0){
            pthread_cond_wait(&cond,&mutex);
        }
        consume();
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}


int main(){
    pthread_t prod,cons;

    pthread_create(&prod,nullptr,producter,nullptr);
    pthread_create(&cons,nullptr,consumer,nullptr);

    pthread_join(prod,nullptr);
    pthread_join(cons,nullptr);

    return 0; 
}
