#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>
#include <semaphore.h>

sem_t barreira1;
sem_t barreira2;

void tarefa(long i){
    printf("[%ld] tarefa()\n", i);
    sleep((int)i);
}

void* tarefa_thread_1(void* p){
    long idx = (long)p;
    printf("[%ld] iniciou...\n", idx);
    while (1){
        tarefa(idx);
        printf("[%ld] chegou na barreira...\n", idx);
        sem_post(&barreira1);
        sem_wait(&barreira2);
        printf("[%ld] passou da barreira...\n", idx);
        
    }
    
}

void* tarefa_thread_2(void* p){
    long idx = (long)p;
    printf("[%ld] iniciou...\n", idx);
    while (1){
        tarefa(idx);
        printf("[%ld] chegou na barreira...\n", idx);
        sem_post(&barreira2);
        sem_wait(&barreira1);
        printf("[%ld] passou da barreira...\n", idx);
        
    }
    
}

int main(int argc, char const *argv[]){
    pthread_t th1, th2;

    
    sem_init(&barreira1, 0,0);
    sem_init(&barreira2, 0,0);
    
    pthread_create(&th1,NULL,tarefa_thread_1,(void*)1);
    pthread_create(&th2,NULL,tarefa_thread_2,(void*)2);
    

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);

    return 0;
}





