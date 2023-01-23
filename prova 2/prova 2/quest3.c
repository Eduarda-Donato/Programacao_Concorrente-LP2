/*(Questão 3 - 5 pontos) Faça um programa em C/C++ que simule o controle de
temperatura de um motor. O programa deverá ter uma variável global do tipo
float chamada temperatura (que armazenará a temperatura em Celsius do motor,
com valor inicial 600 C) e duas threads além da main, realizando funções 
diferentes. A primeira thread  realizará   um   incremento   na   temperatura
com um valor aleatório entre -10 e 90 e depois dormirá 200ms. A segunda 
thread deverá conter um mecanismo que detecte o quanto antes quando o valor da 
temperatura ultrapassar 1000C - quando isso acontecer deve-se dormir 300ms e o
valor da variável temperatura passa a ser 600 novamente.
 
O programa deverá ser finalizado depois de 3 reduções de temperatura do motor.
Comente o código explicando o mecanismo utilizado na segunda thread e qual 
seria a alternativa se não houvesse o requisito de detecção da temperatura de
forma mais imediata possível. */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include <semaphore.h>

#define QTD_REPETICAO 3
#define QTD_THS 2

float temperatura = 600;

//utilitarios para o algoritmo do ticket
int number = 1; 
int next = 1; 
int turn[QTD_THS];


void* incrementa(void* p){
    long id = (long) p;
    int data;
    
    while (1){
        data =(rand() % 90); //valor de incremento randomizado

        //protocolo de entrada
        turn[id] = atomic_fetch_add(&number, 1);
        while (turn[id] != next);

        //seção critica
        temperatura += data; //manipulação da variavel golbal
        printf("[THREAD %ld] gerou %d (temp = %f)\n", id, data, temperatura);
        usleep ( 200 );

        // protocolo de saida
        atomic_fetch_add(&next, 1);
    }
    
        

}

void* detecta(void* p){
    long id = (long) p;
    int cont = 0;

    while(temperatura < 1000){ //while que vai rodar até a temperatura chegue a 1000
        
        //protocolo de entrada para garantir que apenas uma thread realize a detecção por vez
        turn[id] = atomic_fetch_add(&number, 1);
        while (turn[id] != next);
        
        printf("[THREAD %ld] (temp = %f)\n", id, temperatura);
        if (temperatura > 1000){ //se a temperatura for maior que 1000 ela volta ao estado inicial
            printf("voltou ao estado inicial\n\n");
            usleep ( 300 );
            temperatura = 600;
            cont++;
        }
        
        if(cont == 3){ //o programa para apos 3 detecção
            exit(0);
        }
        atomic_fetch_add(&next, 1); // protocolo de saida para liberar outra thread
    }
    
}

int main(int argc, char const *argv[]){
    time_t t;
    srand(time(&t));
    pthread_t ths[QTD_THS];

    for (int i = 0; i < QTD_THS; i++){
        turn[i] = 0;
    }
    
    pthread_create(&ths[0], NULL, incrementa, (void*) 0);
    pthread_create(&ths[1], NULL, detecta, (void*) 1);

    for (int i = 0; i < QTD_THS; i++){
        pthread_join(ths[i], NULL);
    }
    
    printf("temperatura = %f", temperatura);
    
    return 0;
}

/*
na segunda thread eu utelizei uma estrutua condicional if para tornar a temperatura 600 novamente
Poderia ter utilizado barreira e deixar a thread um incrementa e quando chegasse a 1000 seria liberada a thread 2 para tornar a temperatura 600 novamente
*/