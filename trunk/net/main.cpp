
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "CNetwork.h"

int main(int argc, char **argv)
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

    if (Network.NetworkMode() == NETWORKMODE_SERVER)
        std::cout << "Waiting for client to connect..." << std::endl;

    if (!Network.Connect(IpAddressString))
    {
        std::cout << "Unable to connect!" << std::endl;
        return -1;
    }

    if (Network.NetworkMode() == NETWORKMODE_SERVER)
        std::cout << "Successfully connected to client." << std::endl;
    else if (Network.NetworkMode() == NETWORKMODE_CLIENT)
        std::cout << "Successfully connected to server." << std::endl;

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

            std::cout << cur;

            if (cur != 13 && len < 511)
            {

                sendBuffer[len++] = cur;

            }
            else
            {

                if (len < 511)
                    sendBuffer[len++] = '\0';

                std::cout << std::endl;

                if (Network.NetworkMode() == NETWORKMODE_CLIENT)
                    Network.Send(SOCKET_SERVER, sendBuffer, len);
                else if (Network.NetworkMode() == NETWORKMODE_SERVER)
                    Network.Send(SOCKET_CLIENT, sendBuffer, len);

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
                Received += Network.Receive(SOCKET_CLIENT, &recieveBuffer[Received], Bufsize);
            else if (Network.NetworkMode() == NETWORKMODE_CLIENT)
                Received += Network.Receive(SOCKET_SERVER, &recieveBuffer[Received], Bufsize);

            if (Received == SDL_ERROR)
            {
                theLog.Write("sent error: %s\n", SDLNet_GetError());
                return false;
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
                std::cout  << "client says: " << recieveBuffer << std::endl;
            else if (Network.NetworkMode() == NETWORKMODE_CLIENT)
                std::cout << "server says: " << recieveBuffer << std::endl;

            std::cout << "> ";
        }

    };

    return 0;

}
