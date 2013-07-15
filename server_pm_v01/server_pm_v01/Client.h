#ifdef _WIN32
#pragma comment (lib, "lib_for_my_proj.lib")
#include "lib_for_my_proj.h"
#endif

class CClient {
	lfmp::CSocket sock;
	
	//client's name
	char * client;
public:
	CClient();
	~CClient();
	
	//transmit a socket's copy into class object
	void giveCAbysock(lfmp::CSocket given_sock);

	//get client's name
	char * getCAbysock();
	
	//get client's socket
	int getSock();
	
	//send message to client
	int sendMess(char * buff, int len, int flags);
};
