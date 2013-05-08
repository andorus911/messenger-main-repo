#include <WinSock2.h>

int main()
{
	WORD wVersionRequested = 0;
	LPWSADATA lpWSAData = 0;
	WSAStartup(wVersionRequested, lpWSAData); // что делать с этим, пока не знаю

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); //сокет для инета, потоковый, тисипи

	const struct sockaddr FAR* addr = 0;

	connect(sock, addr, 12); // поработать с другой главой
	//при ошибке вернет не ноль

	send(,,,MSG_PEEK); // передает пакеты, надо поставть флаги и сокеты - каакие пока не знаю

	recv(,,,MSG_PEEK); // принимает пакеты, опять же не знаю пока какие сокеты
	// обе функции возвращают значение SOCKET_ERROR == -1 при ошибке
	
	shutdown(sock, SD_BOTH); // немного более сложное закрытие соединения
	
	closesocket(sock); // закрываем сокет

	WSACleanup(); // очищаем память

	return 0;
}