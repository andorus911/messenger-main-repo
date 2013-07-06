#ifdef _WIN32
	#pragma comment (lib, "ws2_32.lib")
	
	#include <WinSock2.h>
	#include <Windows.h>
	
	#define START(x,y) if(WSAStartup(x, (WSADATA *) y)) { PERROR("Error WSAStartup %d\n", LERROR); }
	#define CLEAN WSACleanup()
	#define LERROR WSAGetLastError()
	#define PERROR(x,y) printf(x,y); WSACleanup(); return -1
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


class CSocket
{
public:
#ifdef _WIN32
	SOCKET sokh;
#else 
	int sokh;
#endif

};

class CThread
{
public:
	DWORD ID;
	CSocket socket;

	void crThread(/*DWORD __stdcall **/LPTHREAD_START_ROUTINE res, void * sock, DWORD * ID) {
		#ifdef _WIN32
			CreateThread(NULL, NULL, res, sock, NULL, ID);
		#else
			pthread_create(ID, NULL, res, sock);
		#endif
	}
};

DWORD __stdcall PrintMessages(void * server_socket);

int main(int argc, char* argv[])
{
	char buff[1024]; // buffer
	puts("TCP CLIENT");

	START(0x202, &buff[0]);

	CSocket my_sock;
	my_sock.sokh = socket(AF_INET, SOCK_STREAM, 0);
	if(my_sock.sokh < 0)
	{
		PERROR("Socket() error %d\n", LERROR);
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
			CLOSE(my_sock.sokh);
			PERROR("Invalid address %s\n", SERVERADDR);
			CLEAN;
			return -1;
		}
		//get addr serv - connect to serv
		if (connect(my_sock.sokh, (sockaddr *) &dest_addr, sizeof(dest_addr)))
		{
			printf("Connect error %d\n", LERROR);
			return -1;
		}

		printf("Connection to %s: succesfull\nType \"quit\" for quit\n\n", SERVERADDR);


		//СОЗДАТЬ ТУТ ПОТОК
		CThread thr;
		thr.socket.sokh = my_sock.sokh;
		DWORD thID;
		thr.crThread(PrintMessages, /*&my_sock.sokh*/&thr.socket.sokh, &thID);
		int nsize;
		while(/*(nsize = recv(my_sock.sokh, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR*/true)
		{
			// null in end of string
			//buff[nsize] = 0;

			//printf("S=>C:%s", buff);

			printf("I say: "/*, (hst) ? hst->h_name : inet_ntoa(dest_addr.sin_addr)*/);
			fgets(&buff[0], sizeof(buff)-1, stdin);//read string
			//НАДО ПРОСТО ДОБАВИТЬ СЮДА ПОЛУЧЕНИЕ ПАКЕТА И ВСЕ НО КАК ЭТО СДЕЛАТЬ, или нет
			if (!strcmp(&buff[0], "quit\n"))
			{
				//exit
				puts("Exit...");
				CLOSE(my_sock.sokh);
				CLEAN;
				return 0;
			}

			//send string to serv
			send(my_sock.sokh, &buff[0], strlen(buff)/*nsize*/, 0);
		}

		CLOSE(my_sock.sokh);
		PERROR("Recv error %d\n", LERROR);
		//shutdown(sock, SD_BOTH); // немного более сложное закрытие соединения
		//CLEAN;
		return -1;
}


DWORD __stdcall PrintMessages(void * server_socket) {
	CSocket my_sock;
	my_sock.sokh = ((int *) server_socket)[0];
	char buff[20 * 1024];
	int bytes_recv;

	//sockaddr_in dest_addr;
	//dest_addr.sin_family = AF_INET;
	//dest_addr.sin_port = htons(PORT);
	//HOSTENT *hst; // try take host name
	//hst = gethostbyaddr((char *) &dest_addr.sin_addr.s_addr, 4, AF_INET);

	while((bytes_recv = recv(my_sock.sokh, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) {
		buff[bytes_recv] = 0;
		printf("%s", /*(hst) ? hst->h_name : inet_ntoa(dest_addr.sin_addr),*/ buff);
	}

	//CLOSE(my_sock.sokh);
	return 0;
}