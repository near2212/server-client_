#pragma once
#pragma once
#include <map>
#include <vector>
#include <string>
//#include "../Base/Header.h"
#include "Header.h"

class Server : public Base
{
public:
	const std::string REQUEST_SORT = "sort1 "; //объ€вл€ем ключ1
	const std::string REQUEST_SEND = "send"; //объ€вл€ем ключ 2

	Server();//конструктор
	Server(int port);
	~Server();//деструктор 
	int init();
	int handle();//переопределенный метод
	typedef void (Server::* CallbackFunc)(void);//определение типа указател€ на функцию, котора€ ничего не возвращает и не принимает
	DWORD WINAPI ToClient(LPVOID client_socket);
	void sort();//сортировка
	void send_name();//заполнени€ вектора имен
	//std::string save_name;
	void rec_sort(std::vector<std::string> a, int b);//сортировка рекурсией. в коде не примен€етс€, но можно если хочетс€

private:

	std::string* buff_s;
	std::vector<std::string> message;//вектор имен
	std::map<std::string, CallbackFunc> fn_map;//словарь команд,и указателей на функции типа Callback 

	char buff[1024];//буфер char
	SOCKET mysocket;// сокет
};

