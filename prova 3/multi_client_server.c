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

#define MY_PORT 9998

#define QTD_CLIENTS 5

#define RECV_BUF_SIZE 32


void* client_handler(void* p) {
	int client_sock = (int)(long)p;
	while (1) {
		char recv_buffer[RECV_BUF_SIZE];
		bzero(recv_buffer, RECV_BUF_SIZE);
		int r_ret = recv(client_sock, recv_buffer,
                        sizeof(recv_buffer), 0);
	        printf("Recebi do cliente (%d): %s\n", 
				client_sock, recv_buffer);
		printf("Ecoando mensagem...\n");
		int s_ret = send(client_sock, recv_buffer,
			sizeof(recv_buffer), 0);
		
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

		pthread_create(
  		    &client_threads[client_counter], 
		    0,
		    client_handler,
		    (void*)(long)
		    client_sock[client_counter]);

		client_counter++;
	}

	//fecha recepcao de novas conexoes
	close(server_sock);

	return 0;
}
