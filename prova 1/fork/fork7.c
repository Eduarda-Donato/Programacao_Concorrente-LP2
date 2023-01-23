#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/*Faça um programa em C que solicite do usuário um valor inteiro e crie (clone) este valor recebido
em quantidade de processos. Os processos executarão uma função para que durmam 60s.
O processo original (em sua função principal) posteriormente deve dormir por 20s e depois finalizar
todos os processos clonados antes que estes acordem.*/

 
void dorme(){
    sleep(60);
}

int main(void){
    int quantidade;
    int forkReturn;
    int pid_original;
    int* pid_clones; //n aloca quantos de tamanho tem o array pq n sabemos a quant de clones ainda

    pid_original = getpid();

    printf("Antes do fork. Programa original\n");
    printf("[ORIGINAL]: Digite a quantidade de processos a criar: ");
    scanf("%d", &quantidade);

    //Alocar a memoria do array pid_clones, pois agora sabemos sua quantidade
    pid_clones = (int*)malloc(sizeof(int)*quantidade);

    for (int i = 0; i < quantidade;i++){
        if(getpid() == pid_original){
            //processo original
            forkReturn = fork();
            if (getpid() == pid_original){
                // processo original
                pid_clones[i] = forkReturn;
            }else{
                // processo clone
                dorme();
            }
        }else{
            // processo clone
            dorme();
        }
    }
    if (getpid() == pid_original){
        // processo original
        printf("[ORIGINAL]: Dormir por 10 segundos...\n");
        sleep(10);
        printf("[ORIGINAL]: finalizar processos clonados\n");
        for (int i = 0; i < quantidade; i++){
            kill(pid_clones[i], SIGHUP); 
        }
        

    }
    
        

    printf("oi\n");
}
