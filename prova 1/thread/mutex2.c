#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QUANT_THRS 5

long global = 0;
pthread_mutex_t mutex;

void* f_pthread(void* pt){
    long id = (long)pt;
    int contador = 0;
    printf("THREAD %ld\n", id);
    while (contador != 10){
        pthread_mutex_lock(&mutex);
        global++;
        pthread_mutex_unlock(&mutex);
        contador++;
    }
    printf("GLOBAL = %ld\n", global);
}

int main(void){
    pthread_t thrs[QUANT_THRS];
    global = 0;
    pthread_mutex_init(&mutex,0);
    printf("MAIN\n");
    for (long i = 0; i < QUANT_THRS; i++){
        pthread_create(&thrs[i], 0, f_pthread, (void*)i);
    }
    for (long i = 0; i < QUANT_THRS; i++){
        pthread_join(thrs[i], NULL);
    }
    printf("GLOBAL = %ld\n", global);
    
    
    return 0;
}