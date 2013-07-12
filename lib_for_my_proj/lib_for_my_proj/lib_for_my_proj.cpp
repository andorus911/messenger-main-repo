#include "lib_for_my_proj.h"

namespace lfmp
{
	CSocket::CSocket()
	{
	}

	CSocket::CSocket(int a, int s, int p, int pt, int tp)
	{

		af = a;
		str = s;
		pr = p;
		port = pt;
		type = tp;
	}

	void CSocket::start_h(int vers, char * buff)
	{
		if(WSAStartup(vers, (WSADATA *) buff))
		{
			lfmp::CSocket::printerr("Error WSAStartup");
		}
	}

	SOCKET CSocket::getSock(void)
	{
		return sokh;
	}

	void CSocket::socket_h(void)
	{
		if((sokh = socket(af, str, pr)) < 0)
		{
			printerr("Error socket");
		}

		addrh.sin_family = af; // for net
		addrh.sin_port = htons(port); // warning
		addrh.sin_addr.s_addr = 0;

	}

	void CSocket::bind_h(void)
	{
		if (bind(sokh, (sockaddr *) &addrh, sizeof(addrh)))
		{
			closesokh();
			printerr("Error bind");
		}
	}

	void CSocket::listen_h(int q)
	{
		if(listen(sokh, q))
		{
			closesokh();
			printerr("Error bind");
		}
	}

	int CSocket::accept_h(int * client_addr_size)
	{
		return (sokh = accept(sokh, (sockaddr *) &addrh, client_addr_size));
	}

	int CSocket::send_h(char * buff, int len, int flags)
	{
		send(sokh, &buff[0], len, flags);
		return 0;
	}

	int CSocket::recv_h(char * buff, int len, int flags)
	{
		return recv(sokh, &buff[0], len, flags);
	}

	int CSocket::closesokh(void)
	{
		#ifdef _WIN32
			return closesocket(sokh);
		#else
			return close(sokh);
		#endif
	}

	void CSocket::gethostbyaddrh(void)
	{
		hst = gethostbyaddr((char *) &addrh.sin_addr.s_addr, 4, AF_INET);
	}

	char * CSocket::gethostname(void)
	{
		return (hst) ? hst->h_name : "";
	}

	char * CSocket::gethostaddr(void)
	{
		return inet_ntoa(addrh.sin_addr);
	}

	int CSocket::sizeofaddrh()
	{
		return sizeof(addrh);
	}

	int CSocket::printerr(char * message)
	{
		printf("%s %d", message, WSAGetLastError());
		WSACleanup();
		return -1;
	}

	void CThread::crThread(void * sock, LPTHREAD_START_ROUTINE res)
	{
		#ifdef _WIN32
			CreateThread(NULL, NULL, res, sock, NULL, &ID);
		#else
			pthread_create(&ID, NULL, res, sock);
		#endif
	}

	DWORD WINAPI CThread::wrThread(LPVOID client_socket)
	{
		return 0;
	}
}