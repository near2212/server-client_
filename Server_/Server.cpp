#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>
#include <winsock2.h> // Wincosk2.h должен быть раньше windows!
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Server.h"

/**/

// макрос для печати количества активных пользователей
#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); \
         else printf("No User on line\n");
static int nclients = 0;//определение колличества подключенных клиентов
// Эта функция создается в отдельном потоке
// и обсуживает очередного подключившегося клиента независимо от остальных
DWORD WINAPI Server::ToClient(LPVOID client_socket)
{

	SOCKET my_sock;
	my_sock = ((SOCKET*)client_socket)[0];
	char buff[20 * 1024];//TODO: и с ним сотворить динамическую магию а то больно толстый
	//создание констант прри помощи define
#define sHELLO "Fill with comand send then sort it with command sort1(for sort) and sort2 (for rec_sort)it and watch the result\r\n"
#define what_toAdd "print name to add\r\n"
#define no_data "nothing to sort, first use command SEND òî add info to DB\r\n"
#define success "successfully added"

	// отправляем клиенту приветствие
	send(my_sock, sHELLO, sizeof(sHELLO), 0);

	// цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
	int bytes_recv;

	int s;

	if (!buff_s)//если память под этот элемент не выделена
		buff_s = new std::string();//динамически выделяем память для строки в которой временно будут храниться передаваемые клиентом слова и имена


	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) &&
		bytes_recv != SOCKET_ERROR)//пока передуются данные
		//a = strlen(buff);
	{
		s = 0;//счетчик букв записи в строковый буфер
		for (int i = 0; buff[i] != '\n'; i++)
		{
			s += 1;
		}
		buff_s->append(buff, s);//заполнение строкового буфера

		// ставим завершающий ноль в конце строки
		buff[bytes_recv] = 0;
		if (!strcmp(&buff[0], "sort1\n"))//если команда sort получена
		{
			if (message.size() > 1)//если есть что сортировать
			{
				(this->*fn_map[REQUEST_SORT])();
				send(my_sock, &this->buff[0], sizeof(this->buff), 0);
				//вызываем метод sort по ключу
			}
			else
				send(my_sock, no_data, sizeof(no_data), 0);//вывод ошибки содежимого в message

		}
		if (!strcmp(&buff[0], "sort2\n"))//если команда sort получена
		{
			if (message.size() > 1)//если есть что сортировать
			{
				std::string temp = "";
				Server::rec_sort(this->message, this->message.size());
				for (int i = 0; i < this->message.size(); i++)
					temp = temp + " " + this->message[i];
				strcpy(buff, temp.c_str());
				send(my_sock, buff, sizeof(buff), 0);
				//вызываем метод sort по ключу
			}
			else
				send(my_sock, no_data, sizeof(no_data), 0);//вывод ошибки содежимого в message

		}

		else if (!strcmp(&buff[0], "send\n"))//если команда send
		{

			send(my_sock, what_toAdd, sizeof(what_toAdd), 0);//предлагаем пользователю ввести слово
		}
		else
		{
			(this->*fn_map[REQUEST_SEND])();
			//вызов метода send по ключу. (на самом деле добавление будет происходить независимо от нажатия команды Send и это не хорошо)


			send(my_sock, success, sizeof(success), 0);//сообщение об успехе
		}
		buff_s->clear();//чистим содержимое
	}
// возращения функцией recv ошибки - соединение с клиентом разорвано
	delete buff_s;// возращения функцией recv ошибки - соединение с клиентом разорвано
	//send(my_sock, &buff[0], bytes_recv, 0);

// возращения функцией recv ошибки - соединение с клиентом разорваноî
	nclients--; // уменьшаем счетчик активных клиентов
	printf("-disconnect\n"); PRINTNUSERS

		// закрываем сокет
		closesocket(my_sock);
	return 0;
}

Server::Server()
	: Server(666)
{

}

Server::Server(int port)
	: Base(port),
	buff_s(nullptr)
{
	this->fn_map[REQUEST_SORT] = &Server::sort;//в словаре у нас лежат указатели на функции sort и send_name
	this->fn_map[REQUEST_SEND] = &Server::send_name;
}

Server::~Server() {
	if (buff_s)
		delete buff_s;
}

