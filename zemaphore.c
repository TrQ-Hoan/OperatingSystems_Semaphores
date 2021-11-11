#include "zemaphore.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

zem_t s;

void *child(void *arg) {
    sleep(4);
    puts("child");
    zem_post(&s);  // signal here: child is done
    return NULL;
}

int main(int argc, char *argv[]) {
    zem_init(&s, 0);
    puts("parent: begin");
    pthread_t c;
    pthread_create(&c, NULL, child, NULL);
    zem_wait(&s);  // wait here for child
    puts("parent: end");
    zem_destroy(&s);
    return 0;
}
