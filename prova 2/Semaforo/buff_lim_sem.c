#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define TAM_BUF 8

int buf[TAM_BUF];
sem_t buf_empty, buf_full;
int rear = 0, front = 0;

void* producer(void* p) {
	int data;
	while(1) {
		sem_wait(&buf_empty);
		data = rand() % 100;
		buf[rear] = data;
		printf("[prod] gerou %d e depositou no slot %d\n", data,rear);
		rear = (rear + 1) % TAM_BUF;
		sleep(1);
		sem_post(&buf_full);
	}
}

void* consumer(void* p) {
	int result;
	while(1) {
		sem_wait(&buf_full);
		result = buf[front];
		printf("[cons] consumiu %d do slot %d\n", result, front);
		front = (front + 1) % TAM_BUF;
		sleep(1);
		sem_post(&buf_empty);
	}

}

int main(void) {
	time_t t;
	pthread_t prod;
	pthread_t cons;

	srand(time(&t));

	sem_init(&buf_empty, 0, TAM_BUF);
	sem_init(&buf_full, 0, 0);

	pthread_create(&prod, 0, producer, (void*)0);

	pthread_create(&cons, 0, consumer, (void*)0);

	sleep(60);

	return 0;
}
