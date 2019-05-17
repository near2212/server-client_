#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h> //Wincosk2.h должен быть раньше windows!
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")

class Base
{
public:
	Base() : Base(666) {}//конструктор вызывает конструктор
	Base(int _port) : port(_port) {}//конструктор
	virtual ~Base() {};//виртуальный деструктор
	virtual int handle() = 0;//чисто виртуальный метод
	int GetPort() const { return port; }//геттер
	void SetPort(int _port) { port = _port; }//сеттер

protected:
	int port;
};
