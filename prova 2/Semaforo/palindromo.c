#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


#define QTD_THREADS 6

pthread_barrier_t barreira_palavra;
pthread_barrier_t barreira_teste;

char palavra[QTD_THREADS];

void* gera_char(void* p) {
	long idx = (long) p;
	int count = 0;
	do {
		char c = 65 + (rand() % 26);
		palavra[idx] = c;
		count++;
		printf("[thread-%ld] gerou %c (%d)\n", idx, c, count);
		sleep(1);
		pthread_barrier_wait(&barreira_palavra);
		pthread_barrier_wait(&barreira_teste);
	} while (count < 10);
}

void print_palavra(char* str, int size) {
	for (int i = 0; i < size; i++) {
		printf("%c", str[i]);
	}
}

int testa_palindromo(char* str, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = size-1; j >=0; j--) {
			if (palavra[i] != palavra[j]) {
				return 0; //NOT PALINDROMO
			}
		}	
	}
	return 1; //PALINDROMO
}

int main(void) {
	pthread_t threads[QTD_THREADS];
	time_t t;
	int count = 0;
	
	srand(time(&t));

	pthread_barrier_init(&barreira_palavra, NULL, QTD_THREADS+1);
	pthread_barrier_init(&barreira_teste, NULL, QTD_THREADS+1);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL, gera_char, (void*)i);
	}

	do {
		pthread_barrier_wait(&barreira_palavra);
		if (testa_palindromo(palavra, QTD_THREADS)) {
			print_palavra(palavra, QTD_THREADS);
			printf(" foi detectada como palindromo! (%d)\n", 
					count);
		} else {
			print_palavra(palavra, QTD_THREADS);
			printf(" foi detectada como nao palindromo! (%d)\n",
					count);
		}
		sleep(1);
		count++;
		pthread_barrier_wait(&barreira_teste);
	} while (count < 10);

	return 0;
}
