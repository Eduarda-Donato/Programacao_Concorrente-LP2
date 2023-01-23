#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define QTD 5

long var_global;

void* funcao_pthread(void* p){
    long id = (long)p;
    long valor = rand() % 100;
    printf("[thread#%ld] start\n",id);
    sleep(1+(int)id);
    var_global++;
    printf("[thread#%ld] VARIAVEL GLOBAL = %ld\n", id, var_global);

    /*for (long i = 0; i < 100000; i++){
        var_global++;
    }*/
    
    printf("[thread#%ld] end e retorna: %ld\n",id,valor);
    pthread_exit((void*)valor);

}

int main(void){
    pthread_t ths[QTD];
    void* retornado[QTD];
    var_global = 0;
    time_t t;
    srand(time(&t));
    printf("[main] start\n");
    for(long i = 0; i < QTD; i++){
        pthread_create(&ths[i],0,funcao_pthread,(void*)i);
    }
    for(long i = 0; i < QTD; i++){
        pthread_join(ths[i],&retornado[i]);
        printf("[main] recebeu %ld da thread#%ld\n",(long)retornado[i],i);
    }
    printf("[main] end\n");
    printf("VARIAVEL GLOBAL = %ld\n",var_global);

    return 0;
}

