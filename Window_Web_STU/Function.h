#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
extern sockaddr_in server_addr;
extern SOCKET sock;

extern char buffer_send[1024];
extern char buffer_recv[1024];
extern char buffer_file[1024 * 8];

extern int islink;
extern HANDLE h_recvfile;
extern HANDLE h_sendfile;



struct Client {
	SOCKET sock;

	sockaddr_in sock_addr;

	int sockaddr_len;
};



namespace UDP {

	struct head {
		short message;						//0x01=FIN 0x02=SEQ 0x04=ACK 0x08=connect2client
		int ACK, SEQ, Temp;

	};



	extern int Connect_p2p(LPVOID argv);

	extern int UDP_Client();

	extern int thread_to_send(LPVOID argv);

	extern int thread_to_recv(LPVOID argv);

	extern void HeartBeat(LPVOID argv);

	extern int thread_to_sendfile(LPVOID argv);

	extern int thread_to_recvfile(LPVOID argv);
}

#define Check_Function(function) \
if(function == -1){ \
	printf("%s\n[line:%d] error_code=%d\n",__FILE__,__LINE__,GetLastError()); \
	return -1; \
}

#define Signed_show(function)\
printf("%d\n",function);