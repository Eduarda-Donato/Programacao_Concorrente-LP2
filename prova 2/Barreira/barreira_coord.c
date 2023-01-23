//sem contenção de memoria
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>

#define QTD_THS 5
int arrive[QTD_THS];
int proceed[QTD_THS]; 

void tarefa(long i){
    sleep(i+1);
}

void* worker(void* p){
    long index = (long)p;
    while (1)
    {
        printf("[%ld] vou realizar tarefa\n", index);
        tarefa(index);
        arrive[index] = 1; //ponto de barreiramento
        //<await (proceed[index] == 1)>
        while (proceed[index] != 1); //bloqueio
        proceed[index] = 0;
        printf("[%ld] realizou tarefa\n", index);
        
    }
}

    void* coordinator(void* p){ //tira a contenção de memoria
        while (1){
            for (int i = 0; i < QTD_THS; i++)
            {
                //<await (arrive[i] == 1)>
                while (arrive[i] != 1); //detecção bastante custosa 
                    arrive[i] = 0;
                
                
            }
            for (int i = 0; i < QTD_THS; i++)
            { // libera pra passar pelo barramento
                proceed[i] = 1;
            }
        }
        
    }
    


int main(int argc, char const *argv[]){
    pthread_t ths[QTD_THS];
    pthread_t coord;
    for (long i = 0; i < QTD_THS; i++)
    {
        arrive[i] = 0;
        proceed[i] = 0;
    }
    

    for (long i = 0; i < QTD_THS; i++)
    {
        pthread_create(&ths[i], NULL, worker, (void*)i);
    }

    pthread_create(&coord, NULL, coordinator, NULL);

    for (long i = 0; i < QTD_THS; i++)
    {
        pthread_join(ths[i], NULL);
    }

    pthread_join(coord, NULL);
    return 0;
}
