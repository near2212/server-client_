#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Client.h"

//fgjfgbzjfgjf
//ahgadhasgd
Client::Client() : serverAddr("127.0.0.1")//�������� ������������
{

}

Client::Client(int _port, const std::string& _serverAddr) ://�����������
	serverAddr(_serverAddr), Base(_port)//������ �������������
{
}


int Client::init()//�������� ������ init
{
	printf("TCP DEMO CLIENT\n");

	// ��� 1 - ������������� ���������� Winsock
	if (WSAStartup(MAKEWORD(2, 2), (WSADATA*)& buff[0]))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}

	// ��� 2 - �������� ������

	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		return -1;
	}

	// ��� 3 - ��������� ����������
	// ���������� ��������� sockaddr_in - �������� ������ � ����� �������
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	HOSTENT* hst;

	// �������������� IP ������ �� ����������� � ������� ������
	if (inet_addr(serverAddr.c_str()) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(serverAddr.c_str());
	else
	{
		// ������� �������� IP ����� �� ��������� ����� �������
		//if (hst = gethostbyname(SERVERADDR))
		if (hst = gethostbyname("localhost"))
			// hst->h_addr_list �������� �� ������ �������,
			// � ������ ���������� �� ������
			((unsigned long*)& dest_addr.sin_addr)[0] =
			((unsigned long**)hst->h_addr_list)[0][0];
		else
		{
			printf("Invalid address %s\n", serverAddr.c_str());
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}
	}

	// ����� ������� ������� - �������� ���������� ����������
	if (connect(my_sock, (sockaddr*)& dest_addr, sizeof(dest_addr)))
	{
		printf("Connect error %d\n", WSAGetLastError());
		return -1;
	}

	printf("Connection with %s succesfully deployed\n \
             Type quit for quit\n\n", serverAddr.c_str());
	return 0;
}

int Client::handle()
{
	// ��� 4 - ������ � �������� ���������
	int nsize;

	while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)//���� �������� ��������� �� ������� 
	{
		// ������ ����������� ���� � ����� ������
		buff[nsize] = 0;

		// ������� �� �����
		printf("S=>C:%s", buff);

		// ������ ���������������� ���� � ����������
		printf("S<=C:"); fgets(&buff[0], sizeof(buff) - 1, stdin);

		// �������� �� "quit"
		if (!strcmp(&buff[0], "quit\n"))
		{
			// ���������� �����
			printf("Exit...");
			closesocket(my_sock);
			WSACleanup();
			return 0;
		}

		// �������� ������ ������� �������
		send(my_sock, &buff[0], strlen(&buff[0]), 0);
	}
	printf("Recv error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
	return -1;
}