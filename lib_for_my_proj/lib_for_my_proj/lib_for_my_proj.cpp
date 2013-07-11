#include "lib_for_my_proj.h"

namespace lfmp {
	int CSocket::close(void) {
		#ifdef _WIN32
			return closesocket(sokh);
		#else
			return close(sokh);
		#endif
	}

	int CSocket::printerr(char * message) {
		printf("%s %d", message, WSAGetLastError());
		WSACleanup();
		return -1;
	}

	int CSocket::start(int vers, char * buff) {
		if(WSAStartup(vers, (WSADATA *) buff)) {
			lfmp::CSocket::printerr("Error WSAStartup");
		}

		return 0;
	}

	void CThread::crThread(void * sock, LPTHREAD_START_ROUTINE res) {
		#ifdef _WIN32
			CreateThread(NULL, NULL, res, sock, NULL, &ID);
		#else
			pthread_create(&ID, NULL, res, sock);
		#endif
	}

	DWORD WINAPI CThread::wrThread(LPVOID client_socket) {
		return 0;
	}
}