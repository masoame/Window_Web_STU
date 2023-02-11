#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>

extern char buffer_send[1024];
extern char buffer_recv[1024];
extern int islink[2];
extern char buffer_file[1024*8];


struct Client {
	SOCKET sock;

	sockaddr_in sock_addr;

	int sockaddr_len;
};

namespace TCP {
	extern int TCP_Client();

	extern bool Send_Message(void* server);

	extern bool RecvMessage(void* server);
}

namespace UDP {
	extern int UDP_Client();

	extern int thread_to_send(LPVOID argv);

	extern int thread_to_recv(LPVOID argv);

	extern void HeartBeat(LPVOID argv);
}

#define Check_Function(function) \
if(function == -1){ \
	printf("%s\n[line:%d] error_code=%d\n",__FILE__,__LINE__,GetLastError()); \
	return -1; \
}

#define Signed_show(function)\
printf("%d\n",function);