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
	const std::string REQUEST_SORT = "sort1 "; //��������� ����1
	const std::string REQUEST_SEND = "send"; //��������� ���� 2

	Server();//�����������
	Server(int port);
	~Server();//���������� 
	int init();
	int handle();//���������������� �����
	typedef void (Server::* CallbackFunc)(void);//����������� ���� ��������� �� �������, ������� ������ �� ���������� � �� ���������
	DWORD WINAPI ToClient(LPVOID client_socket);
	void sort();//����������
	void send_name();//���������� ������� ����
	//std::string save_name;
	void rec_sort(std::vector<std::string> a, int b);//���������� ���������. � ���� �� �����������, �� ����� ���� �������

private:

	std::string* buff_s;
	std::vector<std::string> message;//������ ����
	std::map<std::string, CallbackFunc> fn_map;//������� ������,� ���������� �� ������� ���� Callback 

	char buff[1024];//����� char
	SOCKET mysocket;// �����
};

