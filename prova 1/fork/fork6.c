#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
    int returnFork;
    int valor_usuario;
    int retorno_filho = 0;

    printf("ORIGINAL: Processo original, PID: %d\n", getpid());
    printf("ORIGINAL: antes do fork()...\n");

    returnFork = fork();

    if (returnFork >= 0){
        //executou a clonagem
        if (returnFork == 0){
            //processo clonado
            printf("CLONADO: Sou o processo clonado!\n");
            printf("CLONADO: Meu pid %d\n", getpid());
            printf("CLONADO: Pid do processo original %d\n", getppid());
            printf("CLONADO: DOrmir por 5 segundos...\n");
            sleep(5);
            printf("CLONADO: Digite um valor de 0 a 100: ");
            scanf("%d", &valor_usuario);
            printf("CLONADO: dormindo 5s antes de sair...\n");
            sleep(5);
            printf("CLONADO: VOu sair enviando o valor %d\n", valor_usuario);
            exit(valor_usuario);
        }else{
            //processo original
            printf("ORIGINAL: Sou o processo original!\n");
            printf("ORIGINAL: Meu pid %d\n", getpid());
            printf("ORIGINAL: Pid do processo original %d\n", getppid()); //etorna o pid da shell
            printf("ORIGINAL: aguardando processo clonado sair...\n"); //atraves do wait
            wait(&retorno_filho);
            printf("ORIGINAL: processo original recebeu do clonado: %d\n", WEXITSTATUS(retorno_filho));
            printf("ORIGINAL: finalizando...\n");
            exit(0);
        }   
    }else{
        //não executou a clonagem
        printf("Erro ao invocar fork()!\n");
        exit(-1);
    }

    return 0;
}