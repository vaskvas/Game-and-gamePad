#include "serverSystem.h"
#include "modules/eventListSystem/eventListSystem.h"
#include "base/ccMacros.h"
#include <cassert> 
#include <thread>

DWORD WINAPI LinkToClient(LPVOID client_socket);
static serverSystem* server;

serverSystem* serverSystem::getInstance() {
	if (!server) {
		server = new serverSystem();
		assert(server);
		if (!server->init()) {
			delete server;
			return nullptr;
		}
	}
	return server;
}
serverSystem::serverSystem() {
	ListenSocket = INVALID_SOCKET;
}
serverSystem::~serverSystem() {
	closesocket(ListenSocket);
	WSACleanup();
}
bool serverSystem::init() {
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		CCLOG("WSAStartup failed with error: %d\n", iResult);
		system("pause");
		return false;
	}
	char ac[80];
	if (gethostname(ac, sizeof(ac)) != SOCKET_ERROR){
		struct hostent* phe = gethostbyname(ac);
		if (phe == 0) {
			CCLOG("Yow! Bad host lookup.");
			return false;
		}
		for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
			struct in_addr addr;
			memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
			CCLOG("Address %d: %s", i, inet_ntoa(addr));
			publicAddress = inet_ntoa(addr);
		}
	}
	struct sockaddr_in addrinfo;
	addrinfo.sin_family = AF_INET;
	addrinfo.sin_port = htons(PORT);
	InetPton(AF_INET, L"0.0.0.0", &addrinfo.sin_addr.s_addr);
	if ((ListenSocket = socket(addrinfo.sin_family, SOCK_STREAM, 0)) < 0) {
		CCLOG("socket failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}
	iResult = bind(ListenSocket, (struct sockaddr*) & addrinfo, sizeof(addrinfo));
	if (iResult == SOCKET_ERROR) {
		CCLOG("bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		CCLOG("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}
	CCLOG("Waiting for connection request...\n");
	return true;
}
void serverSystem::startListenClients() {
	auto clientConnect = [this]() {
		int client_addr_size = sizeof(client_addr);

		while ((client_socket = accept(ListenSocket, (sockaddr*)
			&client_addr, &client_addr_size)))
		{
			nclients++;
			HOSTENT* hst;
			hst = gethostbyaddr((char*)
				&client_addr.sin_addr.s_addr, 4, AF_INET);

			CCLOG("+%s [%s] new connect!\n",
				(hst) ? hst->h_name : "",
				inet_ntoa(client_addr.sin_addr));
			printUsers();

			DWORD thID;
			CreateThread(NULL, NULL, LinkToClient,&client_socket, NULL, &thID);
		}
    };
	std::thread listenThread(clientConnect);
	listenThread.detach();
}

void serverSystem::printUsers() {
	if (nclients != 0)
		CCLOG("%d user on-line\n", nclients);
	else CCLOG("No User on line\n");
}

DWORD WINAPI LinkToClient(LPVOID client_socket) {
	SOCKET my_sock;
	my_sock = ((SOCKET*)client_socket)[0];
	char buff[20 * 1024];
	int bytes_recv;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) {
		auto text = std::string(buff, 0, bytes_recv);
		eventListSystem::getInstance()->requestData(text);
		CCLOG("server received: %s", text.c_str());
		send(my_sock, &buff[0], bytes_recv, 0);
	}
	CCLOG("-disconnect\n");
	closesocket(my_sock);
	return 0;
}
