//实现环形缓存区，注意缓冲区为空和满时的条件

#include<pthread.h>
#include<assert.h>
#include<iostream>
#include<unistd.h>
using namespace std;

#define BUFSIZE 3

int buf[BUFSIZE];
int head = 0,tail = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cons_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t prod_cond = PTHREAD_COND_INITIALIZER;

void product(int i,int id){
    assert((head + 1) % BUFSIZE != tail);
    cout << "生产者 "  << id << " 生产" <<endl;
    buf[head] = i;
    head = (head + 1) % BUFSIZE;
}

int consume(int id){
    assert(tail != head);
    cout << "消费者 "  << id <<" 消费" <<endl;
    tail = (tail + 1) % BUFSIZE;
    return buf[tail];
}

void* producter(void* argv){
    int id = *(int*)argv;
    for(int i = 0;i < 10;i++){
        //sleep(1);
        pthread_mutex_lock(&mutex);
        while((head + 1) % BUFSIZE == tail){
            pthread_cond_wait(&prod_cond,&mutex);
        }
        product(520,id);
        pthread_cond_signal(&cons_cond);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

void* consumer(void* argv){
    int id = *(int*)argv;
    for(int i = 0;i < 20;i++){
        pthread_mutex_lock(&mutex);
        while(tail == head){
            pthread_cond_wait(&cons_cond,&mutex);
        }
        consume(id);
        pthread_cond_signal(&prod_cond);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}


int main(){
    pthread_t prod1,prod2,cons1,cons2;
    int pt1id = 1,pt2id = 2,st1id = 1,st2id = 2;

    pthread_create(&prod1,nullptr,producter,(void*)(&pt1id));
    pthread_create(&prod2,nullptr,producter,(void*)(&pt2id));
    pthread_create(&cons1,nullptr,consumer,(void*)(&st1id));
    //pthread_create(&cons2,nullptr,consumer,(void*)(&st2id));

    pthread_join(prod1,nullptr);
    pthread_join(prod2,nullptr);
    pthread_join(cons1,nullptr);
    pthread_join(cons2,nullptr);

    return 0; 
}