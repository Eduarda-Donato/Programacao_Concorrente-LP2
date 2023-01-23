#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

/*
    Primeiro exemplo de uso da funcao fork()
    - Clona o processo e tanto o original quanto o clonado 
    dormem por 20s (e exibem as mesmas mensagens duplicadas)
*/
/*int main(void){
    printf("Sou o original\n");
    fork();
    sleep(10);
    printf("olha eu ai\n");
}*/


/*
    Segundo exemplo de uso da funcao fork()
    - Clona o processo e testa o valor de retorno da funcao
    fork, executando instrucoes diferentes no processo original
    e no processo clonado
*/
/*int main(void){
    int soma = 0;
    int forkretorna;
    forkretorna = fork();
    if(forkretorna > 0){
        soma += 5;
        printf("%d\n", soma);
    }else{
        soma += 3;
        printf("%d\n", soma);
    }
}*/

/*
    Terceiro exemplo de uso da funcao fork()
    - Invocacoes sucessivas a funcao fork()
    - processos dormem por 15s
    n = quantidade de forks
    2^n = quantidade de processos gerados
*/
/*int main(void){
    int quant;
    printf("Digite a quantidade de processos: ");
    scanf("%d",&quant);
    for (int i = 0; i < quant; i++){
        fork();
    }
    sleep(5);
    printf("Olha eu aqui\n"); 

}*/


/*
    Quarto exemplo de uso da funcao fork()
    - Invocacoes sucessivas a funcao fork() para criacao de 3 processos
    - processos dormem por 15s
    - so o processo original se clona, controlando o numero
    de processos gerados
*/

/*int main(void){
    int forkretorna;
    pid_t originalpid;
    originalpid = getpid();
    for (int i = 0; i < 3; i++){ //cria 3 processos clones e tem o original
        if(getpid() == originalpid){
            forkretorna = fork();
        }
    }
    sleep(5);
    printf("OLha eu aqui\n");
    
}*/

/*
    Quinto exemplo de uso da funcao fork()
    - Invocacoes sucessivas a funcao fork() para criacao de processos
    de acordo com quantidade definida pelo usuario (clonarei quantidade-1,
    pois contamos com o processo original)
    - so o processo original se clona, controlando o numero
    de processos gerados
*/
/*int main(void){
    int quant;
    int pidoriginal;
    pidoriginal = getpid();
    int retornofork;
    printf("DIgite a qauntidade total desejada: ");
    scanf("%d", &quant);
    for (int i = 0; i < quant-1; i++){
        if(getpid() == pidoriginal){
            retornofork = fork();
        }
    }
    printf("OLha eu aqui\n");
    
}*/

/*Faça um programa em C que solicite do usuário um valor inteiro e crie (clone) este valor recebido
em quantidade de processos. Os processos executarão uma função para que durmam 60s.
O processo original (em sua função principal) posteriormente deve dormir por 20s e depois finalizar
todos os processos clonados antes que estes acordem.*/

void f_dorme(){
    sleep(20);
}


int main(void){
    int quant;
    int pidoriginal;
    pidoriginal = getpid();
    int retornofork;
    int *procs;
    procs = (int*)malloc(sizeof(int)*quant);
    printf("DIgite a qauntidade total desejada: ");
    scanf("%d", &quant);
    if(quant > 1){
        for (int i = 0; i < quant; i++){
            if(getpid() == pidoriginal){
                retornofork = fork();
                if (getpid() == pidoriginal){
                    procs[i] = retornofork;
                }else{
                    f_dorme();
                }
                
            }else{
                f_dorme();
            }
        }
    }
    if(getpid() == pidoriginal){
        f_dorme();
    }else{
        sleep(10);
        for (int i = 0; i < quant; i++){
            kill(procs[i], SIGHUP);
        }
        
    }
    printf("OLha eu aqui\n");
    
}