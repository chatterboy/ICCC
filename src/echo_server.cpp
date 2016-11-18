#include "arpa/inet.h"
#include "unistd.h"

#include <bits/stdc++.h>
using namespace std;

const int BUF_LEN = 128;

int main(int argc, char **argv) {
	int len;
	int msg_size;
	int server_fd;
	int client_fd;
	char temp[20];
	char buffer[BUF_LEN];
	sockaddr_in server_addr;
	sockaddr_in client_addr;
	if (argc != 2) {
		printf("usage: %s [port]\n", argv[0]);
		exit(0);
	}
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("server: can't open stream socket\n");
		exit(0);
	}
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("server: can't bind local address\n");
		exit(0);
	}
	if (listen(server_fd, 5) < 0) {
		printf("server: can't listening connect\n");
		exit(0);
	}
	memset(buffer, 0x00, sizeof(buffer));
	printf("server: waiting connection request\n");
	len = sizeof(client_addr);
	while (1) {
		client_fd = accept(server_fd, (sockaddr *)&client_addr, (socklen_t *)&len);
		if (client_fd < 0) {
			printf("server: accept failed\n");
			exit(0);
		}
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
		printf("server: %s client connected\n", temp);
		msg_size = read(client_fd, buffer, 1024);
		write(client_fd, buffer, msg_size);
		printf("server: %s client closed\n", temp);
	}
	printf("never...");
	close(client_fd);
	close(server_fd);
	return 0;
}
