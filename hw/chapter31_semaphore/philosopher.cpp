#include<semaphore.h>
#include<iostream>
#include<unistd.h>
using namespace std;

#define Num 2

sem_t myfork[Num];
pthread_mutex_t mutex;
void fork_init(){
    for(int i = 0;i < Num;i++){
        sem_init(&myfork[i],0,1);
    }
}
void getfork(int i){sem_wait(&myfork[i]);sleep(2);}
void eat(int i){pthread_mutex_lock(&mutex); cout << "哲学家" << i << "开始用餐" << endl;pthread_mutex_unlock(&mutex);}
void putfork(int i){sem_post(&myfork[i]);}
void think(int i){pthread_mutex_lock(&mutex);cout << "哲学家" << i << "开始思考" << endl;pthread_mutex_unlock(&mutex);}//sleep(3);}

void* philosopher_work(void* argv){
    int id = *(int*)argv; 
    for(int i = 0;i < 1 ;i++){
        getfork(id);
        getfork((id + 1)% Num);
        eat(id);
        putfork(id);
        putfork((id + 1)% Num);
        think(id);
    }
    return nullptr;
}

int main(){
    pthread_t philosopher[Num];
    fork_init();
    int t0=0,t1=1;
    pthread_create(&philosopher[0],nullptr,philosopher_work,&t0);
    pthread_create(&philosopher[1],nullptr,philosopher_work,&t1);
    for(int i = 0;i<Num;i++){
        pthread_join(philosopher[i],nullptr);
    }
}
