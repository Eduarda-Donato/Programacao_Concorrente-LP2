#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

int main(void){
    pid_t orig_pid;
    orig_pid = getpid();
    int forkret;
    int procs[4];
    time_t t;
    //srand(getpid());
    //srand(time(&t));
    int retorno1,retorno2,retorno3,retorno4;
    int retorno[4];

    printf("Sou o Processo original\n");
    
    for (int i = 0; i < 4; i++){ //4 filhos e 1 pai
        if(getpid() == orig_pid){
            forkret = fork();
            if (getpid() == orig_pid){
                procs[i] = forkret;
            }
        }
    }
    //sleep(10);
    /*if(getpid() != orig_pid){
        printf("oi\n");
    }
    if(getpid() == orig_pid){
        printf("oie\n");
    }*/
    if(getpid() != orig_pid){ //clones
        srand(getpid());
        int retornarei = rand() % 100;
        sleep(3);
        printf("Sou o clone %d e estou retornando %d\n", getpid(), retornarei);
        exit(retornarei);
        
    }
    if(getpid() == orig_pid){

        /*waitpid(procs[0], &retorno1, 0);
        waitpid(procs[1], &retorno2, 0);
        waitpid(procs[2], &retorno3, 0);
        waitpid(procs[3], &retorno4, 0);*/

        for (int i = 0; i < 4; i++){ //espera o retorno dos clones
            waitpid(procs[i], &retorno[i], 0);
        } 
    }
    for (int i = 0; i < 4; i++){ //finlaliza os clones
        kill(procs[i],SIGHUP);
    }
    
    for (int i = 0; i < 4; i++){
        printf("[ORIGINAL] recebi %d do clone %d\n", WEXITSTATUS(retorno[i]), procs[i]);
    }
    
    /*printf("[ORIGINAL] recebi %d do clone %d\n", WEXITSTATUS(retorno1), procs[0]);
    printf("[ORIGINAL] recebi %d do clone %d\n", WEXITSTATUS(retorno2),procs[1]);
    printf("[ORIGINAL] recebi %d do clone %d\n", WEXITSTATUS(retorno3),procs[2]);
    printf("[ORIGINAL] recebi %d do clone %d\n", WEXITSTATUS(retorno4),procs[3]);*/
    
    exit(0); //finaliza o processo origginal
}