#include <WinSock2.h>

int main()
{
	WORD wVersionRequested = 0;
	LPWSADATA lpWSAData = 0;
	WSAStartup(wVersionRequested, lpWSAData); // что делать с этим, пока не знаю

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); //сокет для инета, потоковый, тисипи

	const struct sockaddr FAR* name = 0;

	bind(sock, name, 12); // связывание сокета с адресом, на который будет кидаться клиент
	// вернет ноль при успехе
	
	listen(sock, 50); // сколько подключений можем обрабатывать

	struct sockaddr FAR* addr = 0;
	int FAR* addrlen = 0;
	SOCKET sock2 = sock;
	do{
		sock2 = accept(sock2, addr, addrlen); // соединение,  надо породить множественное соединение
	}while(); // надо подумать

		send(,,,MSG_PEEK); // передает пакеты, надо поставть флаги и сокеты - каакие пока не знаю

	recv(,,,MSG_PEEK); // принимает пакеты, опять же не знаю пока какие сокеты
	// обе функции возвращают значение SOCKET_ERROR == -1 при ошибке
	
	shutdown(sock, SD_BOTH); // немного более сложное закрытие соединения
	
	closesocket(sock); // закрываем сокет

	WSACleanup(); // очищаем память

	return 0;
}	