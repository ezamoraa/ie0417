#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <errno.h>

#define NUM_THREADS 5

int counter;
pthread_mutex_t lock;

struct thread_data {
    pthread_t tid;
    bool do_lock;
};

void* thread_func(void *arg)
{
    struct thread_data *data = arg;
    printf("Thread %ld started\n", data->tid);

    for (int i = 0; i < 100000; i++) {
        if (data->do_lock) {
            pthread_mutex_lock(&lock);
        }
        counter++;
        if (data->do_lock) {
            pthread_mutex_unlock(&lock);
        }
    }

    printf("Thread %ld finished\n", data->tid);

    return NULL;
}

int main(int argc, char **argv)
{
    int ret;
    struct thread_data thr_data[NUM_THREADS] = {};

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        fprintf(stderr, "Failed to init mutex lock\n");
        return -1;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thr_data[i].do_lock = true;

        ret = pthread_create(&(thr_data[i].tid), NULL, &thread_func, &thr_data[i]);
        if (ret != 0) {
            fprintf(stderr, "Failedx to create thread :[%s]", strerror(ret));
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thr_data[i].tid, NULL);
    }

    printf("Final counter value: %d\n", counter);

    pthread_mutex_destroy(&lock);

    return 0;
}
