//sincronismo condicional
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>

#define QTD_THS 5

pthread_barrier_t barreira;

void tarefa(long idx){
    printf("[%ld] tarefa()\n", idx);
    sleep((int)idx + 1);
    for (long i = 0; i < (idx +1)*10000000; i++);
    
}

void* tarefa_thread(void* p){
    long idx = (long) p;
    printf("[%ld] iniciou...\n", idx);
    while (1)
    {
        tarefa(idx);
        printf("[%ld] chegou na tarefa\n", idx);
        pthread_barrier_wait(&barreira);
        printf("[%ld] passou da tarefa\n", idx);
    }
    
}

int main(int argc, char const *argv[]){
    pthread_t ths[QTD_THS];

    int init_ret = pthread_barrier_init(&barreira, NULL, QTD_THS);

    if(init_ret != 0){
        printf("[main] erro ao inicializar barreira (%d)\n",init_ret);
    }
    for (long i = 0; i < QTD_THS; i++){
        pthread_create(&ths[i], NULL, tarefa_thread, (void*)i);
    }
    for (long i = 0; i < QTD_THS; i++){
        pthread_join(ths[i], NULL);
    }
    
    return 0; 
}
