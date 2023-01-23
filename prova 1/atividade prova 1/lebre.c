/*
(Questão 5) Faça um programa em C/C++ que receberá 3 parâmetros: 
se usará processos ou threads, quantas instâncias serão criadas e um valor em 
metros, que será o tamanho de uma pista fictícia para uma corrida de 'lebres'.

Exemplo de execução:

	$ ./corrida -t 5 1000 (5 threads, pista de 1000m)
	lebre 0 saltou 30cm (total: 30)
	lebre 2 saltou 42cm (total: 42)
	...
	lebre 2 saltou 31cm (total: 1002)
	lebre 2 venceu! 

	$ ./corrida -p 10 500 (10 processos, pista de 500m)
	lebre 1 saltou 20cm (total: 20)
	lebre 0 saltou 43cm (total: 43)
	...
	lebre 3 saltou 44cm (total: 520)
	lebre 3 venceu!

Os processos / threads vão representar lebres em uma corrida fictícia; eles 
devem executar uma função que receberá dois parâmetros: um valor inteiro que
identifique a lebre (índice) e um valor real (float ou double) que represente
a distância máxima do salto dessa lebre. A função possui um loop onde cada 
salto acontece a partir da geração de um valor aleatório entre 1 e o valor 
máximo definido para a "lebre", considerando valores expressos em cm. 

Faça as lebres "correrem" nessa "pista" com o tamanho definido na execução do
programa. Para cada vez que a função de cada processo/thread (lebre) executar
a geração de valores aleatórios (entre 1 e a distância máxima do salto da lebre),
uma soma de todos os saltos deverá ser armazenada em uma variável com o quanto a
lebre já percorreu na pista (uma variável float ou double no escopo da função 
que a lebre executa). A função permanece em loop, "saltando", até que o valor
acumulado seja igual ou superior ao tamanho da pista.

A cada "salto" (iteração realizada pela função associada ao processo ou a 
thread), deverá ser exibida uma mensagem contendo a distância do salto 
efetuado e qual distância total percorrida (dormindo 1s - função sleep - até 
que dê outro salto). A primeira 'lebre' (thread ou processo) que finalizar sua
função (ou seja, tenha a variável que represente o quanto a lebre já percorreu
com valor maior ou igual ao tamanho da pista) deverá exibir uma mensagem onde 
se declara vencedora e o programa deverá ser finalizado. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define MODO_THREAD 0
#define MODO_PROCESSO 1

#define SALTO_MAX_LEBRE 20

int modo;
int tam_pista;
int quant_lebre;

void* lebre(void* pt){
	long id = (long) pt;
	int soma_salto = 0;
	int salto_max = rand() % SALTO_MAX_LEBRE;
	printf("[lebre-%ld] iniciou\n", id);
	while (soma_salto < tam_pista) {
		int salto = (rand() % salto_max)+1;
		soma_salto += salto;
		printf("[lebre-%ld] saltou %d cm (%d)\n",
				id, salto, soma_salto);
	}
	//usleep(10);
	printf("[lebre-%ld] vencedora!\n", id);
	//fflush(stdout);
	exit(0);
}


int main(int argc, char* argv[]){
	if(argc < 4){
		printf("Falta Argumentos\n");
	}
	if(argv[1][0] == 'p'){
		modo = MODO_PROCESSO;
	}
	else if(argv[1][0] == 't'){ 
		MODO_THREAD;
	}else{
		printf("MODO INVALIDO\n");
		exit(-1);
	}
	quant_lebre = atoi(argv[2]); 
	tam_pista = atoi(argv[3]);
	printf("Modo: %d\n", modo);
	printf("Quantidade: %d\n", quant_lebre);
	printf("Tamanho: %d cm\n", tam_pista);	

	//time_t tempo;
	//srand(time(&tempo));

	if(modo == MODO_THREAD){
		pthread_t *thrs;
		thrs = (pthread_t*)malloc(sizeof(pthread_t)*quant_lebre);
		//corrida thread
	
		for (long i = 0; i < quant_lebre; i++){
			pthread_create(&thrs[i],0,lebre,(void*) i);
		}
		for (long i = 0; i < quant_lebre; i++){
			pthread_join(thrs[i],NULL);
		}
	}else{
		pid_t orig_pid;
		orig_pid = getpid();
		pid_t *prcsf;
		prcsf = (pid_t*)malloc(sizeof(pid_t)*quant_lebre);

		int forkreturn;
		for (long i = 0; i < quant_lebre; i++){
			if(getpid() == orig_pid){
				int forkreturn = fork();
				if(getpid() == orig_pid){
					prcsf[i] = forkreturn;
				}
			}else{
				lebre((void*)i);
			}	
		}
		if (getpid() == orig_pid){
			wait(NULL);
			printf("[main] chegada detectada!\n");
			for (int i = 0; i < quant_lebre; i++){
				kill(prcsf[i], SIGHUP);
			}
			exit(0);
		}
		
		
		
	}
	
	
	return 0;
}