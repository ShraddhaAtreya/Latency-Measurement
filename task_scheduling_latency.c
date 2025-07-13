#include <stdio.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t lock;
pthread_cond_t cond;
struct timespec start, end;

void *worker(void *arg) {
    pthread_mutex_lock(&lock);
    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_cond_wait(&cond, &lock);
    clock_gettime(CLOCK_MONOTONIC, &end);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t thread;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&thread, NULL, worker, NULL);

    // Sleep briefly to ensure the thread is waiting
    struct timespec delay = {0, 10 * 1000000}; // 10 ms
    nanosleep(&delay, NULL);

    pthread_mutex_lock(&lock);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    pthread_join(thread, NULL);

    long latency_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    printf("Scheduling latency (simulated): %ld ns\n", latency_ns);

    return 0;
}
