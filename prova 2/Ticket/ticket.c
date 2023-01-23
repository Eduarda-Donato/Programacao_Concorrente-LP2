#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>

#define QTD_THS 5

int global = 0;
int number =1;
int next = 1;
int turn[QTD_THS];

void* a_thread(void* p){
    long id = (long) p;
    int cont = 0;
    sleep(1);
    printf("[thread %ld] iniciou\n", id);
    
    //protocolo de entrada
    /*
    <turn[id] = number; number++>
    <await(turn[index] == next);>
    */
    turn[id] = atomic_fetch_add(&number,1); //lê e adiciona 1
    while (turn[id] != next);
    printf("[thread %ld] na secao critica (%d) ticket \n", id, turn[id]);

    //secao critica
    while (cont < 10){
        global++;  
        cont++;
    }   

    //protocolo de saida
    atomic_fetch_add(&next,1);
    printf("[thread %ld] fora da secao critica\n", id);
    //next++; // não muda a lógica //é viavel neste caso

        
    
    
}

int main(int argc, char const *argv[])
{
    pthread_t ths[QTD_THS];
    for (int i = 0; i < QTD_THS; i++)
    {
        turn[i] = 0;
    }
    
    for (long i = 0; i < QTD_THS; i++)
    {
        pthread_create(&ths[i], NULL, a_thread,(void*)i+1);
    }
    
    for (long i = 0; i < QTD_THS; i++)
    {
        pthread_join(ths[i], NULL);
    }
    printf("GLOBAL = %d", global);
    return 0;
}

