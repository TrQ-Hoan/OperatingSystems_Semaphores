#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define THREAD_NUM 1

sem_t mutex;
volatile int counter = 0;
const int N = 1000000;

void *child(void *arg) {
    int i;
    for (i = 0; i < N; i++) {
        sem_wait(&mutex);
        counter++;
        sem_post(&mutex);
    }
}

int main(int argc, char *argv[]) {
    sem_init(&mutex, 0, 1);
    pthread_t c1, c2;
    pthread_create(&c1, NULL, child, NULL);
    pthread_create(&c2, NULL, child, NULL);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);
    printf("Result: %d (should be %d)\n", counter, N*2);
    return 0;
}