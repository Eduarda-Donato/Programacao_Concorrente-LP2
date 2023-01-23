/*(Questão 1 - 8 pontos) Faça um programa em C/C++ que implemente um servidor
que atendem os requisitos listados a seguir.
 
	1) Receba até 10 conexões simultâneas de clientes na porta 9622 TCP;
 
	2) Cada cliente deve enviar uma string ao se conectar ao servidor: 
	"START" para que a comunicação seja iniciada. Caso o cliente inveje
	outra string, a conexão com ele deve ser finalizada.
 
	3) O servidor tratará strings enviadas pelos clientes como comandos, e 
	retornar o resultado como descrito a seguir. Os seguintes comandos
	devem ser suportados: 
 
		a) CRTIME\n fará o servidor retornar a data e hora corrente;
 
		b) RNDNUM\no servidor irá gerar e retornar um valor aleatório 
		entre 0 e 100;
 
		c) FINISH\nfará o servidor finalizar a conexão com o cliente;
 
		d) SHTDWN\n envia mensagem para todos os clientes informando 
		que o servidor será finalizado; finaliza o processo do servidor;
 
		e) Comandos não reconhecidos obrigatórios fazer o servidor retornar
		"ERRO" para os clientes.
 
Comente o código explicando a diferença entre os papéis de cliente e servidor, 
ressaltando quais chamadas feitas no código são necessárias no contexto 
específico do protocolo TCP. Teste o servidor com vários clientes, usando
instâncias de telnet ou nc (netcat). Explique também como diferença com relação
ao usar sockets com TCP e UDP.*/

//bibliotecas utilizadas
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

//constantes
#define MY_PORT 9622
#define QTD_CLIENTS 10
#define RECV_BUF_SIZE 32

int clients_sock[QTD_CLIENTS]; 

//função das threads
void* client_handler(void* p) {
	int client_sock = (int)(long)p;
    char str[100];
    char recv_buffer[RECV_BUF_SIZE];

	recv(client_sock, recv_buffer,sizeof(recv_buffer), 0);
    if (!strncmp("START", recv_buffer, 5)== 0){ //espera pelo START inicial, se não receber fecha o cliente
            close(client_sock);
			pthread_exit(0);
    }
    
	while (1) {  
		bzero(recv_buffer, RECV_BUF_SIZE);
		int r_ret = recv(client_sock, recv_buffer,sizeof(recv_buffer), 0); //recebe o que o cliente mandou
	    printf("Recebi do cliente (%d): %s\n", client_sock, recv_buffer);  //e exibe no servidor
 
        if (strncmp("CRTIME", recv_buffer, 6)== 0){  //se o cliente digitar CRTIME ele vai retornar ao cliente data e hora
            time_t tt;
            time(&tt);
            struct tm *local = localtime(&tt);

            int hora = local->tm_hour;    
            int minuto = local->tm_min;        
            int segundo = local->tm_sec;        
            int dia = local->tm_mday;            
            int mes = local->tm_mon + 1;     
            int ano = local->tm_year + 1900;

            sprintf(str, "Data e Hora: %02d:%02d:%02d %02d/%02d/%d\n", hora, minuto, segundo, dia, mes, ano); //converte para string já no format

            send(client_sock, str, 33, 0); //envia para 

        }else if (strncmp("RNDNUM", recv_buffer, 6)== 0){  //se o cliente digitar RNDNUM ele vai retornar ao cliente um numero aleatorio
            int num = rand() % 100;  //randomiza o numero entre 0 e 100
            sprintf(str, "numero aleatorio: %d\n", num);
    
            send(client_sock, str, 22, 0);  //envia para o cliente
            
        }else if (strncmp("FINISH", recv_buffer, 6)== 0){  //se o cliente digitar FINISH ele vai matar o cliente, mas o servidor permanece
            close(client_sock);     //fecha o cliente
			pthread_exit(0);        

        }else if (strncmp("SHTDWN", recv_buffer, 6)== 0){       //se o cliente digitar SHTDWN ele vai matar o servidor e consequentemente os clientes, porém avisa os clientes antes de finalizar
            char buffer[RECV_BUF_SIZE] = "servidor será finalizado\n";
            for (int i = 0; i < QTD_CLIENTS; i++){              //percorre por todos os clientes e envia a mensagem de finalização
                int s_ret = send(clients_sock[i], buffer,sizeof(buffer), 0);
            }	
            exit(0);

        }else{
            send(client_sock, "ERRO\n", 5, 0);
        }
	}
}

