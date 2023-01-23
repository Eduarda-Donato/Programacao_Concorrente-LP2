#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
//#include <

int main(void){
    int pidOrig;
    int* pidClone;
    //char c;
    int pegaFork;
    int quantProc;

    pidOrig = getpid();
    /*printf(pidOrig);
    printf("\n");*/

    //printf("Digite enter para continuar");
    //scanf("%c", &c);
    printf("Digite a quantidade de processos desejados: ");
    scanf("%d", &quantProc);

    pidClone = (int*)malloc(sizeof(int)*quantProc);

    for (int i = 0; i < quantProc; i++){
        if(getpid() == pidOrig){
            pegaFork = fork();
            if(getpid() == pidOrig){
                pidClone[i] = pegaFork;
            }
        }
    }
    sleep(20);
    for (int i = 0; i < quantProc; i++){
        kill(pidClone[i], SIGHUP);
    }
    
    
    

    return 0;
}