#include"Function.h"
#include<stdio.h>

int UDP::UDP_Client()
{
	WSAData wsadata;
	Check_Function(WSAStartup(MAKEWORD(2, 2), &wsadata))


	SOCKET local_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in server_addr;
	server_addr.sin_addr.S_un.S_addr = inet_addr("120.79.172.7");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9898);

	sendto(local_sock, NULL, 0, 0, (sockaddr*)&server_addr, sizeof(sockaddr));
	
	//创建Client结构体存储信息
	Client target_client;
	target_client.sockaddr_len = sizeof(sockaddr);
	target_client.sock = local_sock;
	recvfrom(local_sock, (char*)&target_client.sock_addr, sizeof(sockaddr), 0, (sockaddr*)&server_addr, &target_client.sockaddr_len);

	printf("%s: %d\n", inet_ntoa(target_client.sock_addr.sin_addr), htons(target_client.sock_addr.sin_port));

	HANDLE h_recv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_to_recv, &target_client, 0, NULL);
	HANDLE h_send = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_to_send, &target_client, 0, NULL);

	Signed_show(WaitForSingleObject(h_recv, INFINITE));
	Signed_show(WaitForSingleObject(h_send, INFINITE));

	closesocket(local_sock);
	WSACleanup();

	return 0;
}

int UDP::thread_to_send(LPVOID argv) {
	Client* client_taget = (Client*)argv;
	int ret;
	Sleep(1000);
	sendto(client_taget->sock, NULL, 0, 0, (sockaddr*)&client_taget->sock_addr, client_taget->sockaddr_len);
	while (true) {

		scanf_s("%s", buffer_send);
		
		if (!strcmp(buffer_send, "quit") || !strcmp(buffer_send, "QUIT")) {
			closesocket(client_taget->sock);
			WSACleanup();
			exit(-1);
		}
		ret=sendto(client_taget->sock, buffer_send, strlen(buffer_send)+1, 0, (sockaddr*)&client_taget->sock_addr, client_taget->sockaddr_len);
	}

	return 0;
}

int UDP::thread_to_recv(LPVOID argv) {
	Client* client_taget = (Client*)argv;

	int ret;

	while (true) {
		buffer_recv[0] = 1;
		ret = recvfrom(client_taget->sock, buffer_recv, 1024, 0, (sockaddr*)&client_taget->sock_addr, &client_taget->sockaddr_len);
		if (ret == -1) {
			continue;
		}
		else if (ret == 0) {
			sendto(client_taget->sock, "", 1, 0, (sockaddr*)&client_taget->sock_addr, client_taget->sockaddr_len);
			continue;
		}
		else if (buffer_recv[0] == 0) {
			printf("connect success\n");
			continue;
		}
		
		printf("[%s:%d]:%s\n", inet_ntoa(client_taget->sock_addr.sin_addr), htons(client_taget->sock_addr.sin_port), buffer_recv);
	}
	delete client_taget;
	return 0;
}



