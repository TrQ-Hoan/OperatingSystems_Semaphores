#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define THREAD_NUM 12

typedef struct _rwlock_t {
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;

void rwlock_init(rwlock_t *lock) {
    lock->readers = 0;
    sem_init(&lock->lock, 0, 1);
    sem_init(&lock->writelock, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *lock) {
    sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1) sem_wait(&lock->writelock);
    sem_post(&lock->lock);
}

void rwlock_release_readlock(rwlock_t *lock) {
    sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0) sem_post(&lock->writelock);
    sem_post(&lock->lock);
}

void rwlock_acquire_writelock(rwlock_t *lock) { sem_wait(&lock->writelock); }

void rwlock_release_writelock(rwlock_t *lock) { sem_post(&lock->writelock); }

void rwlock_detroy(rwlock_t *lock) {
    sem_destroy(&lock->lock);
    sem_destroy(&lock->writelock);
}

rwlock_t mutex;
int memory;

void *writer(void *arg) {
    rwlock_acquire_writelock(&mutex);
    memory = rand() % 90 + 10;
    printf("\033[1;32mWriter %2d\033[1;0m: modified to %d\n", (int)arg, memory);
    rwlock_release_writelock(&mutex);
}

void *reader(void *arg) {
    rwlock_acquire_readlock(&mutex);
    printf("\033[1;31mReader %2d\033[1;0m: read as %d\n", (int)arg, memory);
    rwlock_release_readlock(&mutex);
}

int main() {
    pthread_t th[THREAD_NUM];
    rwlock_init(&mutex);

    srand(time(NULL));
    int idx_writer[THREAD_NUM] = {};
    int cnt_w, num_w = round(THREAD_NUM * (1.0 / 3.0));

    while (cnt_w != num_w) {
        idx_writer[rand() % THREAD_NUM] = 1;
        cnt_w = 0;
        for (int i = 0; i < THREAD_NUM; ++i)
            if (idx_writer[i] == 1) cnt_w++;
    }
    puts("READERS WRITERS");
    //
    for (int i = 0; i < THREAD_NUM; i++) {
        if (idx_writer[i]) {
            pthread_create(&th[i], NULL, &writer, (void *)i);
        } else {
            pthread_create(&th[i], NULL, &reader, (void *)i);
        }
    }
    //
    for (int i = 0; i < THREAD_NUM; i++) pthread_join(th[i], NULL);
    //
    rwlock_detroy(&mutex);
    return 0;
}