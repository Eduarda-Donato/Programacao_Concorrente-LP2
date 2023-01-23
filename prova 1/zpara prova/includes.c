#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

time_t t;
srand(time(&t));
srand(getpid()); // quando for com waitpid
long valor = rand() % 100;

pid_t *prcsf;
prcsf = (pid_t*)malloc(sizeof(pid_t)*quant_lebre);

argv[0] //guardar o nome do programa


pthread_t/ pid_t

pthread_create(&threads[],0 função_thread, (void*)indice);
void* retorno[qtd-threads];
pthread_join(threads[], &retorno[]/NULL);
pthread_exit((void*)valorpararetorno);
printf("%ld", (long)returno[]);


fork(); / getpid(); / getppid();
exit(valorretorno);
wait(&retorno/NULL);
waitpid(pid_filho, &armazenaretorno,0);
printf("%d", WEXITSTATUS(armazenaretorno));
kill(procs[], SIGHUP);
char* argumentos[3] = {"./pa-test", "STRING_TEST", NULL};
execv("./pa-test", argumentos[]); //nome do arquivo e depois argumentos


//https://github.com/guga06436/codigos-lp2/blob/main/primeira-unidade/thread/primeiro_exemplo_thread.c
