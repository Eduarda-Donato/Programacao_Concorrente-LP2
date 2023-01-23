/*(Questão 2 - 4 pontos) Faça um programa em C/C++ que possa ser usado como
cliente do servidor implementado na questão anterior. Comente o código
explicando a diferença entre os papéis de cliente e servidor, ressaltando o que
chamadas feitas no código são necessárias no contexto específico do protocolo TCP.*/
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

#define TAM_MSG 100

#define SERVER_ADDR "localhost"

int main(void) {
    char recv_buffer[32];
    struct sockaddr_in server_info;
	struct sockaddr_in recv_info;
	int client_sock;
	struct hostent *hp;
    int porta_servidor;
	char* msg_cliente;
	int tam;

    printf("Digite a porta usada na conexao: ");
    scanf("%d", &porta_servidor);

	hp = gethostbyname(SERVER_ADDR);

	client_sock = socket(AF_INET, SOCK_STREAM, 0);

	server_info.sin_family = AF_INET;
    server_info.sin_port = htons(porta_servidor);
    server_info.sin_addr.s_addr = *((unsigned long *)hp->h_addr);


	printf("Cliente conectando a %s\n", SERVER_ADDR);

	int r_con = connect(client_sock,(struct sockaddr *)&server_info,sizeof(server_info));

	if (r_con == -1) {
		printf("Connection error (%d)!\n",errno);
	       	exit(0);
	}

	printf("Conectado ao servidor!\n");

	recvfrom(client_sock, &recv_buffer, TAM_MSG, 0, (struct sockaddr *)&recv_info, &tam);
	printf("Mensagem do servidor: %s\n", recv_buffer);

	while (1){
        msg_cliente = (char*)malloc(sizeof(char)* TAM_MSG);
        printf("Digite uma mensagem: ");
        scanf(" %s", msg_cliente);
		sendto(client_sock, (void*)msg_cliente, strlen(msg_cliente),0, (struct sockaddr *)&server_info, sizeof(server_info));
		tam = sizeof(recv_info);
		
		if (strncmp("FINISH", msg_cliente, 6)== 0){
            exit(0);
        }else if (strncmp("SHTDWN", msg_cliente, 6)== 0){
            exit(0);
        }
		free(msg_cliente);

		
    }

	close(client_sock);

	return 0;
}
