#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define QTD_THREADS 4

sem_t mutex;
int var_global = 0;

void* secao_critica(void* p) {
	long idx = (long)p;
	printf("[%ld] iniciou...\n", idx);
	while(1) {
		sem_wait(&mutex);
		printf("[%ld] secao_critica()\n", idx);
		var_global++;
		printf("[%ld] %d\n", idx, var_global);
		sem_post(&mutex);
		sleep(1);
	}
}

int main(void) {
	pthread_t threads[QTD_THREADS];

	sem_init(&mutex, 0, 1);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, 
				secao_critica, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
