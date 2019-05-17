#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h> // Wincosk2.h должен быть раньше windows!
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")

class Base//абстрактный класс
{
public:
	Base() {}//конструктор
	Base(int _port) : port(_port) {}//конструктор
	virtual int handle() = 0;//виртуальный метод
	int GetPort() const { return port; }//метод не меняющий ничего в классе
	int SetPort(int _port) { port = _port; }

protected:
	int port = 666;
};