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
//#include <typeinfo.h>

#define MY_PORT 666 // port listen server

#define PRINTNUSERS if (nclients) printf("%d users on-line\n", nclients); else puts("No user on-line...\n");

unsigned long __stdcall PMWorking(void * client_socket);
//function for clients

class CClient {
public:
	int sock;
	char client[35];
	//char addr[15]; //сделать конструктор дл€ выделени€ пам€ти
	//сдделать метод гет дл€ доступа к сокету
	void giveCAbysock(int s, char c[], char a[]) {
		sock = s;
		int j = 0;
		for(int i = 0; c[i] != 0 && i < 10; i++) {
			client[j++] = c[i];
		}
		client[j++] = ' ';
		client[j++] = '[';
		for(int i = 0; a[i] != 0; i++) {
			client[j++] = a[i];
		}
		client[j++] = ']';
		client[j++] = ':';
		client[j++] = ' ';
		client[j] = 0;
	}
	char * getCAbysock()
	{
		return client;
	}
	int getSock()
	{
		return sock;
	}
};

int nclients = 0;
lfmp::CSocket clients[0x100]; //отправить в качестве пол€ в CClient
int clientInd = 0;

	CClient cli[0x100];//тоже динамика не помешает
	int ind = 0;

	
int main(int argc, char* argv[])
{
	char buff[1024]; //buffer
	lfmp::CSocket mysocket;

	puts("TCP SERVER RUNING\n");
	
	mysocket.start(0x0202, &buff[0]);

	//AF_INET - sock for net, SOCK_STREAM - stream, 0 - TCP(default)
	if ((mysocket.sokh = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		mysocket.printerr("Error socket");
	}

	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET; // for net
	local_addr.sin_port = htons(MY_PORT); // warning
	local_addr.sin_addr.s_addr = 0;
	// server take connections on all its ipaddrs

	// св€зка
	if (bind(mysocket.sokh, (sockaddr *) &local_addr, sizeof(local_addr)))
	{
		mysocket.close();
		mysocket.printerr("Error bind");
	}

	// size of queue - 0x100.. why?
	if(listen(mysocket.sokh, 0x100))
	{
		mysocket.close();
		mysocket.printerr("Error bind");
	}

	puts("Waiting for clients...\n");

	lfmp::CSocket client_socket;
	sockaddr_in client_addr; // client's addr, syst will write in it
	int client_addr_size = sizeof(client_addr); // for accept(..)


	lfmp::CThread cth;
	while((client_socket.sokh = accept(mysocket.sokh, (sockaddr *) &client_addr, &client_addr_size)))
	{
		nclients++; // +1 to clients

		//clients[clientInd++] = client_socket;

		HOSTENT *hst; // try take host name
		hst = gethostbyaddr((char *) &client_addr.sin_addr.s_addr, 4, AF_INET);

		cli[ind++].giveCAbysock(client_socket.sokh, (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));

		printf("+%s [%s] new connect!\n",
			(hst) ? hst->h_name : "",
			inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS;
		cth.crThread(&client_socket.sokh, PMWorking);//можно создать структуру содержащую 
	}
	return 0;
}

unsigned long __stdcall PMWorking(void * client_socket)
{
	//засунуть в класс, можно наследовать клиентский и серверный тред
	//создать длл из которой мы будем наслеовать класс работы с тредом и 
	//переопредел€ть метод “редћетод дл€ работы с сообщени€ми - отправка или распечатка
	lfmp::CSocket my_sock;
	my_sock.sokh = ((int *) client_socket)[0];
	char buff[20 * 1024];
	char * cbuff = new char[1024];//[20 * 1024];ввезде ввести динаммическую пам€ть
	int k, j;
	int bytes_recv;

	clients[clientInd++] = my_sock;

#define sHELLO "Hello, dude!\n"

	send(my_sock.sokh, sHELLO, sizeof(sHELLO), 0);

	while((bytes_recv = recv(my_sock.sokh, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR)
	{
		//sockaddr_in client_addr; // client's addr, syst will write in i
		//HOSTENT *hst; // try take host name
		//hst = gethostbyaddr((char *) &client_addr.sin_addr.s_addr, 4, AF_INET);
		//cbuff = (hst) ? hst->h_name : "";
		////cbuff = strcat(cbuff, inet_ntoa(client_addr.sin_addr));
		//cbuff = strcat(cbuff, buff);
		for(k = 0; cli[k].sock != my_sock.sokh; k++);
		strcpy(cbuff, cli[k].getCAbysock());
		for(k = 0; cbuff[k] != 0; k++);
		for(j = 0; j < bytes_recv; j++)
			cbuff[k++] = buff[j];
		for(int i = 0; i < clientInd; i++)
			if(clients[i].sokh != my_sock.sokh)
				send(clients[i].sokh, /*&buff[0]*/&cbuff[0], /*bytes_recv*/k-1, 0);//некоторым отсылаютс€ дублированные сообщени€
	}
	
	nclients--;
	puts("-disconnect\n");
	PRINTNUSERS;

	my_sock.close();
	return 0;
}