int Server::init()
{
	printf("TCP SERVER DEMO\n");
	// Шаг 1 - Инициализация Библиотеки Сокетов
	// т.к. возвращенная функцией информация не используется
	// ей передается указатель на рабочий буфер, преобразуемый к указателю
	// на структуру WSADATA.
	// Такой прием позволяет сэкономить одну переменную, однако, буфер
	// должен быть не менее полкилобайта размером (структура WSADATA
	// занимает 400 байт)
	if (WSAStartup(MAKEWORD(2, 2), (WSADATA*)& buff[0]))
	{
		// Ошибка!
		printf("Error WSAStartup %d\n", WSAGetLastError());
		return -1;
	}

	// Шаг 2 - создание сокета
	// AF_INET - сокет Интернета
	// SOCK_STREAM - потоковый сокет (с установкой соединения)
	// 0 - по умолчанию выбирается TCP протокол
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		// Ошибка!
		printf("Error socket %d\n", WSAGetLastError());
		WSACleanup(); // Äåèíèöèëèçàöèÿ áèáëèîòåêè Winsock
		return -1;
	}

	// Шаг 3 - связывание сокета с локальным адресом
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(port); // не забываем о сетевом порядке!!!
	local_addr.sin_addr.s_addr = 0;// сервер принимает подключения
									// на все свои IP-адреса

	// вызываем bind для связывания
	if (bind(mysocket, (sockaddr*)& local_addr, sizeof(local_addr)))
	{
		// Ошибка
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket); // закрываем сокет!
		WSACleanup();
		return -1;
	}

	return 0;
}
int Server::handle()
{
	// Шаг 4 - ожидание подключений
	// размер очереди - 0x100
	if (listen(mysocket, 0x100))
	{
		// Ошибка
		printf("Error listen %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	printf("Waiting for connection...\n");

	// Шаг 5 - извлекаем сообщение из очереди
	SOCKET client_socket;// сокет для клиента
	sockaddr_in client_addr;// адрес клиента (заполняется системой)

	// функции accept необходимо передать размер структуры
	int client_addr_size = sizeof(client_addr);

	// цикл извлечения запросов на подключение из очереди
	while ((client_socket = accept(mysocket, (sockaddr*)& client_addr, \
		& client_addr_size)))
	{
		nclients++;  // увеличиваем счетчик подключившихся клиентов
		// пытаемся получить имя хоста
		HOSTENT* hst;
		hst = gethostbyaddr((char*)& client_addr.sin_addr.s_addr, 4, AF_INET);

		// вывод сведений о клиенте
		printf("+%s [%s] new connect!\n",
			(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS

				// Вызов нового потока для обслужвания клиента
			// Да, для этого рекомендуется использовать _beginthreadex
			// но, поскольку никаких вызовов функций стандартной Си библиотеки
			// поток не делает, можно обойтись и CreateThread
			DWORD thID;
		ToClient(&client_socket);
		// CreateThread(NULL, NULL, ToClient, &client_socket, NULL, &thID);//TODO:ðàçäåëèòü íà ïîòîêè äëÿ ïîääåðæêè áîëåå ÷åì îäíîãî êëèåíòà
	}
	return 0;
}

void Server::sort()//сортировка вектора Message
{
	std::sort(message.begin(), message.end());
	std::string a = "";
	int s = 0;
	//std::string buff_s;
	for (int i = 0; i < message.size(); i++)
		a = a + " " + message[i];
	strcpy(buff, a.c_str());
	//std::cout << buff;
	//send(my_sock, &buff[0], a.length(), 0);
}
void Server::send_name() // метод send который дает команду записать в вектор messages новую строку
{
{
	if (buff_s)
		message.push_back(*buff_s);
	// TODO: Можно напечатать в консоль сообщение об ошибке
}
}
void Server::rec_sort(std::vector<std::string> a, int b)//сортировка рекурсией при желании ею можно заменить сортировку send, но такого желания нет)
	std::string temp = "";
	//int len;
	//len = a.size();
	for (int n = 1; n < b; n++)
	{
		if (strcmp(a[n - 1].c_str(), a[n].c_str()) > 0)
		{
			temp = a[n];
			a[n] = a[n - 1];
			a[n - 1] = temp;
		}
	}

	if (b > 1) rec_sort(a, b - 1);//условие выхода из рекурсии
	message = a;
}


