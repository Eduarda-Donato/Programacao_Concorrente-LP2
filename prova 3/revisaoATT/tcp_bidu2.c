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

#define MY_PORT 9002
#define QTD_CLIENTS 10
#define QTD_SHUTS 4
#define RECV_BUF_SIZE 1024

char buffer[RECV_BUF_SIZE];
int client_sock[QTD_CLIENTS];
pthread_barrier_t shut_barrier;

pthread_mutex_t buf_mutex;

void* shut_handler(){
    pthread_barrier_wait(&shut_barrier);
    printf("SHUTDOWN RECEIVED\n");
    exit(0);
}

void* client_handler(void* p) {
	int _client_sock = (int)(long)p;
	char recv_buffer[64];
    int s_ret = send(_client_sock, "START\n", 7, 0);
	while (1) {
		
		bzero(recv_buffer, 32);
		int r_ret = recv(_client_sock, recv_buffer,sizeof(recv_buffer), 0);
	    printf("Recebi do cliente (%d): %s\n", _client_sock, recv_buffer);

        if(strncmp(recv_buffer, "SHUTDOWN",8)==0){
            close(_client_sock);
            pthread_barrier_wait(&shut_barrier);
            pthread_exit(0);
        }
        pthread_mutex_lock(&buf_mutex);
        strcat(buffer, recv_buffer);
        strcat(buffer, "\n");
        pthread_mutex_unlock(&buf_mutex);
        for (int i = 0; i < QTD_CLIENTS; i++){
            int s_ret = send(client_sock[i], buffer,sizeof(buffer), 0);
        }	
	}
}

int main(void) {
	struct sockaddr_in server_info;
	struct sockaddr_in client_info[QTD_CLIENTS];
	int client_info_len[QTD_CLIENTS];
	int client_counter = 0;

	int server_sock;

	pthread_t client_threads[QTD_CLIENTS];
    pthread_t shut_threads;
    pthread_mutex_init(&buf_mutex, 0);

	printf("Servidor aguardando na porta: %d\n", MY_PORT);

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
    //trava a porta para ouvir conexão
	int l_ret = listen(server_sock, QTD_CLIENTS);

	if (l_ret == -1) {
		printf("listen() error (%d)!\n", errno);
		exit(0);
	}

	pthread_create(&shut_threads, 0,shut_handler, (void*)0);
    pthread_barrier_init(&shut_barrier, 0, QTD_SHUTS +1);
    

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
