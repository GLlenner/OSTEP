//使用信号量，实现一写者多读者的读写者模型

#include<semaphore.h>
#include<iostream>
#include<pthread.h>
#include <unistd.h>
#include <random>
using namespace std;

typedef struct _rwlock_t { 
    sem_t lock; // binary semaphore (basic lock) 
    sem_t writelock; // used to allow ONE writer or MANY readers 
    int readers; // count of readers reading in critical section 
} rwlock_t; 

void rw_init(rwlock_t* rw){
    sem_init(&rw->lock,0,1);
    sem_init(&rw->writelock,0,1);
    rw->readers = 0;
}

void rwlock_acquire_readlock(rwlock_t* rw){
    sem_wait(&rw->lock);
    if(rw->readers == 0){
        sem_wait(&rw->writelock);
    }
    rw->readers++;
    sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw){
    sem_wait(&rw->lock);
    if(rw->readers == 1){
        sem_post(&rw->writelock);
    }
    rw->readers--;
    sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw){
    sem_wait(&rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw){ 
    sem_post(&rw->writelock); 
}

void* reader(void* argv){
    rwlock_t* rw = (rwlock_t*)argv;
    rwlock_acquire_readlock(rw);
    sem_wait(&rw->lock);
    std::cout << "还有读者 "  << rw->readers << " 名正在阅读"  << std::endl;
    sem_post(&rw->lock);
    rwlock_release_readlock(rw);
    return nullptr;
}

void* writer(void* argv){
    rwlock_t* rw = (rwlock_t*)argv;
    rwlock_acquire_writelock(rw);
    sem_wait(&rw->lock);
    std::cout << "写者开始写作" << std::endl;
    sem_post(&rw->lock);
    rwlock_release_writelock(rw);
    return nullptr;
}

int main(){
    rwlock_t rw;  // 直接在栈上分配内存
    rw_init(&rw);

    pthread_t readers[10000], writer_thread;

    // 创建 10 个读者线程
    for (int i = 0; i < 10000; i++) {
        pthread_create(&readers[i], nullptr, reader, (void*)&rw);
    }

    // 创建一个写者线程
    pthread_create(&writer_thread, nullptr, writer, (void*)&rw);

    // 等待所有读者线程结束
    for (int i = 0; i < 10; i++) {
        pthread_join(readers[i], nullptr);
    }

    // 等待写者线程结束
    pthread_join(writer_thread, nullptr);

    return 0;
}