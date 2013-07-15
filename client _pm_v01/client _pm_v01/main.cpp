#ifdef _WIN32
	#pragma comment (lib, "lib_for_my_proj.lib")
	#include "lib_for_my_proj.h"
	#include <Windows.h>
	#define FUNC_THREAD unsigned long __stdcall
#else 
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define FUNC_THREAD unsigned long
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORT 666
#define SERVERADDR "127.0.0.1"

unsigned long __stdcall PMWorking(void * server_socket);

int main(int argc, char* argv[])
{
	char * buff = new char[1024];
	puts("TCP CLIENT");
	lfmp::CSocket my_sock(0x202, &buff[0], AF_INET, SOCK_STREAM, 0, PORT, 0);
	
	my_sock.socket_h();

	my_sock.dest_addr(SERVERADDR);

	my_sock.connect_h();

	printf("Connection to %s: succesfull\nType \"quit\" for quit\n\n", SERVERADDR);

	lfmp::CThread thr;
	thr.crThread(&my_sock, PMWorking);

	while(true)
	{
		fgets(&buff[0], 1024, stdin);

		if (!strcmp(&buff[0], "quit\n"))
		{
			//exit
			puts("Exit...");
			my_sock.closesokh();
			my_sock.printerr("");
			return 0;
		}

		my_sock.send_h(&buff[0], strlen(buff), 0);
	}

	my_sock.closesokh();
	my_sock.printerr("Recv error");
	delete[] buff;
	return -1;
}


unsigned long __stdcall PMWorking(void * server_socket) {
	lfmp::CSocket my_sock(server_socket);
	char * buff = new char[1024];
	int bytes_recv = strlen(buff);

	while((bytes_recv = my_sock.recv_h(&buff[0], 1024, 0)) && bytes_recv != SOCKET_ERROR)
	{
		buff[bytes_recv] = 0;
		puts(buff);
	}

	delete[] buff;
	return 0;
}