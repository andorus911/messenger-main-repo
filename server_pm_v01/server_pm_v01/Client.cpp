#include "Client.h"

CClient::CClient()
{
	client = new char[35];
}
CClient::~CClient()
{
	delete[] client;
	delete client;
}
//transmit a socket's copy into class object
void CClient::giveCAbysock(lfmp::CSocket given_sock) {
	sock = given_sock;
	int j = 0;
	char * c = given_sock.gethostname();
	char * a = given_sock.gethostaddr();

	for(int i = 0; c[i] != 0 && i < 10; i++) {
		client[j++] = c[i];
	}
	client[j++] = ' ';
	client[j++] = '[';
	for(int i = 0; a[i] != 0; i++) {
		client[j++] = a[i];
	}
	client[j++] = ']';
	client[j++] = ':';
	client[j++] = ' ';
	client[j] = 0;
}
//get client's name
char * CClient::getCAbysock()
{
	return client;
}
//get client's socket
int CClient::getSock()
{
	return sock.getSock();
}
//send message to client
int CClient::sendMess(char * buff, int len, int flags)
{
	sock.send_h(buff, len, flags);
	return 0;
}