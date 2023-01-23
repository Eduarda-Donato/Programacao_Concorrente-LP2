
//cada cliente se conecta apenas a 1 servidor 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#define SERVER_PORT 9998
#define SERVER_ADDR "localhost"
int main(void) {
    char recv_buffer[32];
    struct sockaddr_in server_info;
	int client_sock;
	struct hostent *hp;
	char* client_msg = "MENSAGEM DO CLIENTE";

	hp = gethostbyname(SERVER_ADDR);

	client_sock = socket(AF_INET, SOCK_STREAM, 0);

	server_info.sin_family = AF_INET;
        server_info.sin_port = htons(SERVER_PORT);
        server_info.sin_addr.s_addr = *((unsigned long *)hp->h_addr);


	printf("Cliente conectando a %s\n", SERVER_ADDR);

	int r_con = connect(client_sock,(struct sockaddr *)&server_info,sizeof(server_info));

	if (r_con == -1) {
		printf("Connection error (%d)!\n",errno);
	       	exit(0);
	}

	printf("Conectado ao servidor!\n");

        int r_ret = recv(client_sock, recv_buffer,sizeof(recv_buffer), 0);

	printf("Recebi do servidor: %s\n", recv_buffer);

	printf("Enviando para o servidor: %s\n", 
					client_msg);

	int s_ret = send(client_sock, client_msg,strlen(client_msg), 0);

	close(client_sock);

	return 0;
}



