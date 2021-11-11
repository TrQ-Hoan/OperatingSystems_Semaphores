#ifndef __zemaphore_h__
#define __zemaphore_h__
#include <pthread.h>

typedef struct __zem_t {
    int value;
    pthread_cond_t cond;
    pthread_mutex_t lock;
} zem_t;

void zem_init(zem_t *z, int value) {
    z->value = value;
    pthread_cond_init(&z->cond, (void *)value);
    pthread_mutex_init(&z->lock, (void *)value);
}

void zem_wait(zem_t *z) {
    pthread_mutex_lock(&z->lock);
    while (z->value <= 0) pthread_cond_wait(&z->cond, &z->lock);
    z->value--;
    pthread_mutex_unlock(&z->lock);
}

void zem_post(zem_t *z) {
    pthread_mutex_lock(&z->lock);
    z->value++;
    pthread_cond_signal(&z->cond);
    pthread_mutex_unlock(&z->lock);
}

void zem_destroy(zem_t *z) {
    pthread_cond_destroy(&z->cond);
    pthread_mutex_destroy(&z->lock);
}

#endif  // __zemaphore_h__
