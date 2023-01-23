#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define QTD 5

void* funcao_pthread(void* p){
    long id = (long)p;
    printf("[THREAD #%ld] start\n", id);
    sleep(1+(int)id);
    printf("[THREAD #%ld] end\n", id);
}

int main(void){
    pthread_t ths[QTD];

    printf("[main] start\n");

    for (long i = 0; i < QTD; i++){
        pthread_create(&ths[i], 0, funcao_pthread, (void*) i);
    }
    for (long i = 0; i < QTD; i++){
        pthread_join(ths[i], NULL); //null pq n recebe nenhum retorno. O resto da main só vai ser executado quando todas as threads terminarem
    }
    
    //sleep(10); sem o join o sleep ajuda a terminar a execução das threads, mas é meio errado fazer isso
    printf("[main] end\n");
    return 0;
}