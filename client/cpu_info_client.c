#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<stdlib.h>

#include "../include/error.h"

char* SERVER_HOST_NAME = "127.0.0.1";

int SERVER_HOST_PORT_NO = 8080;

char* get_cpu_info();
char* get_cpu_info_v2();

int main()
{
	
	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(0);
	
	//create a socket
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket < 0 )
	{
		fail("create client socket fail");
	}
	
	struct sockaddr_in server_addr;
	bzero((char *)&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	struct hostent *server;
	server = gethostbyname(SERVER_HOST_NAME);
	if(server == NULL)
	{
		fail("fail to get host name");
	}
	bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

	server_addr.sin_port = htons(atoi("8080"));
	socklen_t server_addr_len = sizeof(server_addr);

 	printf("connecting to %s, port=%d\n", inet_ntoa(server_addr.sin_addr.s_addr), server_addr.sin_port);
	if(connect(client_socket, (struct sockaddr*) &server_addr, server_addr_len) == -1 )
	{
		fail("connent to server fail");
	}

	char* content = get_cpu_info_v2();
	send(client_socket, content, strlen(content), 0);


	printf("send completed, size = %d\n", strlen(content));

	close(client_socket);
	

	return 0;
}


char* get_cpu_info()
{
	FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
	char *config = "\0";
	size_t size = 0;
	while(getdelim(&config, &size, 0, cpuinfo) != -1)
	{
		puts(config);
	}

	fclose(cpuinfo);


	return config;
}

char* get_cpu_info_v2()
{
	char buffer[1024];
	int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
	sprintf(buffer, "cpu num = %d\n", cpu_num);

	return buffer;
}

