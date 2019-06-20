#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Client.h"

//fgjfgbzjfgjf
//ahgadhasgd
Client::Client() : serverAddr("127.0.0.1")//описание конструктора
{

}

Client::Client(int _port, const std::string& _serverAddr) ://конструктор
	serverAddr(_serverAddr), Base(_port)//спосок инициализации
{
}


int Client::init()//описнаие метода init
{
	printf("TCP DEMO CLIENT\n");

	// Шаг 1 - инициализация библиотеки Winsock
	if (WSAStartup(MAKEWORD(2, 2), (WSADATA*)& buff[0]))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}

	// Шаг 2 - создание сокета

	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		return -1;
	}

	// Шаг 3 - установка соединения
	// заполнение структуры sockaddr_in - указание адреса и порта сервера
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	HOSTENT* hst;

	// преобразование IP адреса из символьного в сетевой формат
	if (inet_addr(serverAddr.c_str()) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(serverAddr.c_str());
	else
	{
		// попытка получить IP адрес по доменному имени сервера
		//if (hst = gethostbyname(SERVERADDR))
		if (hst = gethostbyname("localhost"))
			// hst->h_addr_list содержит не массив адресов,
			// а массив указателей на адреса
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

	// адрес сервера получен - пытаемся установить соединение
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
	// Шаг 4 - чтение и передача сообщений
	int nsize;

	while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)//пока получаем сообщения от сервера 
	{
		// ставим завершающий ноль в конце строки
		buff[nsize] = 0;

		// выводим на экран
		printf("S=>C:%s", buff);

		// читаем пользовательский ввод с клавиатуры
		printf("S<=C:"); fgets(&buff[0], sizeof(buff) - 1, stdin);

		// проверка на "quit"
		if (!strcmp(&buff[0], "quit\n"))
		{
			// Корректный выход
			printf("Exit...");
			closesocket(my_sock);
			WSACleanup();
			return 0;
		}

		// передаем строку клиента серверу
		send(my_sock, &buff[0], strlen(&buff[0]), 0);
	}
	printf("Recv error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
	return -1;
}