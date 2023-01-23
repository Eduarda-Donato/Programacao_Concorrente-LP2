/*Faça um programa em C/C++ que crie um servidor TCP que aguarde conexões na porta
9000. Este servidor deverá ser capaz de receber e tratar conexões de até 10 
clientes simultâneos. Cada nova conexão deverá receber uma string de início de 
conexão (ex. “START”). Faça o programa gerenciar um buffer compartilhado composto
por até 1024 caracteres. Este buffer deverá ser modificado concatenando cada nova
string recebida de cada um dos clientes. Sempre que receber uma nova string de um
dos clientes, o servidor modifica o buffer e o envia para todos os cliente.

Exemplo de execução:
	3 clientes se conectam;
	servidor envia “START” para os 3 clientes;
	cliente 1 envia a string “ABC” para o servidor;
	servidor envia buffer para o cliente 1: “ABC”;
	cliente 2 envia a string “XYZ” para o servidor;
	servidor envia buffer para cliente 2: “ABC\nXYZ”
	cliente 3 envia a string “IJK” para o servidor;
	servidor envia buffer para cliente 3: “ABC\nXYZ\nIJK”;

Se 4 dos clientes enviarem a string “SHUTDOWN” o servidor deverá ser 
finalizado. Utilize uma barreira para implementar esta funcionalidade.

O programa deverá utilizar a biblioteca POSIX Threads (PThreads) ou as threads
STD (C++11) e a biblioteca sockets.h (ou winsock.h). A saída do seu programa 
deve ser bem planejada, de forma a mostrar o que está acontecendo em cada 
momento relevante. Para testes do servidor, usar netcat (nc) ou telnet para 
o lado do cliente.*/


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
#include <netdb.h>
#include <semaphore.h>
#include <time.h>

#define MY_PORT 9002
#define QTD_CLIENTS 10
#define RECV_BUF_SIZE 1024

int client_count = 0;
char* msgStart = "START\n";
int cont = 0;

char recv_buffer2[RECV_BUF_SIZE] = "";

/*typedef struct monitor{
    FILE *arq;
    sem_t mutex;
}t_monitor;

t_monitor monitor_escrever_arq;*/
pthread_barrier_t barreira;

void* client_handler(void* p){
    int client_sock = (int)(long)p;
    char* str = "\n";
    write(client_sock, msgStart, strlen(msgStart));
    
    while (1){
        char recv_buffer[100] = "";
        /*if(cont == 4){
            exit(0);
        }*/
        
        //int recv_ret = recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);
        int read_size = read(client_sock, recv_buffer, sizeof(recv_buffer));

        if (strncmp("EXIT\n", recv_buffer, 4)== 0){
            exit(0);
            //pthread_barrier_wait(&barreira);
            //cont++;
        }else{
            write(client_sock, str, strlen(str));

            printf("Recebi do cliente (%d): %s\n", client_sock, recv_buffer);
            //strcat(str, recv_buffer);
            strcat(recv_buffer2, recv_buffer);
            
            //int s_ret = send(client_sock, recv_buffer2, sizeof(recv_buffer2), 0);
            write(client_sock, recv_buffer2, strlen(recv_buffer2));
        }
        
        
        
    }
    
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in server_info;
    struct sockaddr_in client_info[QTD_CLIENTS];
    int client_info_len[QTD_CLIENTS];
    int client_counter = 0;

    int server_sock;
    int client_sock[QTD_CLIENTS];
    pthread_t client_threads[QTD_CLIENTS];

    int init_ret = pthread_barrier_init(&barreira, NULL, 4);

    if(init_ret != 0){
        printf("[main] erro ao inicializar barreira (%d)\n",init_ret);
    }

    printf("Servidor aguardando na porta: %d\n", MY_PORT);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if(server_sock == -1){
        printf("socket() error!");
        exit(0);
    }

    //informacoes do servidor
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(MY_PORT);
    server_info.sin_addr.s_addr = INADDR_ANY;

    //associo informacoes ao server socket 
    int bind_ret = bind(server_sock, (struct sockaddr*)&server_info, sizeof(server_info));

    if (bind_ret == -1) {
        printf("bind() error (%d)!\n", errno);
        exit(0);
    }

    //notifico SO - server socket
    int listen_ret = listen(server_sock, QTD_CLIENTS);

    if (listen_ret == -1) {
		printf("listen() error (%d)!\n", errno);
		exit(0);
	}

    //aguarda novas conexoes 
    while (1){
        client_sock[client_counter] = accept(server_sock, (struct sockaddr *)&client_info[client_counter], &client_info_len[client_counter]);

        //conexao recebida - estrutura com info do cliente populada
    
        //char* str_addr = inet_ntoa(client_info[client_counter].sin_addr);
        char str_addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_info->sin_addr), str_addr, INET_ADDRSTRLEN);

        printf("Recebi uma conexao!\n");
		printf("Endereco: %s\n", str_addr);

        if (client_sock[client_counter] == -1) {
			printf("accept() error (%d)!", 
					errno);
			exit(0);
		}

        pthread_create(&client_threads[client_counter], 0, client_handler, (void*)(long)client_sock[client_counter]);

        client_counter++;
    }
    //pthread_barrier_wait(&barreira);
    close(server_sock);
    
    return 0;
}
