//如何实现一个锁, Peterson 算法
#include<pthread.h>
#include<iostream>
#include<random>
#include<unistd.h>
using namespace std;

volatile int flag[2] = {0,0};
volatile int turn = 0;
volatile int sum = 0;

void lock(int self){
    flag[self] = 1;
    turn = 1 - self;
    while(turn == 1-self && flag[1 - self]);
}

void unlock(int self){
    flag[self] = 0;
}

void sleepSomeTime(){
    random_device rc;
    mt19937 gen(rc());
    uniform_int_distribution<> time(1,10);
    sleep(time(gen));
}

void* run(void* para){
    int self = *(int*)para;
    for(int i = 0;i < 1000000 ;i ++ ){
        lock(self);
        sum++;
        //cout << "thread" << self << " want to wc" << endl;
        unlock(self);
        //sleepSomeTime();
    }
    return nullptr;
}

int main(){
    pthread_t t0,t1;
    int id0 = 0, id1 = 1;

    pthread_create(&t0,nullptr,run,&id0);
    pthread_create(&t1,nullptr,run,&id1);

    pthread_join(t0,nullptr);
    pthread_join(t1,nullptr);

    cout << "sum = " << sum << endl;
    //实际打印结果并不是 2000000
    //原因：由于编译器优化或 CPU 的乱序执行等，也可能出现“内存可见性”问题或“编译器未能遵守 volatile 语义”。
    return 0;
}

//解决方法：在参数前加volatile
//volatile 是一个关键字，它的作用是告诉编译器不对这个变量进行优化
//既不缓存变量的值，而是每次都从内存中重新读取它。
//这样可以确保程序每次操作这个变量时，它的值都是最新的，而不会使用可能已经过时的缓存值。