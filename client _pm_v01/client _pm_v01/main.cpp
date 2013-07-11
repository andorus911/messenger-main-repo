#ifdef _WIN32
//	#pragma comment (lib, "ws2_32.lib")
	#pragma comment (lib, "lib_for_my_proj.lib")
//	#include <WinSock2.h>
	#include "lib_for_my_proj.h"
	#include <Windows.h>
#else 
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORT 666
#define SERVERADDR "127.0.0.1"

unsigned long __stdcall PrintMessages(void * server_socket);

int main(int argc, char* argv[])
{
	char buff[1024];
	puts("TCP CLIENT");
	lfmp::CSocket my_sock;

	my_sock.start(0x202, &buff[0]);//конструктор должен делать эту работу, прин€ть на себу эту фигню
	
	my_sock.sokh = socket(AF_INET, SOCK_STREAM, 0);//все функции по работе с сокетом запихать в длл ибо пол€ метода надо — –џ“№
	if(my_sock.sokh < 0)//как и тут и далее заприватить сокх
	{
		my_sock.printerr("Socket() error");
	}

	sockaddr_in dest_addr;//отправить в длл как и функции
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	HOSTENT *hst;

	if (inet_addr(SERVERADDR) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else
		// trying get ip addr (wiz domen)
		if (hst = gethostbyname(SERVERADDR))
			// hst->h_addr_list - arr * on addr
			((unsigned long *) &dest_addr.sin_addr)[0] = ((unsigned long **) hst->h_addr_list)[0][0];
		else
		{
			my_sock.close();
			my_sock.printerr(strcat("Invalid address ", SERVERADDR));
			return -1;
		}

		//get addr serv - connect to serv
		if (connect(my_sock.sokh, (sockaddr *) &dest_addr, sizeof(dest_addr)))//тоже в длл
		{
			printf("Connect error\n");
			return -1;
		}

		printf("Connection to %s: succesfull\nType \"quit\" for quit\n\n", SERVERADDR);


		lfmp::CThread thr;
		thr.crThread(&my_sock.sokh, PrintMessages);
		int nsize;
		while(/*(nsize = recv(my_sock.sokh, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR*/true) {
			// null in end of string
			//buff[nsize] = 0;

			//printf("S=>C:%s", buff);

			//printf("I say: ", (hst) ? hst->h_name : inet_ntoa(dest_addr.sin_addr));
			fgets(&buff[0], sizeof(buff)-1, stdin);//read string

			if (!strcmp(&buff[0], "quit\n")) {
				//exit
				puts("Exit...");
				my_sock.close();
				//WSACleanup();
				my_sock.printerr("");
				return 0;
			}

			//send string to serv
			send(my_sock.sokh, &buff[0], strlen(buff)/*nsize*/, 0);//в длл
		}

		my_sock.close();
		my_sock.printerr("Recv error");
		return -1;
}


unsigned long __stdcall PrintMessages(void * server_socket) {
	lfmp::CSocket my_sock;
	my_sock.sokh = ((int *) server_socket)[0];
	char buff[20 * 1024];//динамика
	int bytes_recv;

	//sockaddr_in dest_addr;
	//dest_addr.sin_family = AF_INET;
	//dest_addr.sin_port = htons(PORT);
	//HOSTENT *hst; // try take host name
	//hst = gethostbyaddr((char *) &dest_addr.sin_addr.s_addr, 4, AF_INET);

	while((bytes_recv = recv(my_sock.sokh, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR)//ресив в длл
	{
		buff[bytes_recv] = 0;
		//printf("%s", /*(hst) ? hst->h_name : inet_ntoa(dest_addr.sin_addr),*/ buff);
		puts(buff);
	}


	//прочитать про синхронизацию потока!!!!!!!!!!!!!!!!!!!!111111111111111111


	//CLOSE(my_sock.sokh);
	return 0;
}