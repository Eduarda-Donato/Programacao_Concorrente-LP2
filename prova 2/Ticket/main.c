#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>


#define THREADS 5
int global = 0;

int number = 0;
int next = 0;
int turn[THREADS];

void* thread_func(void* p){
    long id = (long)p;
    int contador = 0;
    printf("Thread [%ld] iniciou\n", id);
    sleep(1);
    turn[id] = atomic_fetch_add(&number, 1);                                                                    
    while(turn[id] != next);
    printf("Thread [%ld] na seção crítica. Ticket = %d\n", id, number);

    while(contador < 10){
        global++;
        contador++;
    }

    atomic_fetch_add(&next, 1);
    printf("Thread [%ld] fora da seção crítica\n", id);

    //next++ é viável nesse caso, já que, se o escalonador mudar de thread antes do incremento ou durante
    //a próxima thread será bloqueada no while, até chegar na thread que vai incrementar. Porém, isso vai 
    //ocasionar uma maior processamento.
}

int main(void){

    pthread_t threads[THREADS];
    for(int i = 0; i < THREADS; i++){
        turn[i] = -1;
    }

    for(long i = 0; i < THREADS; i++){
        pthread_create(&threads[i], 0, thread_func, (void*) i);
    }

    for(int i = 0; i < THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("%d\n", global);


    return 0;
}