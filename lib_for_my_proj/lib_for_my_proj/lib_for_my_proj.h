#ifdef LFMPSDLL_EXPORTS
#define LFMPDLL_API __declspec(dllexport) 
#else
#define LFMPDLL_API __declspec(dllimport) 
#endif

#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

namespace lfmp {
	class CSocket {
	public:
		SOCKET sokh;

		LFMPDLL_API int close(void);
		LFMPDLL_API int printerr(char * message);
		LFMPDLL_API int start(int vers, char * buff);
		//остальные функции идентичны обеим платформам
	};

	class CThread {
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