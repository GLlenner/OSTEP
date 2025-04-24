#include<stdio.h>
#include<pthread.h>
#include<stdbool.h>
#include<unistd.h>

bool flag[2] = {false, false};
int turn;
int sum = 0;
void* thread_func(void* arg) {
    int id = *(int*)arg;
    int other = 1 - id;
    int n = 1000;
    while (n-- > 0 ) {
        // 想进入临界区
        flag[id] = true;
        turn = other;

        while (flag[other] && turn == other) {
            // 等待
        }

        // ---- 临界区开始 ----
        printf("线程%d进入临界区\n", id);
        sum ++ ;
        printf("%d\n",sum); // 模拟干活
        printf("线程%d离开临界区\n", id);
        // ---- 临界区结束 ----

        flag[id] = false; // 模拟做别的事
    }
}

int main() {
    pthread_t t0, t1;
    int id0 = 0, id1 = 1;

    pthread_create(&t0, NULL, thread_func, &id0);
    pthread_create(&t1, NULL, thread_func, &id1);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    return 0;
}
