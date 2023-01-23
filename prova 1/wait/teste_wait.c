#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void){
    char c;
    int fork_retur;
    int pid_orig;
    int valor;
    int recebido;

    pid_orig = getpid();

    printf("Aperte enter");
    scanf("%c", &c);
    fork_retur = fork();
    if(getpid() != pid_orig){
        printf("SOu o clone\n");
        printf("meu pid %d\n", getpid());
        sleep(5);
        printf("DIgite um valor: ");
        scanf("%d", &valor);
        exit(valor);
    }
    if(getpid() == pid_orig){
        printf("\nso saiu quando receber o valor do clone\n");
        wait(&recebido);
        printf("recebi: %d\n",WEXITSTATUS(recebido));
    }
    
    return 0;
}