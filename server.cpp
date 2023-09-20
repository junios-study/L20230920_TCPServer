#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN ListenSock;
	memset(&ListenSock, 0, sizeof(ListenSocket));
	ListenSock.sin_family = PF_INET;
	ListenSock.sin_port = htons(7777);
	ListenSock.sin_addr.s_addr = INADDR_ANY;
	//inet_pton(AF_INET,
		//"127.0.0.1", &(ListenSock.sin_addr.s_addr));
	bind(ListenSocket, (SOCKADDR*)&ListenSock, sizeof(ListenSock));
	listen(ListenSocket, SOMAXCONN);

	while (true)
	{
		SOCKADDR_IN ClientSock;
		ZeroMemory(&ClientSock, sizeof(ClientSock));
		int ClientSockLength = sizeof(ClientSock);

		SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSock, &ClientSockLength);

		srand((unsigned int)time(nullptr));

		char Message[1024] = { 0, };

		int First = (rand() % 90) + 10;
		int Second = (rand() % 90) + 10;
		sprintf_s(Message, "%2d+%2d", First, Second);

		int SendByte = send(ClientSocket, Message, 5, 0);

		char Result[1024] = { 0, };
		int RecvByte = recv(ClientSocket, Result, sizeof(Result), 0);

		cout << Message << "=";
		cout << Result << endl;

		closesocket(ClientSocket);
	}

	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}