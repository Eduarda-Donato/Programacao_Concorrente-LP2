#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define QTD 5

void* funcao_pthread(void* p){
    long id = (long)p;
    long valor = rand() % 100;
    printf("[thread #%ld] start\n", id);
    sleep(1+(int)id);
    printf("[thread #%ld] retorna: %ld\n", id, valor);
    printf("[thread #%ld] end\n", id);
    pthread_exit((void*)valor);
}

int main(void){
    pthread_t ths[QTD];
    void* retornado[QTD];
    time_t t;
    srand(time(&t));

    printf("[main] start\n");

    for (long i = 0; i < QTD; i++){
        pthread_create(&ths[i], 0, funcao_pthread, (void*)i);
    }
    for (long i = 0; i < QTD; i++){
        pthread_join(ths[i], &retornado[i]);
        printf("[main] A thread#%ld retornou: %ld\n", i, (long)retornado[i]);
    }

    printf("[main] end\n"); 
    
}