#include <WinSock2.h>

int main()
{
	WORD wVersionRequested = 0;
	LPWSADATA lpWSAData = 0;
	WSAStartup(wVersionRequested, lpWSAData); // ��� ������ � ����, ���� �� ����

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); //����� ��� �����, ���������, ������

	const struct sockaddr FAR* addr = 0;

	connect(sock, addr, 12); // ���������� � ������ ������
	//��� ������ ������ �� ����

	send(,,,MSG_PEEK); // �������� ������, ���� �������� ����� � ������ - ������ ���� �� ����

	recv(,,,MSG_PEEK); // ��������� ������, ����� �� �� ���� ���� ����� ������
	// ��� ������� ���������� �������� SOCKET_ERROR == -1 ��� ������
	
	shutdown(sock, SD_BOTH); // ������� ����� ������� �������� ����������
	
	closesocket(sock); // ��������� �����

	WSACleanup(); // ������� ������

	return 0;
}