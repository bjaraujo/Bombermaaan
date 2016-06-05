
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "CNetwork.h"

int main(int argc, char **argv)
{

    const int serverColor = 10;
    const int clientColor = 14;

    CNetwork Network;

    if (argc < 2)
    {
        std::cout << "Wrong number of arguments!" << std::endl;
        return -1;
    }

    char IpAddressString[32];

    if (argc > 2)
        strcpy(IpAddressString, argv[2]);

    // client mode and ip address given?
    if (strstr(argv[1], "-c") != NULL ||
        strstr(argv[1], "--client") != NULL)
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

    if (!Network.Connect(IpAddressString, 1234))
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

    int len = 0;

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
                std::cout << ": " << recieveBuffer << std::endl;
            else if (Network.NetworkMode() == NETWORKMODE_CLIENT)
                std::cout << ": " << recieveBuffer << std::endl;

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
