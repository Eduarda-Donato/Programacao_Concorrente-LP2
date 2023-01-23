/*(Questão 1 - 10 pontos) Faça um programa em C/C++ que crie um servidor TCP que
atender os requisitos listados a seguir.

	1) Trate até 8 conexões simultâneas de clientes na porta 9900;

	2) Cada novo cliente deve receber uma string de boas vindas ao se conectar ao
	servidor (“BEM-VINDO”);

	3) O servidor tratará strings enviadas pelos clientes como comandos, e
	retornará o resultado para eles de acordo com os comandos, como descrito a
	seguir. Os seguintes comandos devem ser suportados:

		a) "RNDNUM\n" - o servidor irá gerar e retornar um valor aleatório
	        entre 0 e 1000;

		b) "CRTIME\n" - o servidor retornará a data e hora corrente;

		c) "FINISH\n" - o servidor finaliza a conexão do cliente;

		d) "SHTDWN\n" - o servidor está finalizado (fim do programa);

	4) O servidor deve armazenar todas as solicitações realizadas pelos clientes
	em um arquivo de texto chamado "log.txt". O acesso de escrita e leitura a esse
	arquivo deve ser feito através de um monitor.

A saída do seu programa deve ser bem eletrônica, de forma a mostrar o que está
controle em cada momento relevante. Teste o servidor com múltiplos clientes,
usando instâncias de telnet ou nc (netcat).*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>

#define MY_PORT 9900

#define QTD_CLIENTS 8

#define RECV_BUF_SIZE 32

typedef struct {
    char* filename;
    FILE* filedescript;
    pthread_mutex_t mutex;
}monitor;

monitor mon;

void* client_handler(void* p) {
	int client_sock = (int)(long)p;
    char str[100];
	char inicial[20] = "BEM-VINDO\n";
	send(client_sock, inicial, 20, 0);
	while (1) {
		char recv_buffer[RECV_BUF_SIZE];
		bzero(recv_buffer, RECV_BUF_SIZE);
		int r_ret = recv(client_sock, recv_buffer,
                        sizeof(recv_buffer), 0);
	    printf("Recebi do cliente (%d): %s\n", client_sock, recv_buffer);
		
        if (strncmp("RNDNUM", recv_buffer, 6)== 0){
            int num = rand() % 1000;
            sprintf(str, "numero aleatorio: %d\n", num);
            pthread_mutex_lock(&mon.mutex);
            fputs(str, mon.filedescript);
            pthread_mutex_unlock(&mon.mutex);
            
        }else if (strncmp("CRTIME", recv_buffer, 6)== 0){
            time_t now;
            time(&now);
            struct tm *local = localtime(&now);

            int hours = local->tm_hour;    
            int minutes = local->tm_min;        
            int seconds = local->tm_sec;        
            int day = local->tm_mday;            
            int month = local->tm_mon + 1;     
            int year = local->tm_year + 1900;

            sprintf(str, "Data e Hora: %02d:%02d:%02d %02d/%02d/%d\n", hours, minutes, seconds, day, month, year);

            pthread_mutex_lock(&mon.mutex);
            fputs(str, mon.filedescript);
            pthread_mutex_unlock(&mon.mutex);


        }else if (strncmp("FINISH", recv_buffer, 6)== 0){
            close(client_sock);
			pthread_exit(0);
        }else if (strncmp("SHTDWN", recv_buffer, 6)== 0){
            exit(0);
        }
	}
}

int main(void) {
	struct sockaddr_in server_info;
	struct sockaddr_in client_info[QTD_CLIENTS];
	int client_info_len[QTD_CLIENTS];
	int client_counter = 0;

	int server_sock;
	int client_sock[QTD_CLIENTS];
	pthread_t client_threads[QTD_CLIENTS];

    mon.filename = (char*) malloc(sizeof(char)*20);
    strcpy(mon.filename, "log.txt");
    mon.filedescript = fopen(mon.filename, "wt");

    pthread_mutex_init(&mon.mutex, NULL);

    time_t t;
    srand(time(&t));

	printf("Servidor aguardando na porta: %d\n", 
			MY_PORT);

	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (server_sock == -1) {
		printf("socket() error!\n");
		exit(0);
	}

	//informacoes do servidor
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(MY_PORT);
	server_info.sin_addr.s_addr = INADDR_ANY; 

	//associo informacoes ao server socket 
	int b_ret = bind(server_sock, 
			(struct sockaddr *)&server_info,
			sizeof(server_info));

	if (b_ret == -1) {
		printf("bind() error (%d)!\n", errno);
		exit(0);
	}

	//notifico SO - server socket
	int l_ret = listen(server_sock, QTD_CLIENTS);

	if (l_ret == -1) {
		printf("listen() error (%d)!\n", errno);
		exit(0);
	}

	//aguarda novas conexoes 
	
	while (1) {	
		client_sock[client_counter] = accept(
			server_sock, 
			(struct sockaddr *)
			&client_info[client_counter],
			&client_info_len[client_counter]);

		//conexao recebida - estrutura com info
		//do cliente populada

		char* str_addr = 
		  inet_ntoa(
		   client_info[client_counter].sin_addr);

		printf("Recebi uma conexao!\n");
		printf("Endereco: %s\n", str_addr);

		if (client_sock[client_counter] == -1) {
			printf("accept() error (%d)!", 
					errno);
			exit(0);
		}

		pthread_create(&client_threads[client_counter], 0,client_handler, (void*)(long)client_sock[client_counter]);

		client_counter++;
	}

	//fecha recepcao de novas conexoes
	close(server_sock);

	return 0;
}
