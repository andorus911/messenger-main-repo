#ifdef _WIN32
	#pragma comment (lib, "ws2_32.lib")
	#include <WinSock2.h>
	#include <Windows.h>
	#define START(x,y) if(WSAStartup(x, (WSADATA *) y)) { PERROR("Error WSAStartup %d\n"); }	
	//#define START if(WSAStartup(0x0202, (WSADATA *) &buff[0])) { PERROR("Error WSAStartup %d\n"); }
	//#define CLEAN WSACleanup()
	//#define LASTERR WSAGetLastError()
	#define PERROR(x) printf(x,WSAGetLastError()); WSACleanup(); return -1
	#define CLOSE(x) closesocket(x)
	#define THREADCREATE(x,y,z) CreateThread(NULL, NULL, x, y, NULL, z)
	typedef SOCKET CRSOCK; 
#else 
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define START
	#define SOCKET_ERROR (-1)
	#define THREADCREATE(x,y,z) pthread_create(z, NULL, x, y)
	#define PERROR(x) printf(x); CLEAN; return -1
	#define CLOSE(x) close(x)
	typedef int CRSOCK;
	typedef unsigned long DWORD WINAPI;
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MY_PORT 666 // port listen server

#define PRINTNUSERS if (nclients) printf("%d users on-line\n", nclients); else puts("No user on-line...\n");
//mb puts(..) not best func for it


DWORD __stdcall ProcessingClient(LPVOID client_socket);
//function for clients

int nclients = 0;

int main(int argc, char* argv[])
{
	char buff[1024]; //buffer
	//i use it as * on WSADATA too, couse this char big

	puts("TCP SERVER RUNING\n");
	
	START(0x0202, &buff[0]);

	CRSOCK mysocket; //AF_INET - sock for net, SOCK_STREAM - stream, 0 - TCP(default)

	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) // if inicializing
	{
		PERROR("Error socket %d\n");
	}

	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET; // for net
	local_addr.sin_port = htons(MY_PORT); // warning
	local_addr.sin_addr.s_addr = 0;
	// server take connections on all its ipaddrs

	// связка
	if (bind(mysocket, (sockaddr *) &local_addr, sizeof(local_addr)))
	{
		CLOSE(mysocket);
		PERROR("Error bind %d\n");
	}

	// size of queue - 0x100.. why?
	if(listen(mysocket, 0x100))
	{
		CLOSE(mysocket);
		PERROR("Error bind %d\n");
	}

	puts("Waiting for clients...\n");

	CRSOCK client_socket;
	sockaddr_in client_addr; // client's addr, syst will write in it
	int client_addr_size = sizeof(client_addr); // for accept(..)

	while((client_socket = accept(mysocket, (sockaddr *) &client_addr, &client_addr_size)))
	{
		nclients++; // +1 to clients

		HOSTENT *hst; // try take host name
		hst = gethostbyaddr((char *) &client_addr.sin_addr.s_addr, 4, AF_INET);

		printf("+%s [%s] new connect!\n",
			(hst) ? hst->h_name : "",
			inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS;

		DWORD thID;
		THREADCREATE(ProcessingClient, &client_socket, &thID);
		//CreateThread(NULL, NULL, ProcessingClient, &client_socket, NULL, &thID);
		//pthread_create(&thID, NULL, ProcessingClient, &client_socket);
	}
	//shutdown(sock, SD_BOTH); // немного более сложное закрытие соединения
	return 0;
}

DWORD __stdcall ProcessingClient(LPVOID client_socket)
{
	CRSOCK my_sock;
	my_sock = ((SOCKET *) client_socket)[0];
	char buff[20 * 1024];
	int bytes_recv;

#define sHELLO "Hello, sir!\n"

	send(my_sock, sHELLO, sizeof(sHELLO), 0); // send hello

	//echo circle
	while((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR)
		//SOCKET_ERROR = -1
		send(my_sock, &buff[0], bytes_recv, 0);

	nclients--;
	puts("-disconnect\n");
	PRINTNUSERS;

	CLOSE(my_sock);
	return 0;
}