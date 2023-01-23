#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>
#include <semaphore.h>

#define QTD_THREADS 4

sem_t barreira[QTD_THREADS];

void tarefa(long i){
    printf("[%ld] tarefa()\n", i);
    sleep((int)i);
}

void* tarefa_thread(void* p){
    long idx = (long)p;
    printf("[%ld] iniciou...\n", idx);
    while (1){
        tarefa(idx);
        printf("[%ld] chegou na barreira...\n", idx);
        sem_post(&barreira[idx]);
        for (int i = 0; i < QTD_THREADS; i++){
            if(i != idx){
                sem_wait(&barreira[i]);
            }
        }
        printf("[%ld] passou da barreira...\n", idx);
        
    }
    
}

int main(int argc, char const *argv[]){
    pthread_t ths[QTD_THREADS];

    for (int i = 0; i < QTD_THREADS; i++)
    {
        sem_init(&barreira[i], 0,0);
    }
    
    
    for (long i = 0; i < QTD_THREADS; i++){
        pthread_create(&ths[i],NULL,tarefa_thread,(void*)i);
    }
    for (long i = 0; i < QTD_THREADS; i++){
        pthread_join(ths[i],NULL);
    }
    return 0;
}

























  