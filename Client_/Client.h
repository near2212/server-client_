#pragma once
#include <string>

//#include "../Base/Header.h"
#include "Header.h"
class Client : public Base//наследование от абстрактного класса
{
public:
	Client();//конструктор
	Client(int _port, const std::string& _serverAddr);//конструктор
	int init();
	int handle();
	void SetServerAddr(const std::string& _serverAddr) { serverAddr = _serverAddr; }//сеттер, создание строки при помощи спецификатора Const
	std::string GetServerAddr() const { return serverAddr; }//геттер

private:
	char buff[1024];//буфер для разных нужд
	SOCKET my_sock;//сокет

	std::string serverAddr;
};
