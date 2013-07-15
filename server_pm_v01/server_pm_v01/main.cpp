#include "Client.h"
#ifdef _WIN32
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

#define MY_PORT 666

#define PRINTNUSERS if (nclients) printf("%d users on-line\n", nclients); else puts("No user on-line...\n");

FUNC_THREAD PMWorking(void * client_socket);

int nclients = 0;
int cliID = 0;
CClient * cli = new CClient[256];
	
int main(int argc, char* argv[])
{
	char buff[1024];
	lfmp::CSocket mysocket(514, &buff[0], AF_INET, SOCK_STREAM, 0, MY_PORT, 0);

	puts("TCP SERVER RUNING\n");
	
	mysocket.socket_h();

	mysocket.bind_h();

	mysocket.listen_h(256);

	puts("Waiting for clients...\n");

	lfmp::CSocket client_socket;
	int client_addr_size = sizeof(sockaddr_in);

	lfmp::CThread cth;
	while((client_socket.accept_h(mysocket, &client_addr_size)))
	{
		nclients++;

		client_socket.gethostbyaddrh();

		cli[cliID++].giveCAbysock(client_socket);

		printf("+%s [%s] new connect!\n",
			client_socket.gethostname(),
			client_socket.gethostaddr());
		PRINTNUSERS;
		cth.crThread(&(client_socket), PMWorking);
	}
	return 0;
}

FUNC_THREAD PMWorking(void * client_socket)
{
	lfmp::CSocket my_sock(client_socket);

	char * buff = new char[1024];
	char * cbuff = new char[1024];
	int k;
	int j;
	int bytes_recv;

#define sHELLO "Wake up, Neo...\n"
	
	my_sock.send_h(sHELLO, sizeof(sHELLO), 0);

	while((bytes_recv = my_sock.recv_h(buff, strlen(buff), 0)) && bytes_recv != SOCKET_ERROR)
	{
		for(k = 0; cli[k].getSock() != my_sock.getSock(); k++);
		strcpy(cbuff, cli[k].getCAbysock());
		for(k = 0; cbuff[k] != 0; k++);
		for(j = 0; j < bytes_recv; j++)
		{
			cbuff[k++] = buff[j];
		}
		for(int i = 0; i < cliID; i++)
		{
			if(cli[i].getSock() != my_sock.getSock())
			{
				cli[i].sendMess(&cbuff[0], k - 1, 0);
			}
		}
	}
	
	nclients--;
	puts("-disconnect\n");
	PRINTNUSERS;

	delete[] buff;
	delete[] cbuff;

	my_sock.closesokh();
	return 0;
}
