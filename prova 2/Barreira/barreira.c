#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>

#define QTD_THS 5
int count = 0;


void tarefa(long i){
    sleep(i+1);
}



void* barreira(void* p){
    long index = (long)p;
    while (1)
    {
        printf("[%ld] vou realizar tarefa\n", index);
        tarefa(index);
        atomic_fetch_add(&count, 1);
        while ((count % QTD_THS) != 0);
        printf("[%ld] realizou tarefa\n", index);
        
    }
    
}

int main(int argc, char const *argv[]){
    pthread_t ths[QTD_THS];

    

    for (long i = 0; i < QTD_THS; i++)
    {
        pthread_create(&ths[i], NULL, barreira, (void*)i);
    }
    for (long i = 0; i < QTD_THS; i++)
    {
    pthread_join(ths[i], NULL);
    }
    return 0;
}