int main(void) {
	struct sockaddr_in server_info; //serve para guarda dados de conexão
	struct sockaddr_in client_info[QTD_CLIENTS]; //o servidor guarda as informações de todos os clientes, já o cliente só guarda a do servidor
	int client_info_len[QTD_CLIENTS];
	int client_counter = 0;

	int server_sock;
	pthread_t client_threads[QTD_CLIENTS];

    time_t t;
    srand(time(&t));

	printf("Servidor aguardando na porta: %d\n", 
			MY_PORT);

	server_sock = socket(AF_INET, SOCK_STREAM, 0); //cria um servidor tcp pois usou SOCK_STREAM se fosse udp seria SOCK_DGRAM

	if (server_sock == -1) {
		printf("socket() error!\n");
		exit(0);
	}

	//informacoes do servidor
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(MY_PORT);
	server_info.sin_addr.s_addr = INADDR_ANY; 

	//associa a porta as informacoes ao server socket, necessario no servidor, mas não no cliente pois ele recebe tambem no connect, e no udp tambem não é obrigatorio seu uso
	int b_ret = bind(server_sock, (struct sockaddr *)&server_info, sizeof(server_info));  

	if (b_ret == -1) {
		printf("bind() error (%d)!\n", errno);
		exit(0);
	}

	//notifica SO que é um servidor e vai escutar em uma nova porta
	int l_ret = listen(server_sock, QTD_CLIENTS);

	if (l_ret == -1) {
		printf("listen() error (%d)!\n", errno);
		exit(0);
	}

	//aguarda novas conexoes 
	
	while (1) {	
        //o servidor vai ser bloqueado esperando por nova conexão, no caso um connect de um cliente
        //essa é a principal diferença ente o cliente e o servidor: o servidor já em execução aguarda o cliente se conectar
        //e enviar suas informações enquanto que o cliente vai enviar ou solicitar informações para e/ou de um servidor já
        //ativo
		clients_sock[client_counter] = accept(server_sock, (struct sockaddr *)&client_info[client_counter],&client_info_len[client_counter]);

		char* str_addr = 
		  inet_ntoa(
		   client_info[client_counter].sin_addr);

		printf("Recebi uma conexao!\n");
		printf("Endereco: %s\n", str_addr);

		if (clients_sock[client_counter] == -1) {
			printf("accept() error (%d)!", 
					errno);
			exit(0);
		}

		pthread_create(&client_threads[client_counter], 0,client_handler, (void*)(long)clients_sock[client_counter]);

		client_counter++;
	}

	//fecha recepcao de novas conexoes
	close(server_sock);

    //as principais diferenças entre o tcp e o udp são que o tcp necessita de uma solicitação de conexão, pois é a partir dela que ele
    //garante a entrega e a orgem de entrega das informações, evitando qualquer perda e tal fato o torna mais lento, além disso o tcp
    //sempre checka se a conexão está ativa entre servidor e cliente
    //já o udp ele não é orientado a conexão, o que causa perca de informações, pois não sabe se a mensagem foi entregue e nem se 
    //a conexão está ativa, como não garante tudo isso sua conexão é mais rapida
    //Outra caracteristica que se diferencia entre o tcp e udp é que os códigos de server e cliente no udp são bastate parecidos apenas
    //mudam a ordem do sendto e recvfrom

	return 0;
}
