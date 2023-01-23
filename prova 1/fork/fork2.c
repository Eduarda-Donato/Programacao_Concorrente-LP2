
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
/*
    Segundo exemplo de uso da funcao fork()
    - Clona o processo e testa o valor de retorno da funcao
    fork, executando instrucoes diferentes no processo original
    e no processo clonado
*/

int main(void){
    int returnFork;
    char c;

    printf("Sou o processo original\n");
    printf("Mensagem antes da invocação do fork()\n");
    printf("Meu PID: %d\n", getpid());
    printf("Aperte <ENTER> para clonar este processo: ");
    scanf("%c", &c);

    returnFork = fork();

    /*
        fork_return > 0 (execucao do processo pai)
        fork_return == 0 (execucao do processo clonado)
        fork_return == -1 (erro, nao houve clonagem)
    */

    if (returnFork > -1){
        if (returnFork > 0){
            //executado pelo processo original
            printf("\nSou o processo original!\nretorno do fork: %d\n", returnFork);
            sleep(20);
            printf("Sou o processo original e vou terminar... meu PID era: %d\n", getpid());
        }else{
            //executado pelo processo clonado
            printf("\nSou o processo clonado!\nretorno do fork: %d\n", returnFork);
            sleep(20);
            printf("Sou o processo clonado e vou terminar... meu PID era: %d\n", getpid());
        }
    }else{
        printf("ERRO. Clonagem nao executada");
    }
}

/*int main(void){
    printf("ANTES: %d\n", getpid());
    int referencia = fork();
    printf("DEPOIS: %d %d\n", referencia, getpid());
    return 0; 
}*/