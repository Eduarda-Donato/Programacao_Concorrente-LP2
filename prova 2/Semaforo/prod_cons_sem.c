#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define QTD_PROD 2
#define QTD_CONS 4

int buf;
sem_t buf_empty, buf_full;

void* producer(void* p) {
	long idx = (long)p;
	while(1) {
		sem_wait(&buf_empty);
		buf = rand() % 100;
		printf("[prod-%ld] gerou %d\n", idx, buf);
		sleep(1);
		sem_post(&buf_full);
	}
}

void* consumer(void* p) {
	long idx = (long)p;
	int result;
	while(1) {
		sem_wait(&buf_full);
		result = buf;
		printf("[cons-%ld] consumiu %d\n", idx, result);
		sleep(1);
		sem_post(&buf_empty);
	}

}

int main(void) {
	time_t t;
	pthread_t prod[QTD_PROD];
	pthread_t cons[QTD_CONS];

	srand(time(&t));

	sem_init(&buf_empty, 0, 1);
	sem_init(&buf_full, 0, 0);

	for (long i = 0; i < QTD_PROD; i++) {
		pthread_create(&prod[i], 0, producer, (void*)i);
	}
	
	for (long i = 0; i < QTD_CONS; i++) {
		pthread_create(&cons[i], 0, consumer, (void*)i);
	}

	sleep(60);


}
