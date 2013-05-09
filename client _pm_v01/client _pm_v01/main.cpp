#pragma comment (lib, "ws2_32.lib")

#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>

#define PORT 666
#define SERVERADDR "127.0.0.1"

int main(int argc, char* argv[])
{
	char buff[1024]; // buffer
	puts("TCP CLIENT");

	if(WSAStartup(0x202, (WSADATA *) &buff[0]))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}

	SOCKET my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		return -1;
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
			printf("Invalid address %s\n", SERVERADDR);
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}
		//get addr serv - connect to serv
		if (connect(my_sock, (sockaddr *) &dest_addr, sizeof(dest_addr)))
		{
			printf("Connect error %d\n", WSAGetLastError());
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
				closesocket(my_sock);
				WSACleanup();
				return 0;
			}

			//send string to serv
			send(my_sock, &buff[0], nsize, 0);
		}

		printf("Recv error %d\n", WSAGetLastError());
		closesocket(my_sock);
		//shutdown(sock, SD_BOTH); // немного более сложное закрытие соединения
		WSACleanup();
		return -1;
}