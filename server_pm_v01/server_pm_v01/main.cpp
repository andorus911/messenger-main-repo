#include <WinSock2.h>

int main()
{
	WORD wVersionRequested = 0;
	LPWSADATA lpWSAData = 0;
	WSAStartup(wVersionRequested, lpWSAData); // ��� ������ � ����, ���� �� ����

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); //����� ��� �����, ���������, ������

	const struct sockaddr FAR* name = 0;

	bind(sock, name, 12); // ���������� ������ � �������, �� ������� ����� �������� ������
	// ������ ���� ��� ������
	
	listen(sock, 50); // ������� ����������� ����� ������������

	struct sockaddr FAR* addr = 0;
	int FAR* addrlen = 0;
	SOCKET sock2 = sock;
	do{
		sock2 = accept(sock2, addr, addrlen); // ����������,  ���� �������� ������������� ����������
	}while(); // ���� ��������

		send(,,,MSG_PEEK); // �������� ������, ���� �������� ����� � ������ - ������ ���� �� ����

	recv(,,,MSG_PEEK); // ��������� ������, ����� �� �� ���� ���� ����� ������
	// ��� ������� ���������� �������� SOCKET_ERROR == -1 ��� ������
	
	shutdown(sock, SD_BOTH); // ������� ����� ������� �������� ����������
	
	closesocket(sock); // ��������� �����

	WSACleanup(); // ������� ������

	return 0;
}	