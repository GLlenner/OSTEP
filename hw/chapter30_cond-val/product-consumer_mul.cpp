// 实现多生产者，多消费者，单缓存区
// 主要在于如何传递条件变量，生产者传给消费者，消费者传给生产者
// 与单生成者，单消费者不同，条件变量的传递需要两个条件变量，不然可能会使线程全部睡眠

#include<pthread.h>
#include<assert.h>
#include<iostream>
#include<unistd.h>
using namespace std;

int buf;
int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cons_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t prod_cond = PTHREAD_COND_INITIALIZER;

void product(int i){
    assert(count == 0);
    cout << "生产者生产" <<endl;
    count = 1;
    buf = i;
}

int consume(int id){
    assert(count == 1);
    cout << "消费者 "  << id <<" 消费" <<endl;
    count = 0;
    return buf;
}

void* producter(void* agrv){
    while(true){
        // sleep(1);
        pthread_mutex_lock(&mutex);
        while(count == 1){
            pthread_cond_wait(&prod_cond,&mutex);
        }
        product(520);
        pthread_cond_signal(&cons_cond);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

void* consumer(void* argv){
    int id = *(int*)argv;
    while(true){
        pthread_mutex_lock(&mutex);
        while(count == 0){
            pthread_cond_wait(&cons_cond,&mutex);
        }
        consume(id);
        pthread_cond_signal(&prod_cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return nullptr;
}


int main(){
    pthread_t prod,cons1,cons2;
    int t1id = 1;
    int t2id = 2;
    pthread_create(&prod,nullptr,producter,nullptr);
    pthread_create(&cons1,nullptr,consumer,(void*)(&t1id));
    pthread_create(&cons2,nullptr,consumer,(void*)(&t2id));

    pthread_join(prod,nullptr);
    pthread_join(cons1,nullptr);
    pthread_join(cons2,nullptr);

    return 0; 
}
