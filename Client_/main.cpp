#include "Client.h"

int main()
{
	Client client(666, "127.0.0.1");
	client.init();
	client.handle();
}