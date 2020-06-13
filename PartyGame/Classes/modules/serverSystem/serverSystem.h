#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#ifndef _SERVER_SYSTEM_
#define _SERVER_SYSTEM_

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")
#define PORT 10000

class serverSystem {
	public:
		static serverSystem* getInstance();
		serverSystem();
		~serverSystem();
		bool init();

		std::string getPublicAddress() { return publicAddress; }
		void startListenClients();
	private:
		std::string publicAddress = "";

		WSADATA wsaData;
		int iResult;

		SOCKET ListenSocket;
		SOCKET client_socket;  
		sockaddr_in client_addr; 

		int nclients = 0;

		void printUsers();
};

#endif