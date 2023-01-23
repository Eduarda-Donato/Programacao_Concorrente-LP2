#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MY_PORT 9999

int main(void) {
	unsigned short port;
	char recv_buffer[32];
	struct sockaddr_in server_info; //vai servir para guarda dados de conexão
	struct sockaddr_in client_info;
	int client_len;

	int server_sock; //descritor de arquivos é int pq a função socket() retorna interio
	int client_sock;

	printf("Servidor aguardando na porta: %d\n", 
			MY_PORT);

	server_sock = socket(AF_INET, SOCK_STREAM, 0); 	//protocolo de rede, tcp, flag 
													//socket criado
	if (server_sock == -1) {
		printf("socket() error!\n");
		exit(0);
	}

	//informacoes do servidor
	server_info.sin_family = AF_INET; //qual tipo de socket
	server_info.sin_port = htons(MY_PORT); //qual porta queremos
	server_info.sin_addr.s_addr = INADDR_ANY;  //qual interface de rede

	//associo informacoes ao server socket 
	//tenta avrir a porta com o bind
	int b_ret = bind(server_sock, (struct sockaddr *)&server_info,sizeof(server_info)); //escolhe a porta

	if (b_ret == -1) {
		printf("bind() error (%d)!\n", errno);
		exit(0);
	}

	//notifico SO - server socket
	int l_ret = listen(server_sock, 3); //referencia ao socket, numero de conexão pendentes q vc pode ter (fila FIFO com conexão pendentes)
										//ouvindo na porta e esperando conexões
										 
	if (l_ret == -1) {
		printf("listen() error (%d)!\n", errno);
		exit(0);
	}

	//aguarda novas conexoes 
	client_sock = accept(server_sock, (struct sockaddr *)&client_info, &client_len);
	//referencia ao socket, endereço de uma struct onde vai colocar os dados dos clients (pq vc vai receber uma conexao e vai saber o ip de quem veio)
	//accept retorna um ponteiro para outra conexão e é nela q vc vai ler e escrever
	//accept bloqueia ate vc receber um pedido de conexao
	//um client_sock diferente p cada cliente que esta se conectando
	//client_sock é o ponto em que vai criar a conexao com o cliente, server_sock vai ficar para receber conexão novas e vai redirecionar a conexao pro cliente_sock (tipo triagem)

	//conexao recebida - estrutura com info do cliente populada
	//identificação do ciente
	char* str_addr = inet_ntoa(client_info.sin_addr);

	printf("Recebi uma conexao!\n");
	printf("Endereco: %s\n", str_addr);

	if (client_sock == -1) {
		printf("accept() error (%d)!", errno);
		exit(0);
	}

	//envio e recepcao de informacoes 
	//(cliente conectado)
	char send_buf[12] = "Bem vindo!\n";

	printf("Enviando para o cliente: %s\n",
				send_buf);

	int s_ret = send(client_sock, send_buf, sizeof(send_buf), 0);
	//ponteiro da conexão com o cliente q vc ta conversando, a string q vc quer enviar, numero de bites da string, flag


	if (s_ret == -1) { //geralmente sig q ocliente fechou o socket do lado dele  
		printf("send() error (%d)!\n",errno);
	}

	int r_ret = recv(client_sock, recv_buffer,sizeof(recv_buffer), 0);
	//ponteiro da conexão com o cliente q vc ta conversando,  endereço de um buffer de entrada, num de bites que quer receber, flag

	printf("Recebi do cliente: %s\n", recv_buffer);


	//fecha conexao com cliente
	close(client_sock);

	//fecha recepcao de novas conexoes
	close(server_sock);

	return 0;
}
