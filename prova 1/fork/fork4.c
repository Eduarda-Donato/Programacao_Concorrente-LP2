#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
    Quarto exemplo de uso da funcao fork()
    - Invocacoes sucessivas a funcao fork() para criacao de 3 processos
    - processos dormem por 15s
    - so o processo original se clona, controlando o numero
    de processos gerados
*/

int main(void){
    char c;

    int returnFork;
    printf("Mensagem antes do fork()\n");
    printf("Aperte <ENTER> para clonar este processo: ");
    scanf("%c", &c);

    returnFork = fork();
    if (returnFork > 0){
        //processo original
        printf("\nMensagem antes do 2o fork()\n");
        printf("Aperte <ENTER> para clonar este processo novamente: ");
        scanf("%c", &c);

        fork();
    }

    printf("Mensagem depois do fork()\n");
    sleep(15);
    printf("Processo finalizando... meu PID era: %d\n", getpid());
    return 0;

}