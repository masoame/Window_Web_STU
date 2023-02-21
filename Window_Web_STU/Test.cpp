#include"Function.h"
#include<stdio.h>
char buffer[1024];

int Test() 
{
	WSAData wsad;
	WSAStartup(MAKEWORD(2, 2), &wsad);

	SOCKET tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in server;
	server.sin_addr.S_un.S_addr = inet_addr("47.113.190.21");
	server.sin_port = ntohs(9898);
	server.sin_family = AF_INET;

	sockaddr_in client;
	client.sin_addr.S_un.S_addr = 0;
	client.sin_port = ntohs(9898);
	client.sin_family = AF_INET;

	bind(tcp, (sockaddr*)&client, sizeof(sockaddr));
	//bind(udp, (sockaddr*)&client, sizeof(sockaddr));

	connect(tcp, (sockaddr*)&server, sizeof(sockaddr));

	Sleep(3000);
	int ret = sendto(udp, "buffer", 7, 0, (sockaddr*)&server, sizeof(sockaddr));
	printf("%d", ret);
	perror("");
	
	ret=recv(udp, buffer, 1024, 0);
	printf("%d:%s\n", ret, buffer);
	ret=recv(udp, buffer, 1024, 0);
	printf("%d:%s\n", ret, buffer);
	perror("");
	return 0;
}