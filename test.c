#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

static int count;
pthread_mutex_t thread;

void *func_one(void *arg)
{
    pthread_mutex_lock(&thread);
    for (int i = 0; i < 1000000; i++)
        count++;
    pthread_mutex_unlock(&thread);
    return NULL;
}

void *func_two(void *arg)
{
    pthread_mutex_lock(&thread);
    for (int i = 0; i < 1000000 ; i++)
        count++;
    pthread_mutex_unlock(&thread);
    return NULL;
}

int main ()
{
    pthread_t thread_one;
    pthread_t thread_two;

    pthread_mutex_init(&thread, NULL);

    pthread_create(&thread_one, NULL, func_one, NULL);
    pthread_create(&thread_two, NULL, func_two, NULL);

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);
    pthread_mutex_destroy(&thread);
    printf ("COUNT == %d\n", count);
}