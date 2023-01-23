#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define QTD_THREADS 5

typedef struct{
    float saldo;
    int cpf;
    float valor;
}conta;

typedef struct 
{
    pthread_mutex_t mutex_conta;
}monitor;

monitor mon;

void print_info(conta* cont){
    printf("con.cpf = %d\n", cont->cpf);
    printf("con.saldo = %f\n", cont->saldo);
    printf("con.valor = %f\n", cont->valor);
}

void* credito(void* p){
    conta *con = (conta*)p;

    print_info(con);
    pthread_mutex_lock(&mon.mutex_conta);
    con->saldo += con->valor; 
    printf("con.saldo (ATUALIZADO) = %f\n", con->saldo);
    pthread_mutex_unlock(&mon.mutex_conta);
}

void* debito(void* p){
    conta *con = (conta*)p;

    print_info(con);
    pthread_mutex_lock(&mon.mutex_conta);
    con->saldo -= con->valor; 
    printf("con.saldo (ATUALIZADO) = %f\n", con->saldo);
    pthread_mutex_unlock(&mon.mutex_conta);

}

int main(int argc, char const *argv[]){
    pthread_t ths[QTD_THREADS];
    conta contas[QTD_THREADS];
    time_t t;
    srand(time(&t));

    pthread_mutex_init(&mon.mutex_conta, NULL);

    for (int i = 0; i < QTD_THREADS; i++){
        contas[i].cpf = rand() % 10;
        contas[i].saldo = rand() % 1000;
        contas[i].valor = rand() % 100;
    }
    for (int i = 0; i < QTD_THREADS; i++){
        if (i % 2 == 0){
            pthread_create(&ths[i], NULL, credito, (void*)&contas[i]);
        }else{
            pthread_create(&ths[i], NULL, debito, (void*)&contas[i]);
        }  
    }

    for (int i = 0; i < QTD_THREADS; i++){
        pthread_join(ths[i], NULL);
    }
    return 0;
}
