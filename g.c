#include <errno.h>
#include <time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int ready = 0;

void *thread_wait(void *arg) {
    printf("sleep\n");
    sleep(1);
    printf("Waiting\n");
    pthread_mutex_lock(&mutex);
    // pthread_cond_wait(&cond, &mutex);
    // pthread_mutex_unlock(&mutex);

    printf("1111111111111111111!\n");
    return NULL;
}

void *thread_notify(void *arg) {
    printf("lock\n");
    sleep(1);
pthread_mutex_lock(&mutex);
printf("lock\n");
    sleep(1);

    // pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
    printf("relaeas\n");
    sleep(3);

    // pthread_cond_signal(&cond);
    return NULL;
}
void *thread_wait1(void *arg) {
    printf("Waiting\n");

    // /pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    // pthread_mutex_unlock(&mutex);

    printf("2222222222222222222222\n");
    return NULL;
}

int main() {
    pthread_t wait_thread, notify_thread,wait_thread1;

    // pthread_create(&wait_thread1, NULL, thread_wait1, NULL);
    pthread_create(&wait_thread, NULL, thread_wait, NULL);
    pthread_create(&notify_thread, NULL, thread_notify, NULL);

    pthread_join(wait_thread, NULL);
    pthread_join(notify_thread, NULL);

    pthread_cond_destroy(&cond);

    return 0;
}
