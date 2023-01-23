
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <stdatomic.h>                                                    
#include <pthread.h>
#include <semaphore.h>

#define QTD_PROD 1
#define QTD_CONS 4

int buf;
sem_t buf_empty, buf_full;//vazio e cheio


void* producer(void* p){
    long idx = (long)p;
    while (1){
        sem_wait(&buf_empty); // o primeiro passa
        buf = rand() % 100;
        printf("[prod %ld] gerou %d\n", idx, buf);
        sleep(1);
        sem_post(&buf_full);//desbloqueia o consumidor
    }
    
}

void* consumer(void* p){
    long idx = (long)p;
    int result;
    while (1){
        sem_wait(&buf_full); //é liberado qunado o produdor acrescenta no sem_post
        result = buf;
        printf("[cons %ld] consumiu %d\n", idx, buf);
        sleep(1);
        sem_post(&buf_empty);
    }
    
}int prod(){
    int value;
    while (1){
        value = rand() % 100;
    }
    return value;
}

int main(int argc, char const *argv[]){
    pthread_t prod[QTD_PROD];
    pthread_t cons[QTD_CONS];
    time_t t;

    srand(time(&t));

    sem_init(&buf_empty,0,1); //endereço, , valor que começa
    sem_init(&buf_full,0,0);

    for (long i = 0; i < QTD_PROD; i++){
        pthread_create(&prod[i], NULL, producer, (void*)i);
    }
    
    for (long i = 0; i < QTD_PROD; i++){
        pthread_create(&cons[i], NULL, consumer, (void*)i);
    }
    sleep(60);
    return 0;
}
