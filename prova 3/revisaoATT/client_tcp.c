/* Faça um programa em C/C++ que implemente um cliente para
o servidor da primeira questão da revisão. Faça o programa 
solicitar para o usuário a mensagem a enviar e exiba as
mensagens recebidas do servidor. O programa deverá ser 
finalizado caso o usuário envie a string "EXIT".
*/
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

#define TAM_MSG 1024
#define SERVER_ADDR "localhost"

char recv_buffer[TAM_MSG];

int main(int argc, char const *argv[]){
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

    bzero(&server_info, sizeof(server_info));

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(porta_servidor);
    server_info.sin_addr.s_addr = *((unsigned long *)hp->h_addr);

    printf("Cliente conectando a %s\n", SERVER_ADDR);

    int connect_ret = connect(client_sock, (struct sockaddr *)&server_info, sizeof(server_info));

    if (connect_ret == -1) {
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
        if(strncmp("EXIT", msg_cliente, 4)== 0){
            exit(0);
        }else{
            sendto(client_sock, (void*)msg_cliente, strlen(msg_cliente),0, (struct sockaddr *)&server_info, sizeof(server_info));
            tam = sizeof(recv_info);
            recvfrom(client_sock, &recv_buffer, TAM_MSG, 0, (struct sockaddr *)&recv_info, &tam);
            printf("Mensagem do servidor: \n%s\n", recv_buffer);
            free(msg_cliente);
        }
        
    }

    close(client_sock);
    return 0;
}
