#include"Function.h"
#include<stdio.h>

#pragma comment(lib,"ws2_32.lib")

int TCP::TCP_Client()
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data)) {
		MessageBoxW(NULL, L"WSAStartup��������ʧ��", L"message", MB_OK | MB_ICONERROR);
		return -1;
	}

	SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET) {
		MessageBoxW(NULL, L"�����׽���ʧ��", L"message", MB_OK | MB_ICONERROR);
		return -1;
	}

	sockaddr_in ip_adress;
	ip_adress.sin_family = AF_INET;

	ip_adress.sin_addr.S_un.S_addr = inet_addr("47.113.190.21");

	ip_adress.sin_port = htons(9898);


	if (connect(server, (sockaddr*)&ip_adress, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		MessageBoxW(NULL, L"����������ʧ��", L"message", MB_OK | MB_ICONERROR);
		return -1;
	}
	else {
		MessageBoxW(NULL, L"���������ӳɹ�", L"message", MB_OK);
		printf("�������û���");
	}

	HANDLE hsend = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Send_Message, (void*)&server, 0, NULL);

	HANDLE hdown = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvMessage, (void*)&server, 0, NULL);

	WaitForSingleObject(hsend, INFINITE);
	WaitForSingleObject(hdown, INFINITE);

	closesocket(server);
	if (WSACleanup()) {
		MessageBoxW(NULL, L"���绷������ʧ��", L"message", MB_OK | MB_ICONERROR);
	}
	printf("\n���������˳�\n");
}

bool TCP::Send_Message(void* server) {

	SOCKET taget = *((SOCKET*)server);
	char send_buf[1024];
	while (1)
	{
		scanf_s("%s", send_buf);
		if (!strcmp("#quit", send_buf) || !strcmp("#QUIT", send_buf)) {

			closesocket(taget);
			exit(0);
		}

		if (send(taget, send_buf, strlen(send_buf), 0) < 0) {

			MessageBoxW(NULL, L"��Ϣ����ʧ�ܣ�������", L"message", MB_OK | MB_ICONERROR);
		}
		if (send(taget, send_buf, strlen(send_buf)+1, 0) < 0) {

			MessageBoxW(NULL, L"��Ϣ����ʧ�ܣ�������", L"message", MB_OK | MB_ICONERROR);
		}

	}

}
bool TCP::RecvMessage(void* server){

	SOCKET taget = *((SOCKET*)server);
	int len;
	char recv_buf[1024];
	while (1)
	{
		len = recv(taget, recv_buf, sizeof(recv_buf) - 1, 0);
		if (len == -1) return -1;
		recv_buf[len] = 0;
		printf("%d\n", len);
		Sleep(1000);
	}
}