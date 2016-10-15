
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "anyoption.h"

#include "CNetwork.h"

int main(int argc, char **argv)
{

	AnyOption opt;

	opt.addUsage("");
	opt.addUsage("Usage: ");
	opt.addUsage("");
	opt.addUsage(" -h  --help  		  Prints this help ");
	opt.addUsage(" -s  --server       Starts as server");
	opt.addUsage(" -c  --client [ip]  Starts as client");
	opt.addUsage(" -n  --nick [nick]  Chat nick name");
	opt.addUsage("");

	opt.setFlag("help", 'h');
	opt.setFlag("server", 's');
	opt.setOption("client", 'c');
	opt.setOption("nick", 'n');

	opt.processCommandArgs(argc, argv);

	if (!opt.hasOptions()) {
		opt.printUsage();
		return 0;
	}

	if (opt.getFlag("help") || opt.getFlag('h'))
		opt.printUsage();

	CNetwork Network;
	char *aNickName;

	if (opt.getValue("nick") != NULL || opt.getValue('n') != NULL)
	{
		aNickName = opt.getValue("nick");
		std::cout << "nick = " << aNickName << endl;
	}

	if (opt.getFlag("server") || opt.getFlag('s'))
	{
		std::cout << "*** STARTING AS SERVER" << std::endl;
		Network.SetNetworkMode(NETWORKMODE_SERVER);

		std::cout << "Waiting for client to connect..." << std::endl;

		if (!Network.Connect("", 1234))
		{
			std::cout << "Unable to connect!" << std::endl;
			return -1;
		}

	}
	else if (opt.getValue("client") || opt.getValue('c'))
	{
		std::cout << "*** STARTING AS CLIENT" << std::endl;
		Network.SetNetworkMode(NETWORKMODE_CLIENT);

		std::cout << "Connecting to: " << opt.getValue("client") << std::endl;

		if (!Network.Connect(opt.getValue("client"), 1234))
		{
			std::cout << "Unable to connect!" << std::endl;
			return -1;
		}
	}

	int len = 0;

	char anotherNickName[80];

	if (Network.NetworkMode() == NETWORKMODE_SERVER)
	{
		std::cout << "Successfully connected to client." << std::endl;

		Network.Send(SOCKET_CLIENT, aNickName, 80);
		Network.Receive(SOCKET_CLIENT, &anotherNickName[0], 80);
		std::cout << "Talking to: " << anotherNickName << std::endl;

	}
	else if (Network.NetworkMode() == NETWORKMODE_CLIENT)
	{
		std::cout << "Successfully connected to server." << std::endl;

		Network.Send(SOCKET_SERVER, aNickName, 80);
		Network.Receive(SOCKET_SERVER, &anotherNickName[0], 80);
		std::cout << "Talking to: " << anotherNickName << std::endl;

	}

    char sendBuffer[512];
    char recieveBuffer[512];

    std::cout << "> ";

    while (true) {

        // Send message
        if (_kbhit())
        {
            char cur = _getch();

            if (cur == 8)
            {
                if (len > 0)
                {
                    std::cout << cur;
                    cout << " ";
                    std::cout << cur;
                    len--;
                }
            }
            else
            {
                std::cout << cur;


            }

            if (cur != 13 && len < 511)
            {

                if (cur != 8)
                    sendBuffer[len++] = cur;

            }
            else
            {

                sendBuffer[len++] = '\0';

                std::cout << std::endl;

                if (Network.NetworkMode() == NETWORKMODE_SERVER)
                    Network.Send(SOCKET_CLIENT, sendBuffer, len);
                else if (Network.NetworkMode() == NETWORKMODE_CLIENT)
                    Network.Send(SOCKET_SERVER, sendBuffer, len);

                len = 0;

				std::cout << "> ";

            }

        }

        Sleep(20);

        // Recieve messages
        int Received = 0;
        int Bufsize = 512;

        do {

            if (Network.NetworkMode() == NETWORKMODE_SERVER)
                Received += Network.ReceiveNonBlocking(SOCKET_CLIENT, &recieveBuffer[Received], Bufsize);
            else if (Network.NetworkMode() == NETWORKMODE_CLIENT)
                Received += Network.ReceiveNonBlocking(SOCKET_SERVER, &recieveBuffer[Received], Bufsize);

            if (Received == SDL_ERROR)
            {
                theLog.Write("recieve error: %s\n", SDLNet_GetError());
                break;
            }

            if (Received > 0)
                Bufsize -= Received;
            else
                break;

        } while (Bufsize > 0);

        if (Received > 0)
        {
            std::cout << std::endl;

            if (Network.NetworkMode() == NETWORKMODE_SERVER)
				std::cout << anotherNickName << ": " << recieveBuffer << std::endl;
			else if (Network.NetworkMode() == NETWORKMODE_CLIENT)
				std::cout << anotherNickName << ": " << recieveBuffer << std::endl;

            // Beep
            std::cout << '\a';

            std::cout << "> ";

            // Add current buffer
            for (int i = 0; i < len; i++)
                std::cout << sendBuffer[i];

        }

    };

    return 0;

}
