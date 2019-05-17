#pragma once
#include <string>

//#include "../Base/Header.h"
#include "Header.h"
class Client : public Base//������������ �� ������������ ������
{
public:
	Client();//�����������
	Client(int _port, const std::string& _serverAddr);//�����������
	int init();
	int handle();
	void SetServerAddr(const std::string& _serverAddr) { serverAddr = _serverAddr; }//������, �������� ������ ��� ������ ������������� Const
	std::string GetServerAddr() const { return serverAddr; }//������

private:
	char buff[1024];//����� ��� ������ ����
	SOCKET my_sock;//�����

	std::string serverAddr;
};
