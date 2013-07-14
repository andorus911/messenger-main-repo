//#ifdef LFMPSDLL_EXPORTS
#define LFMPDLL_API __declspec(dllexport) 
//#else
//#define LFMPDLL_API __declspec(dllimport) 
//#endif

#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

namespace lfmp
{
	class CSocket
	{
		SOCKET sokh;
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
		LFMPDLL_API void listen_h(int backlog);
		LFMPDLL_API int accept_h(CSocket asock, int * size);
		LFMPDLL_API int send_h(char * buff, int len, int flags);
		LFMPDLL_API int recv_h(char * buff, int len, int flags);
		LFMPDLL_API int closesokh(void);
		LFMPDLL_API void gethostbyaddrh(void);
		LFMPDLL_API char * gethostname(void);
		LFMPDLL_API char * gethostaddr(void);
		LFMPDLL_API int sizeofaddrh();
		LFMPDLL_API int printerr(char * message);
	};

	class CThread
	{
	#ifdef _WIN32
		DWORD ID;
	#else
		unsigned long ID;
	#endif
	public:
		LFMPDLL_API DWORD WINAPI wrThread(LPVOID client_socket);
		LFMPDLL_API void crThread(void * sock, LPTHREAD_START_ROUTINE res);
	};
}