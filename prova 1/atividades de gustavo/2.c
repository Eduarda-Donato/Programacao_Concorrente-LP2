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
    char letra1 = 'A';
    letra1++;
    sleep(id+1);
    for (char letra2 = 'A'; letra2 <= 'Z'; letra2++){
        for (char letra3 = 'A'; letra3 <= 'Z'; letra3++){
            for (char letra4 = 'A'; letra4 <= 'Z'; letra4++){
                printf("%c%c%c%c\n",letra1,letra2,letra3,letra4);
            }
        }
    }  
    printf("olaaaaaa");
    
     
    
}

int main(int argc, char *argv[]){
    
    
    int escolha = atoi(argv[1]);
    if(escolha == 0){
        pthread_t thrs[26];
        //thread
        for (long i = 0; i < 26; i++){
            pthread_create(&thrs[i],0, dicionario,(void*) i);
        }
        for (int i = 0; i < 26; i++){
            pthread_join(thrs[i],NULL);
        }
        
        
        
    }else if(escolha == 1){
        //processo
        pid_t origPid;
        pid_t procs[26];
        origPid = getpid();
        int forkreturn;
        for (long i = 0; i <= 26; i++){
            if(getpid() == origPid){
                forkreturn = fork();
                if (getpid() == origPid){
                    procs[i] = forkreturn;
                }else{
                    if(i = 0){
                        dicionario((void*) i + 1);
                    }
                }
            }else{
                dicionario((void*) i);
            }
        }
        if(getpid() == origPid){
            wait(NULL);
            for (long i = 0; i <= 26; i++){
                
                kill(procs[i], SIGHUP);
            }
            exit(0);
        }
        
    }else{
        printf("Escolha invalida\n");
    }
}
