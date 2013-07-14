#include "lib_for_my_proj.h"

namespace lfmp
{
	CSocket::CSocket()
	{
	}

	CSocket::CSocket(void * c_socket)
	{
		sokh = ((CSocket*)c_socket)->sokh;
		addrh = ((CSocket*)c_socket)->addrh;
		hst = ((CSocket*)c_socket)->hst;
		af = ((CSocket*)c_socket)->af;
		str = ((CSocket*)c_socket)->str;
		pr = ((CSocket*)c_socket)->pr;
		port = ((CSocket*)c_socket)->port;
		type = ((CSocket*)c_socket)->type;
	}

	CSocket::CSocket(int vers, char * buff, int a, int s, int p, int pt, int tp)
	{
		if(WSAStartup(vers, (WSADATA *) buff))
		{
			lfmp::CSocket::printerr("Error WSAStartup");
		}

		af = a;
		str = s;
		pr = p;
		port = pt;
		type = tp;
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

		addrh.sin_family = af;
		addrh.sin_port = htons(port);
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

	int CSocket::accept_h(CSocket asock, int * client_addr_size)
	{
		if(sokh = accept(asock.getSock(), (sockaddr *) &addrh, client_addr_size))
			return 1;
		else
			return 0;
	}

	int CSocket::send_h(char * buff, int len, int flags)
	{
		send(sokh, &buff[0], len, flags);
		return 0;
	}

	int CSocket::recv_h(char * buff, int len, int flags)
	{
		int i = recv(sokh, buff, len, flags);
		return i;
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