#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>

#define SERVER_PORT 9990
#define MY_PORT 9991

#define RECV_BUF_SIZE 32

int main(void) {
	struct sockaddr_in server_info;
	int server_sock;
	int server_info_len;

    char *send_buffer = "MENSAGEM CLIENTE";
	//char buffer[32];

	server_sock = socket(AF_INET, SOCK_DGRAM, 0);

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(SERVER_PORT);
/*
	bind(server_sock, 
	     (struct sockaddr *)&server_info,
	     sizeof(server_info));
*/
	int s_ret = sendto(server_sock, send_buffer,
			strlen(send_buffer), 0,
             (struct sockaddr *)&server_info,
             sizeof(server_info));

	/*int r_ret = recvfrom(server_sock, buffer,
			32, 0,
			(struct sockaddr *)&server_info,
			&server_info_len);*/


	close(server_sock);

	return 0;


}
