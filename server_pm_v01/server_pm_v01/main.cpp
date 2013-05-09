#pragma comment (lib, "ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h> // pered windows.h, recomended
#include <Windows.h>

#define MY_PORT 666 // port listen server

#define PRINTNUSERS if (nclients) printf("%d users on-linr\n", nclients); else puts("No user on-line...\n"); // my first macros in my life =)
//mb puts(..) not best func for it

DWORD WINAPI ProcessingClient(LPVOID client_socket);
//function for clients

int nclients = 0;

int main(int argc, char* argv[])
{
	char buff[1024]; //buffer
	//i use it as * on WSADATA too, couse this char big

	puts("TCP SERVER RUNING\n");
	
	if (WSAStartup(0x0202, (WSADATA *) &buff[0]))
	{
		printf("Error WSAStartup %d\n", WSAGetLastError());
		WSACleanup(); // close winsock
		return -1;
	}

	SOCKET mysocket; //AF_INET - sock for net, SOCK_STREAM - stream, 0 - TCP(default)
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) // if inicializing
	{
		printf("Error socket %d\n", WSAGetLastError());
		WSACleanup(); // close winsock
		return -1;
	}

	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET; // for net
	local_addr.sin_port = htons(MY_PORT); // warning
	local_addr.sin_addr.s_addr = 0;
	// server take connections on all its ipaddrs

	// связка
	if (bind(mysocket, (sockaddr *) &local_addr, sizeof(local_addr)))
	{
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket); // !!!
		WSACleanup();
		return -1;
	}

	// size of queue - 0x100.. why?
	if(listen(mysocket, 0x100))
	{
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	puts("Waiting for clients...\n");

	SOCKET client_socket;
	sockaddr_in client_addr; // client's addr, syst will write in it
	int client_addr_size = sizeof(client_addr); // for accept(..)

	while((client_socket = accept(mysocket, (sockaddr *) &client_addr, &client_addr_size)))
	{
		nclients++; // +1 to clients

		HOSTENT *hst; // try take host name
		(hst) ? hst -> h_name : "", inet_ntoa(client_addr.sin_addr);
		PRINTNUSERS;

		DWORD thID;
		CreateThread(NULL, NULL, ProcessingClient, &client_socket, NULL, &thID);
	}
	//shutdown(sock, SD_BOTH); // немного более сложное закрытие соединения
	return 0;
}

DWORD WINAPI ProcessingClient(LPVOID client_socket)
{
	SOCKET my_sock;
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

	closesocket(my_sock);
	return 0;
}