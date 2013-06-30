#ifdef _WIN32
	#pragma comment (lib, "ws2_32.lib")
	#include <WinSock2.h>
	#include <Windows.h>
	#define START(x,y) if(WSAStartup(x, (WSADATA *) y)) { PERROR("Error WSAStartup %d\n"); }
	#define CLEAN WSACleanup()
	#define LERROR WSAGetLastError()
	#define PERROR(x) printf(x,WSAGetLastError()); WSACleanup(); return -1
	#define CLOSE(x) closesocket(x)
	typedef SOCKET CRSOCK; //переопределение для этого типа не работает
#else 
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define START
	#define CLEAN
	#define SOCKET_ERROR (-1)
	#define PERROR(x) printf(x); CLEAN; return -1
	#define CLOSE(x) close(x)
	typedef int CRSOCK;
	typedef unsigned long DWORD WINAPI;
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define PORT 666
#define SERVERADDR "127.0.0.1"

int main(int argc, char* argv[])
{
	char buff[1024]; // buffer
	puts("TCP CLIENT");

	START(0x202, &buff[0]);

	CRSOCK my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(my_sock < 0)
	{
		PERROR("Socket() error %d\n");
	}

	// addr & port in sockaddr_in
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	HOSTENT *hst;

	// symb to network form
	if (inet_addr(SERVERADDR) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else
		// trying get ip addr (wiz domen)
		if (hst = gethostbyname(SERVERADDR))
			// hst->h_addr_list - arr * on addr
			((unsigned long *) &dest_addr.sin_addr)[0] = ((unsigned long **) hst->h_addr_list)[0][0];
		else
		{
			CLOSE(my_sock);
			PERROR("Invalid address %s\n", SERVERADDR);
			CLEAN;
			return -1;
		}
		//get addr serv - connect to serv
		if (connect(my_sock, (sockaddr *) &dest_addr, sizeof(dest_addr)))
		{
			printf("Connect error %d\n", LERROR);
			return -1;
		}

		printf("Connection to %s: succesfull\nType \"quit\" for quit\n\n", SERVERADDR);

		int nsize;
		while((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
		{
			// null in end of string
			buff[nsize] = 0;

			printf("S=>C:%s", buff);

			printf("S<=C:"); fgets(&buff[0], sizeof(buff)-1, stdin);//read string

			if (!strcmp(&buff[0], "quit\n"))
			{
				//exit
				puts("Exit...");
				CLOSE(my_sock);
				CLEAN;
				return 0;
			}

			//send string to serv
			send(my_sock, &buff[0], nsize, 0);
		}

		printf("Recv error %d\n", WSAGetLastError());
		CLOSE(my_sock);
		//shutdown(sock, SD_BOTH); // немного более сложное закрытие соединения
		CLEAN;
		return -1;
}