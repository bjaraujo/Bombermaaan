
#include <conio.h>
#include <stdio.h>

#include "CNetwork.h"

int main (int argc, char **argv)
{
    
	CNetwork Network;

	if (argc < 2)
	{
		std::cout << "Wrong number of arguments!" << std::endl;
		return -1;
	}

	char IpAddressString[32];

	const char *pos = strstr(argv[1], "--client");

	if (pos != NULL)
		strcpy(IpAddressString, pos + 8);

	if (pos == NULL)
	{
		pos = strstr(argv[1], "-c");
		if (pos != NULL)
			strcpy(IpAddressString, pos + 3);
	}

	// client mode and ip address given?
	if (pos != NULL && strlen(argv[1]) > (unsigned int)(pos - argv[1] + 2))
	{
		std::cout << "*** STARTING AS CLIENT" << std::endl;
		Network.SetNetworkMode(NETWORKMODE_CLIENT);

	}
	else if (strstr(argv[1], "-s") != NULL ||
		strstr(argv[1], "--server") != NULL)
	{
		std::cout << "*** STARTING AS SERVER" << std::endl;
		Network.SetNetworkMode(NETWORKMODE_SERVER);
	}

	if (!Network.Connect(IpAddressString))
	{
		std::cout << "Unable to connect!" << std::endl;
		return -1;
	}

	char sendBuffer[512];
	char recieveBuffer[512];
	
	char ping = 0;

	int len = 0;

	std::cout << "> ";

	while (true) {

		// Send message
		if (_kbhit())
		{
			char cur = _getch();

			if (len > 511)
				len = 511;

			std::cout << cur;

			if (cur != 13)
				sendBuffer[len++] = cur;
			else{
				sendBuffer[len] = '\0';
				std::cout << std::endl;

				if (Network.NetworkMode() == NETWORKMODE_CLIENT)
				{
					Network.Send(SOCKET_SERVER, sendBuffer, len);
				}
				else if (Network.NetworkMode() == NETWORKMODE_SERVER)
				{
					Network.Send(SOCKET_CLIENT, sendBuffer, len);
				}

				len = 0;

				std::cout << "> ";

			}

		}

		Sleep(20);

		// Recieve message
		if (Network.NetworkMode() == NETWORKMODE_CLIENT)
		{

			int Received = 0;
			int Bufsize = 512;

			do {

				Received += Network.Receive(SOCKET_SERVER, &recieveBuffer[Received], 1);

				if (Received == SDL_ERROR)
				{
					theLog.Write("sent error: %s\n", SDLNet_GetError());
					return false;
				}

				Bufsize -= Received;

			} while (Received > 0);

			if (Received > 0)
				std::cout << recieveBuffer << std::endl;

		}
		else if (Network.NetworkMode() == NETWORKMODE_SERVER)
		{

			int Received = 0;
			int Bufsize = 512;

			do {

				Received += Network.Receive(SOCKET_CLIENT, &recieveBuffer[Received], 1);

				if (Received == SDL_ERROR)
				{
					theLog.Write("sent error: %s\n", SDLNet_GetError());
					return false;
				}

				Bufsize -= Received;

			} while (Received > 0);

			if (Received > 0)
				std::cout << recieveBuffer << std::endl;

		}

	};

	return 0;

}
