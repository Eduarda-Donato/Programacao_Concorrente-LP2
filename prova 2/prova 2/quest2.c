/*(Questão 2 - 5 pontos) Faça um programa em C/C++ que controle o acesso de 
threads a duas seções críticas diferentes. O programa deverá ter 10 threads,
e duas variáveis globais (x e y) de tipo int, iniciadas com zero. As threads
modificarão em loop e em sequência as duas variáveis, porém a ordem de 
modificação dependerá do valor do índice que receberão no momento de sua 
criação: caso o índice seja par, a thread incrementa o valor da variável x com 
um valor aleatório entre 5 e 100 e dorme 200ms, em seguida incrementa o valor 
da variável y com um valor aleatório entre 1 e 50 e dorme 100ms; caso o índice 
seja ímpar, a thread incrementa o valor da variável x com um valor aleatório 
entre 1 e 30 e dorme 100ms, em seguida incrementa o valor da variável y com 
um valor aleatório entre 15 e 95 e dorme 250ms.
 
Use semáforos (biblioteca semaphore.h) para controlar o acesso às seções 
críticas, (trate o acesso à cada uma das variáveis como uma seção crítica
diferente) e garanta que todas as  threads  incrementem as variáveis globais
dentro de um ciclo de iterações. O programa deverá ser finalizado quando a 
soma dos valores de x e y for maior que 2000. Comente o código explicando 
o que é uma seção crítica, e como foi feito o controle de acesso às duas 
seções críticas do problema usando semáforos.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include <semaphore.h>

#define QTD_THREAD 10

//semaforos
sem_t lock;
sem_t lock2;

//variaveis globais
float x = 0, y = 0, soma = 0;


void* pares(void* p){
    long id = (long) p;
    int valor_inc;
    int valor_inc2;

    while(1){
        //o semaforo lock irá tratar da manipulação do x
        sem_wait(&lock); //Protocolo de entrada
        valor_inc = 1 + (rand() % 30); //valor do incremento randomizado
        x += valor_inc;
        soma += valor_inc;
        usleep ( 100 );
        sem_post(&lock); //Protocolo de entrada
        
        //o semaforo lock2 irá tratar da manipulação do x
        sem_wait(&lock2); //Protocolo de entrada
        valor_inc2 = 15 + (rand() % 95);
        y = valor_inc2;
        soma += valor_inc;
        usleep ( 250 );
        sem_post(&lock2); //Protocolo de entrada
        printf("[%ld] gerou %d (soma = %f)\n", id, valor_inc, soma);
        
        //se a soma for maior que 2000 finaliza o programa
        if (soma > 2000){
            exit(0);
        }
    }
    
}

void* impares(void* p){
    long id = (long) p;
    int valor_inc;
    int valor_inc2;

    while (1){

        sem_wait(&lock);
        valor_inc = 5 + (rand() % 100);
        
        x += valor_inc;
        soma += valor_inc;
        usleep ( 200 );
        sem_post(&lock);
        
        sem_wait(&lock2);
        valor_inc2 = 1 + (rand() % 50);
        y = valor_inc2;
        soma += valor_inc;
        usleep ( 100 );
        sem_post(&lock2);
        printf("[%ld] gerou %d (soma = %f)\n", id, valor_inc, soma);

        //se a soma for maior que 2000 finaliza o programa
        if (soma > 2000){
            exit(0);
        }
    }
}


int main(int argc, char const *argv[]){
    pthread_t ths[QTD_THREAD];
    time_t t;
    srand(time(&t));

    //icicializa semaforos
    sem_init(&lock,0,1);
    sem_init(&lock2,0,1);
    
    //condição para dividir as threads entre pa e impar
    for (long i = 0; i < QTD_THREAD; i++){
        if (i % 2 == 0){
            pthread_create(&ths[i], NULL, pares, (void*)i);
        }else{
            pthread_create(&ths[i], NULL, impares, (void*)i);
        }
    }
    for (int i = 0; i < QTD_THREAD; i++){
        if (i % 2 == 0){
            pthread_join(ths[i], NULL);
        }else{
            pthread_join(ths[i], NULL);
        }
    }
    
    return 0;
}
/*
seção critica é uma parte do código que manipula uma região de memoria compartilhada, na qual só pode ser manipuada uma threado por vez para evitar inconsistencia de memoria

utilizei o metodo da exclusao mutua, na qual uma thread fica esperando a outra sair para entrar e para isso utilizei dois semaforo locks em cada thread um para manipular o x e o outra para manipular o y
*/