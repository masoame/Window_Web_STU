#include "Function.h"

char buffer_send[1024];
char buffer_recv[1024];
char buffer_file[1024 * 8];

int islink;
HANDLE h_recvfile;
HANDLE h_sendfile;