#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
    Quinto exemplo de uso da funcao fork()
    - Invocacoes sucessivas a funcao fork() para criacao de processos
    de acordo com quantidade definida pelo usuario (clonarei quantidade-1,
    pois contamos com o processo original)
    - so o processo original se clona, controlando o numero
    de processos gerados
*/

int main(void){
    int quantidade;
    int returnFork;
    int pid_original;

    pid_original = getpid();

    printf("Mensagem antes do fork() - pid original: %d\n", pid_original);

    printf("Mensagem antes do fork()\n");
    printf("DIgite a quantidade desejada para clonar este processo: ");
    scanf("%d", &quantidade);
    /*
        Se a quantidade total de processos for a que o usuario digitar
        voce contara o processo original (e o laco considerara um indice
        a menos)
    */

    if (quantidade > 1){
        returnFork = fork();
        if (returnFork > 0){
            for (int i = 0; i < quantidade -2; i++){        //quantidade de processos -2 significa
                if (returnFork > 0){                        //que contaremos com o processo original
                    returnFork = fork();
                }                                            
                
                //fork();
            }
        }
    }
    
    printf("Mensagem depois do fork()\n");
    sleep(15);
    printf("Processo finalizando... meu PID era: %d\n", getpid());

    return 0;
}
