#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t s;

void *child(void *arg) {
    sem_wait(&s);
    printf("child %d\n", arg);
    sleep(5);
    sem_post(&s);
    return NULL;
}

int main(int argc, char *argv[]) {
    int num_threads = 10;
    int sem_value = 3;

    sem_init(&s, 0, sem_value);

    printf("parent: begin\n");
    pthread_t c[num_threads];

    int i;
    for (i = 0; i < num_threads; i++)
        pthread_create(&c[i], NULL, child, (void *)i);

    for (i = 0; i < num_threads; i++) pthread_join(c[i], NULL);
    printf("parent: end\n");

    sem_destroy(&s);
    return 0;
}
