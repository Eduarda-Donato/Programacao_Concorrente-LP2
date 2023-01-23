/*(Questão 2 - 4 pontos) Faça um monitor que controle o acesso de escrita e 
leitura em um arquivo de texto; modifique o código da questão anterior 
para que todos os comandos enviados pelos clientes sejam registrados em um
arquivo de texto cujo acesso é controlado pelo novo monitor implementado.*/

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

#define MY_PORT 9623
#define QTD_CLIENTS 10
#define RECV_BUF_SIZE 32

int clients_sock[QTD_CLIENTS];

typedef struct { //criação da estrutura do monitor para a manipulação de um arquivo
    char* filename;
    FILE* filedescript;
    pthread_mutex_t mutex;
}monitor;

monitor mon; //instancia 1 monitor, pois como é apenas 1 arquivo a ser manipulado, não tem necessidade de mais

void* client_handler(void* p) {
	int client_sock = (int)(long)p;
    char str[100];
    char recv_buffer[RECV_BUF_SIZE];

	recv(client_sock, recv_buffer,sizeof(recv_buffer), 0);
    if (!strncmp("START", recv_buffer, 5)== 0){
            close(client_sock);
			pthread_exit(0);
    }
	while (1) {
		bzero(recv_buffer, RECV_BUF_SIZE);
		int r_ret = recv(client_sock, recv_buffer,
                        sizeof(recv_buffer), 0);
	    printf("Recebi do cliente (%d): %s\n", client_sock, recv_buffer);

        if (strncmp("CRTIME", recv_buffer, 6)== 0){
            time_t tt;
            time(&tt);
            struct tm *local = localtime(&tt);

            int hora = local->tm_hour;    
            int minuto = local->tm_min;        
            int segundo = local->tm_sec;        
            int dia = local->tm_mday;            
            int mes = local->tm_mon + 1;     
            int ano = local->tm_year + 1900;

            sprintf(str, "Data e Hora: %02d:%02d:%02d %02d/%02d/%d\n", hora, minuto, segundo, dia, mes, ano);

            //sempre que o servidor for manipular o arquivo (seção critica) ele vai tratar com mutex para evitar inconsistencia
            //de memoria no arquivo (uma thread escrever em cima do que a outra escreveu no arquivo)
            pthread_mutex_lock(&mon.mutex);         //bloquei para uso de apenas 1 thread   
            fputs(recv_buffer, mon.filedescript);
            pthread_mutex_unlock(&mon.mutex);       //libera
            send(client_sock, str, 33, 0);

        }else if (strncmp("RNDNUM", recv_buffer, 6)== 0){
            int num = rand() % 100;
            sprintf(str, "numero aleatorio: %d\n", num);
            pthread_mutex_lock(&mon.mutex);
            fputs(recv_buffer, mon.filedescript);
            pthread_mutex_unlock(&mon.mutex);
            send(client_sock, str, 22, 0);
            
        }else if (strncmp("FINISH", recv_buffer, 6)== 0){
            pthread_mutex_lock(&mon.mutex);
            fputs(recv_buffer, mon.filedescript);
            pthread_mutex_unlock(&mon.mutex);
            close(client_sock);
			pthread_exit(0);
        }else if (strncmp("SHTDWN", recv_buffer, 6)== 0){
            pthread_mutex_lock(&mon.mutex);
            fputs(recv_buffer, mon.filedescript);
            pthread_mutex_unlock(&mon.mutex);
            char buffer[RECV_BUF_SIZE] = "servidor será finalizado\n";
            for (int i = 0; i < QTD_CLIENTS; i++){
                int s_ret = send(clients_sock[i], buffer,sizeof(buffer), 0);
            }	
            exit(0);
            exit(0);
        }else{
            send(client_sock, "ERRO\n", 5, 0);
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

    mon.filename = (char*) malloc(sizeof(char)*20); //aloca memoria para o nome do arquivo
    strcpy(mon.filename, "log.txt");                //define o nome do arquivo
    mon.filedescript = fopen(mon.filename, "wt");   //abre o arquivo

    pthread_mutex_init(&mon.mutex, NULL);           //inicia o mutex

    time_t t;
    srand(time(&t));

	printf("Servidor aguardando na porta: %d\n", 
			MY_PORT);

	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (server_sock == -1) {
		printf("socket() error!\n");
		exit(0);
	}

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(MY_PORT);
	server_info.sin_addr.s_addr = INADDR_ANY; 

	int b_ret = bind(server_sock, 
			(struct sockaddr *)&server_info,
			sizeof(server_info));

	if (b_ret == -1) {
		printf("bind() error (%d)!\n", errno);
		exit(0);
	}

	int l_ret = listen(server_sock, QTD_CLIENTS);

	if (l_ret == -1) {
		printf("listen() error (%d)!\n", errno);
		exit(0);
	}
	
	while (1) {	
		clients_sock[client_counter] = accept(server_sock, (struct sockaddr *)&client_info[client_counter],&client_info_len[client_counter]);

		char* str_addr = inet_ntoa(client_info[client_counter].sin_addr);

		printf("Recebi uma conexao!\n");
		printf("Endereco: %s\n", str_addr);

		if (clients_sock[client_counter] == -1) {
			printf("accept() error (%d)!", errno);
			exit(0);
		}

		pthread_create(&client_threads[client_counter], 0,client_handler, (void*)(long)clients_sock[client_counter]);

		client_counter++;
	}

	//fecha recepcao de novas conexoes
	close(server_sock);

	return 0;
}
