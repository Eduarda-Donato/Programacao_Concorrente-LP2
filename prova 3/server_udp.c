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

#define MY_PORT 9990
#define CLIENT_PORT 9991

#define RECV_BUF_SIZE 32

int main(void) {
	struct sockaddr_in server_info;
	struct sockaddr_in client_info;
	int client_info_len;
	int server_sock;
	int client_sock;

        char recv_buffer[RECV_BUF_SIZE];

	server_sock = socket(AF_INET, SOCK_DGRAM, 0);

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(MY_PORT);

	bind(server_sock, 
	     (struct sockaddr *)&server_info,
	     sizeof(server_info));

	int r_ret = recvfrom(server_sock, recv_buffer,
			RECV_BUF_SIZE, 0,
			(struct sockaddr *)&client_info,
			&client_info_len);

	printf("Recebi: %s\n", recv_buffer);

	client_sock = socket(AF_INET, SOCK_DGRAM, 0);

	client_info.sin_port = htons(CLIENT_PORT);

	int s_ret = sendto(client_sock, recv_buffer,
			RECV_BUF_SIZE, 0,
			(struct sockaddr *)&client_info,
	                 sizeof(client_info));

	close(server_sock);
	close(client_sock);

	return 0;


}
