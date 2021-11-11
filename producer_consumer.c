#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define THREAD_NUM 8

sem_t empty;
sem_t full;
sem_t mutex;

int buffer[10];
int count = 0;
int LOOPS = 18;

void* producer(void* args) {
    int p = (int)args;
    while (LOOPS-- > 0) {
        // Produce
        int x = rand() % 81 + 10;
        sleep(1);
        // Add to the buffer
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[count] = x;
        printf("%d  \033[1;32mFill\033[1;0m\t[%d]\t%d\n", p, count, x);
        count++;
        sem_post(&mutex);
        sem_post(&full);
    }
}

void* consumer(void* args) {
    int p = (int)args;
    while (1) {
        int y;
        // Remove from the buffer
        sem_wait(&full);
        sem_wait(&mutex);
        y = buffer[count - 1];
        printf("%d  \033[1;31mGot\033[1;0m\t[%d]\t%d\n", p, count - 1, y);
        count--;
        if (count < 0) {
            perror("Empty\n");
            exit(1);
        }
        if (count == 0 && LOOPS <= 0) {
            sem_destroy(&mutex);
            sem_destroy(&full);
            sem_destroy(&empty);
            puts("******* DONE *******");
            exit(0);
        }
        sem_post(&mutex);
        sem_post(&empty);
        // Consume
        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    sem_init(&empty, 0, 10);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    int idx_prod[THREAD_NUM] = {}, cnt_w;
    while (cnt_w != 4) {
        idx_prod[rand() % THREAD_NUM] = 1;
        cnt_w = 0;
        for (int i = 0; i < THREAD_NUM; ++i)
            if (idx_prod[i] == 1) cnt_w++;
    }

    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        // if (i < 5)
        // if (i % 2 == 0)
        if (idx_prod[i])
            pthread_create(&th[i], NULL, &producer, (void*)i);
        else
            pthread_create(&th[i], NULL, &consumer, (void*)i);
    }

    puts("* PRODUCER CONSUMER *");
    printf("id ---- index - value\n");
    for (i = 0; i < THREAD_NUM; i++) pthread_join(th[i], NULL);
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    return 0;
}
