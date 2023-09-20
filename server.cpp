#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")

using namespace std;

//int main()
//{
//	WSAData wsaData;
//	WSAStartup(MAKEWORD(2, 2), &wsaData);
//
//	int HostNumber = 0x12345678;
//	printf("%x\n", HostNumber);
//	printf("%x\n", htonl(HostNumber));
//	printf("%x\n", ntohl(htonl(HostNumber)));
//
//	WSACleanup();
//	return 0;
//}


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

	srand((unsigned int)time(nullptr));

	while (true)
	{
		SOCKADDR_IN ClientSock;
		ZeroMemory(&ClientSock, sizeof(ClientSock));
		int ClientSockLength = sizeof(ClientSock);

		SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSock, &ClientSockLength);

		char Message[1024] = { 0, };
		char Operators[] = { '+', '-', '/', '*', '%' };

		int First = (rand() % 10000) + 1;
		int Second = (rand() % 10000) + 1;

		int NetworkFirst = htonl(First);
		int NetworkSecond = htonl(Second);

		memcpy(Message, &NetworkFirst, sizeof(NetworkFirst));
		memcpy(Message+5, &NetworkSecond, sizeof(NetworkSecond));
		Message[4] = Operators[rand() % 5];

		int SendByte = send(ClientSocket, Message, 9, 0);

		char Result[1024] = { 0, };
		int RecvByte = recv(ClientSocket, Result, 4, MSG_WAITALL);
		int ResultNumber = 0;
		memcpy(&ResultNumber, Result, 4);
		ResultNumber = ntohl(ResultNumber);

		cout << First;
		cout << Message[4];
		cout << Second;
		cout << "=";
		//cout << *(int*)(&(Result+4)) << endl;
		cout << ResultNumber << endl;

		closesocket(ClientSocket);
	}

	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}