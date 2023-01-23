/*(Questão 1 - 5 pontos) Faça um programa em C/C++ que crie um arquivo de texto 
que funcionará como um dicionário contendo todas as strings com 4 caracteres 
alfabéticos maiúsculos (de "AAAA" a "ZZZZ"). Cada letra do alfabeto (de A a
Z) deverá ter sua parte do dicionário gerada por uma thread individual, de tal 
forma que esta thread gere todas as strings iniciadas por aquela letra (ex. a 
thread que gerará as strings começando com a letra 'C' gerará as strings de 
"CAAAA" a "CZZZZ"). Depois que todas as threads geram suas partes, a thread 
principal deve gerar um arquivo de texto contendo o dicionário inteiro. 
 
Use barreiras (API Posix Threads - pthread.h ou a API de threads STD C++11) 
para sincronizar as threads.
 
Referências para manipulação de arquivos em C:
 
https://www.inf.pucrs.br/~pinho/LaproI/Arquivos/Arquivos.htm (português)
http://www.csc.villanova.edu/~mdamian/C/c-files.htm (inglês)
 
Comente o código discutindo o ciclo de vida das threads. Através de 
comentários, faça a relação de cada invocação a funções da API PThreads
com a máquina de estados de threads.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include <semaphore.h>


pthread_barrier_t barreira;
pthread_barrier_t barreira2;
pthread_barrier_t barreira3;
pthread_barrier_t barreira4;

char palavra[4];

void* primeira(void* pt){
    long id = (long) pt; 
    //sleep(id+1);
    printf("%c", 'A');
    pthread_barrier_wait(&barreira);
    for (char letra2 = 'B'; letra2 <= 'Z'; letra2++){
        //printf("%c", letra2);
        palavra[id] = letra2;
    } 
    
}
void* segunda(void* pt){
    long id = (long) pt;
    pthread_barrier_wait(&barreira); 
    printf("%c", 'A');
    pthread_barrier_wait(&barreira2); 
    for (char letra2 = 'B'; letra2 <= 'Z'; letra2++){
        //printf("%c", letra2);
        palavra[id] = letra2;
    } 
    
    
}
void* terceira(void* pt){
    long id = (long) pt; 
    pthread_barrier_wait(&barreira2); 
    printf("%c", 'A');
    pthread_barrier_wait(&barreira3); 
    for (char letra2 = 'B'; letra2 <= 'Z'; letra2++){
        //printf("%c", letra2);
        palavra[id] = letra2;
    } 
    
    
}
void* quarta(void* pt){
    long id = (long) pt;
    pthread_barrier_wait(&barreira3);  
    printf("%c", 'A');
    pthread_barrier_wait(&barreira4);
    for (char letra2 = 'B'; letra2 <= 'Z'; letra2++){
        //printf("%c", letra2);
        palavra[id] = letra2;
    } 
    printf("\n");
    
}

void exibe_palavra(char* palavra, int tamanho){
    for (int i = 0; i < tamanho; i++){
        printf("%c", palavra[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]){
    pthread_t thrs[4];
    pthread_barrier_init(&barreira, NULL, 1);
    pthread_barrier_init(&barreira2, NULL, 1);
    pthread_barrier_init(&barreira3, NULL, 1);
    pthread_barrier_init(&barreira4, NULL, 1);
    //thread
    pthread_create(&thrs[0],0, primeira,(void*) 0);
    pthread_create(&thrs[1],0, segunda,(void*) 1);
    pthread_create(&thrs[2],0, terceira,(void*) 2);
    pthread_create(&thrs[3],0, quarta,(void*) 3);

    pthread_barrier_wait(&barreira4);
    exibe_palavra(palavra, 4);

    for (int i = 0; i < 4; i++){
        pthread_join(thrs[i],NULL);
    }
        
}
