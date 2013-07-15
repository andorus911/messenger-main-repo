#define LFMPDLL_API __declspec(dllexport) 

#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#ifdef _WIN32
#define SOCKH SOCKET
#else
#define SOCK int
#define DWORD unsigned long
#define WINAPI 
#define LPVOID void*
//typedef DWORD (WINAPI *LPTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);
#endif

namespace lfmp
{
	class CSocket
	{
		SOCKH sokh;
		sockaddr_in addrh;
		HOSTENT *hst;
		int af, str, pr, port, type;
	public:
		LFMPDLL_API CSocket();
		LFMPDLL_API CSocket(void * c_socket);
		LFMPDLL_API CSocket(int vers, char * buff, int af, int stream, int protocol, int port, int type);
		LFMPDLL_API SOCKET getSock(void);
		LFMPDLL_API void socket_h(void);
		LFMPDLL_API void bind_h(void);
		LFMPDLL_API void connect_h(void);
		LFMPDLL_API void listen_h(int backlog);
		LFMPDLL_API int accept_h(CSocket asock, int * size);
		LFMPDLL_API int send_h(char * buff, int len, int flags);
		LFMPDLL_API int recv_h(char * buff, int len, int flags);
		LFMPDLL_API int closesokh(void);
		LFMPDLL_API void gethostbyaddrh(void);
		LFMPDLL_API char * gethostname(void);
		LFMPDLL_API char * gethostaddr(void);
		LFMPDLL_API int dest_addr(char * server_addr);
		LFMPDLL_API int sizeofaddrh();
		LFMPDLL_API int printerr(char * message);
	};

	class CThread
	{
		DWORD ID;
	public:
		LFMPDLL_API DWORD WINAPI wrThread(LPVOID client_socket);
		LFMPDLL_API void crThread(void * sock, LPTHREAD_START_ROUTINE res);
	};
}