#include"Function.h"
#include<stdio.h>

int UDP::UDP_Client()
{
	WSAData wsadata;
	Check_Function(WSAStartup(MAKEWORD(2, 2), &wsadata))


	SOCKET local_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


	sockaddr_in server_addr;
	server_addr.sin_addr.S_un.S_addr = inet_addr("47.113.190.21");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9898);

	sendto(local_udp, NULL, 0, 0, (sockaddr*)&server_addr, sizeof(sockaddr));

	//创建Client结构体存储信息
	Client target_client;
	target_client.sockaddr_len = sizeof(sockaddr);
	target_client.sock = local_udp;


	recvfrom(local_udp, (char*)&target_client.sock_addr, sizeof(sockaddr), 0, (sockaddr*)&server_addr, &target_client.sockaddr_len);
	printf("%s: %d\n", inet_ntoa(target_client.sock_addr.sin_addr), htons(target_client.sock_addr.sin_port));

	HANDLE h_recv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_to_recv, &target_client, 0, NULL);
	HANDLE h_send = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_to_send, &target_client, 0, NULL);
	HANDLE h_headth = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HeartBeat, &target_client, 0, NULL);


	Signed_show(WaitForSingleObject(h_recv, INFINITE));
	Signed_show(WaitForSingleObject(h_send, INFINITE));
	Signed_show(WaitForSingleObject(h_headth, INFINITE));



	closesocket(local_udp);
	WSACleanup();

	return 0;
}

int UDP::thread_to_send(LPVOID argv) {
	Client* client_taget = (Client*)argv;
	int ret;
	while (true) 
	{

		scanf_s("%s", buffer_send);
		
		if (!strcmp(buffer_send, "#quit") || !strcmp(buffer_send, "#QUIT")) {
			sendto(client_taget->sock, buffer_send, strlen(buffer_send) + 1, 0, (sockaddr*)&client_taget->sock_addr, client_taget->sockaddr_len);
			closesocket(client_taget->sock);
			WSACleanup();
			exit(0);
		}
		ret = sendto(client_taget->sock, buffer_send, strlen(buffer_send) + 1, 0, (sockaddr*)&client_taget->sock_addr, client_taget->sockaddr_len);
	}

	return 0;
}

int UDP::thread_to_recv(LPVOID argv) {
	Client* client_taget = (Client*)argv;

	int ret;

	while (true) 
	{
		buffer_recv[0] = 1;
		ret = recvfrom(client_taget->sock, buffer_recv, 1024, 0, (sockaddr*)&client_taget->sock_addr, &client_taget->sockaddr_len);

		if (ret == -1) {
			continue;
		}
		else if (ret == 0) {
			
		}
		else if (buffer_recv[0] == '#' ) {
			if (ret == 3) {
				if (buffer_recv[1] == '0' || buffer_recv[1] == '1' || buffer_recv[1] == '2') {
					buffer_recv[1]++;
					sendto(client_taget->sock, buffer_recv, 3, 0, (sockaddr*)&client_taget->sock_addr, client_taget->sockaddr_len);
					if (buffer_recv[1] == '1') 	islink[0] = 1;
				}
			}
			else if(!strcmp(buffer_recv, "#quit") || !strcmp(buffer_recv, "#QUIT")) {
				closesocket(client_taget->sock);
				WSACleanup();
				exit(0);
			}
			continue;
		}
		
		printf("[%s:%d]:%s\n", inet_ntoa(client_taget->sock_addr.sin_addr), htons(client_taget->sock_addr.sin_port), buffer_recv);
	}
	delete client_taget;
	return 0;
}

void UDP::HeartBeat(LPVOID argv) {
	Sleep(1000);
	Client* client_taget = (Client*)argv;
	int ret;
	islink[1] = 3;
	while (islink[1])
	{
		islink[1]--;
		islink[0] = 0;
		ret = sendto(client_taget->sock, "#0", 3, 0, (sockaddr*)&client_taget->sock_addr, client_taget->sockaddr_len);
		if (ret == -1) continue;
		Sleep(4000);
		if (!islink[0]) continue;
		else islink[1] = 3;
		
	}
	closesocket(client_taget->sock);
	WSACleanup();
	exit(-1);
}

