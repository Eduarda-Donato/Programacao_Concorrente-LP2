#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
/*
    Terceiro exemplo de uso da funcao fork()
    - Invocacoes sucessivas a funcao fork()
    - processos dormem por 15s
    n = quantidade de forks
    2^n = quantidade de processos gerados
*/

int main(void){
    char c;
    printf("Mensagem antes do fork()\n");
    printf("Aperte <ENTER> para clonar este processo: ");
    scanf("%c", &c);

    fork();     //realizado só pelo ogirinal
    sleep(10);  //original e clone 1

    fork();     //realizado pelo original e clone 1
    sleep(10);  //original clone 1, clone 2, clone 3

    fork();     //realizado pelo original e clone 1, clone 2, clone 3 (4 processos que já existem)
    sleep(10);  //original e clone 1 clone 2, clone 3, clone 4, clone 5, clone 6, clone 7 ()

    printf("oi\n");
}

/*int main(void){ //usar fork e ficar com apenas 3 programas
    printf("START\n");
    int referencia = fork();
    printf("fork()!\n");
    if (referencia == 0){
        fork();
    }
    printf("FINISH\n");
    return 0;
}*/