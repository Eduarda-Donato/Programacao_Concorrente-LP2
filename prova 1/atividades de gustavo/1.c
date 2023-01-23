/*
(Questão 1 - 5 pontos) 
Faça um programa em C/C++ que crie um arquivo de texto 
que funcionará como um dicionário contendo todas as strings com 4 caracteres 
alfabéticos maiúsculos (de "AAAA" a "ZZZZ"). Cada letra do alfabeto (de A a Z)
deverá ter sua parte do dicionário gerada por um processo ou thread individual,
(o programa deverá receber um parâmetro na sua execução que determine se threads
ou processos serão usados) que gerará todas as strings iniciadas por uma letra 
(ex. processo/thread que gerará as strings começando com a letra 'C' gerará as 
strings de "CAAA" a "CZZZ"). A thread principal do processo original deverá 
aguardar a geração de todas as partes e gerar um arquivo de texto contendo o 
dicionário inteiro. Comente no código as diferenças do uso de processos e 
threads, do ponto de vista do consumo de recursos e do ponto de vista do uso
das funções/API.
*/

/*1111 9999*/ /*t ou p*/ /*pthread_exit*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

void* dicionario(void* pt){
    long id = (long) pt; //vai funcionar como o primeiro numero do alfabeto
    sleep(id+1);
    for (int j = 1; j <= 9; j++){
        for (int k = 1; k <= 9; k++){
            for (int l = 1; l <= 9; l++){
                printf("%ld%d%d%d\n",id,j,k,l);
            }
        }
    }   
    
}

int main(int argc, char *argv[]){
    
    int escolha = atoi(argv[1]);
    if(escolha == 0){
        pthread_t thrs[9];
        //thread
        for (long i = 0; i < 9; i++){
            pthread_create(&thrs[i],0, dicionario,(void*) i+1);
        }
        for (int i = 0; i < 9; i++){
            pthread_join(thrs[i],NULL);
        }
        
        
        
    }else if(escolha == 1){
        //processo
        pid_t origPid;
        pid_t procs[9];
        origPid = getpid();
        int forkreturn;
        for (long i = 0; i <= 9; i++){
            
            if(getpid() == origPid){
                
                forkreturn = fork();
                if (getpid() == origPid){
                    procs[i] = forkreturn;
                }else{
                    if(i = 0){ //n faz sentido
                        dicionario((void*) i+1);
                    }
                }
            }else{
                dicionario((void*) i);
            }
        }
        if(getpid() == origPid){
            wait(NULL);
            for (long i = 0; i <= 9; i++){
                kill(procs[i], SIGHUP);
            }
            exit(0);
        }
        
    }else{
        printf("Escolha invalida\n");
    }
}
