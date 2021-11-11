#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define HUNGRY 0
#define EATING 1
#define THINKING 2

sem_t forks[5];
sem_t lock;
int state[5];

int left(int p) { return p; }

int right(int p) { return (p + 1) % 5; }

void get_forks(int p) {
    if (p == 4) {
        sem_wait(&forks[left(p)]);
        state[p] = HUNGRY;
        sem_wait(&forks[right(p)]);
    } else {
        sem_wait(&forks[right(p)]);
        state[p] = HUNGRY;
        sem_wait(&forks[left(p)]);
    }
}

void put_forks(int p) {
    sem_post(&forks[left(p)]);
    sem_post(&forks[right(p)]);
    state[p] = THINKING;
}

void think(int p) {
    state[p] = THINKING;
    sleep(1);
}

void eat(int p) {
    state[p] = EATING;
    sleep(rand() % 2 + 1);
}

void *philosopher(void *arg) {
    int p = (int)arg;
    int loops = 1000;
    while (loops--) {
        think(p);
        get_forks(p);
        eat(p);
        put_forks(p);
    }
}

void print_info() {
    sem_wait(&lock);
    for (int i = 0; i < 5; i++) {
        if (state[i] == EATING)
            printf("\033[1;31mE\033[1;0m ");
        else if (state[i] == THINKING)
            printf("\033[1;32mT\033[1;0m ");
        else
            printf("\033[1;37mH\033[1;0m ");
    }
    printf("\033[1;0m\n");
    sem_post(&lock);
}

void *info(void *arg) {
    int loops = 30;
    while (loops--) {
        sleep(1);
        print_info();
    }
    for (int i = 0; i < 5; i++) sem_destroy(&forks[i]);
    sem_destroy(&lock);
    printf("FINISHED\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    srand(time(0));
    printf("DINING PHILOSOPHER\n");
    int i;
    pthread_t p[5], inf_print;
    // init
    for (i = 0; i < 5; i++) sem_init(&forks[i], 0, 1);
    sem_init(&lock, 0, 1);
    // create
    for (i = 0; i < 5; i++) pthread_create(&p[i], NULL, philosopher, (void *)i);
    pthread_create(&inf_print, 0, info, 0);
    // join
    for (i = 0; i < 5; i++) pthread_join(p[i], 0);
    return 0;
}
