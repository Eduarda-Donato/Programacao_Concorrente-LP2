#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>
#include <semaphore.h>

#define QTD_THREADS 4

sem_t mutex;
int var_global = 0;

void* secao_critica(void* p){
    long idx = (long)p;
    printf("[%ld] iniciou...\n", idx);
    while (1){
        sem_wait(&mutex);
        printf("[%ld] secao_critica()\n", idx);
        var_global++;
        printf("global = %d\n", var_global);
        //sleep(idx + 1); problema de estavation quando so uma thread entra
        sem_post(&mutex);
        sleep(1); //faz a thread se alternar
    }
    
}

int main(int argc, char const *argv[]){
    pthread_t ths[QTD_THREADS];

    sem_init(&mutex, 0,1);
    for (long i = 0; i < QTD_THREADS; i++){
        pthread_create(&ths[i],NULL,secao_critica,(void*)i);
    }
    for (long i = 0; i < QTD_THREADS; i++){
        pthread_join(ths[i],NULL);
    }
    return 0;
}

























  