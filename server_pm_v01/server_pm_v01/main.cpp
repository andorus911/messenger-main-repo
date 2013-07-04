#ifdef _WIN32
	#pragma comment (lib, "ws2_32.lib")
	#include <WinSock2.h>
	#include <Windows.h>
	#define START(x,y) if(WSAStartup(x, (WSADATA *) y)) { PERROR("Error WSAStartup %d\n"); }	//не принципиально
	//#define START if(WSAStartup(0x0202, (WSADATA *) &buff[0])) { PERROR("Error WSAStartup %d\n"); }
	//#define CLEAN WSACleanup()
	//#define LASTERR WSAGetLastError()
	#define PERROR(x) printf(x,WSAGetLastError()); WSACleanup(); return -1//тоже
	#define CLOSE(x) closesocket(x)//в отдельный класс засунуть
	#define THREADCREATE(x,y,z) CreateThread(NULL, NULL, x, y, NULL, z)
	
	typedef SOCKET CRSOCK; //тоже в отдельный класс
#else 
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define START(x,y)
	#define SOCKET_ERROR (-1)
	#define THREADCREATE(x,y,z) pthread_create(z, NULL, x, y)
	#define PERROR(x) printf(x); return -1
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

class CThread
{
public:
	DWORD ID;
	int * socket;

	void crThread(/*DWORD __stdcall **/LPTHREAD_START_ROUTINE res, CRSOCK * sock, DWORD * ID) {
		#ifdef _WIN32
			CreateThread(NULL, NULL, res, sock, NULL, ID);
		#else
			pthread_create(ID, NULL, res, sock);
		#endif
	}
};

class CSocket
{
public:
#ifdef _WIN32
	SOCKET sokh;
#else 
	int sokh;
#endif

};

DWORD __stdcall ProcessingClient(LPVOID client_socket);
//function for clients

int nclients = 0;

CSocket clients[0x100];
int clientInd = 0;

int main(int argc, char* argv[])
{
	char buff[1024]; //buffer
	CThread cth;
	CSocket mysocket;

	puts("TCP SERVER RUNING\n");
	
	START(0x0202, &buff[0]);

	

	//CRSOCK mysocket; //AF_INET - sock for net, SOCK_STREAM - stream, 0 - TCP(default)

	if ((mysocket.sokh = socket(AF_INET, SOCK_STREAM, 0)) < 0) // if inicializing
	{
		PERROR("Error socket %d\n");
	}

	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET; // for net
	local_addr.sin_port = htons(MY_PORT); // warning
	local_addr.sin_addr.s_addr = 0;
	// server take connections on all its ipaddrs

	// св€зка
	if (bind(mysocket.sokh, (sockaddr *) &local_addr, sizeof(local_addr)))
	{
		CLOSE(mysocket.sokh);
		PERROR("Error bind %d\n");
	}

	// size of queue - 0x100.. why?
	if(listen(mysocket.sokh, 0x100))
	{
		CLOSE(mysocket.sokh);
		PERROR("Error bind %d\n");
	}

	puts("Waiting for clients...\n");

	CSocket client_socket;
	sockaddr_in client_addr; // client's addr, syst will write in it
	int client_addr_size = sizeof(client_addr); // for accept(..)

	while((client_socket.sokh = accept(mysocket.sokh, (sockaddr *) &client_addr, &client_addr_size)))
	{
		nclients++; // +1 to clients

		//clients[clientInd++] = client_socket;

		HOSTENT *hst; // try take host name
		hst = gethostbyaddr((char *) &client_addr.sin_addr.s_addr, 4, AF_INET);

		printf("+%s [%s] new connect!\n",
			(hst) ? hst->h_name : "",
			inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS;

		DWORD thID;//впихнуть в класс
		cth.crThread(ProcessingClient, &client_socket.sokh, &thID);
	}
	//shutdown(sock, SD_BOTH); // немного более сложное закрытие соединени€
	return 0;
}

DWORD __stdcall ProcessingClient(LPVOID client_socket)//”«Ќј“№ что такое __stdcall
{
	CSocket my_sock;
	my_sock.sokh = ((SOCKET *) client_socket)[0];
	char buff[20 * 1024];
	int bytes_recv;

	clients[clientInd++] = my_sock;

#define sHELLO "Hello, sir!\n"

	// send hello
	send(my_sock.sokh, sHELLO, sizeof(sHELLO), 0);

	//echo circle

	while((bytes_recv = recv(my_sock.sokh, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR)
		//send(my_sock, &buff[0], bytes_recv, 0);
		for(int i = 0; i < clientInd; i++)
			send(clients[i].sokh, &buff[0], bytes_recv, 0);

	nclients--;
	puts("-disconnect\n");
	PRINTNUSERS;

	CLOSE(my_sock.sokh);
	return 0;
}