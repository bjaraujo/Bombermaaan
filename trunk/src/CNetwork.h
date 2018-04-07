/************************************************************************************

Copyright (C) 2016 Billy Araujo

This file is part of Bombermaaan.

Bombermaaan is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bombermaaan is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bombermaaan.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************************/


/**
*  \file CNetwork.h
*  \brief Header for network communication
*/

#ifndef __CNETWORK_H__
#define __CNETWORK_H__

#include "SDL_net.h"

#include "CCommandChunk.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum ENetworkMode
{
    NETWORKMODE_LOCAL,
    NETWORKMODE_SERVER,
    NETWORKMODE_CLIENT
};

enum ESocketType
{
    SOCKET_SERVER,
    SOCKET_CLIENT
};

#define SDL_ERROR -1

//! Manages the network communication
class CNetwork
{
private:

    ENetworkMode m_NetworkMode;

    TCPsocket m_Socket;
    TCPsocket m_ClientSocket;
    SDLNet_SocketSet m_socketSet;

public:

    CNetwork();
    ~CNetwork(void);

    ENetworkMode   NetworkMode();
    void           SetNetworkMode(ENetworkMode NetworkMode);

    bool           Connect(const char* IpAddressString, int port);
    bool           Disconnect();

    bool           Send(ESocketType SocketType, const char* buf, int len);
    int            Receive(ESocketType SocketType, char* buf, int len);
    int            ReceiveNonBlocking(ESocketType SocketType, char* buf, int len);

    bool           SendCommandChunk(const CCommandChunk& CommandChunk);
    bool           ReceiveCommandChunk(CCommandChunk& CommandChunk);

    bool           SendSnapshot(const CArenaSnapshot& Snapshot);
    bool           ReceiveSnapshot(CArenaSnapshot& Snapshot);

    unsigned long  CheckSum(const char *buf);

};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CNETWORK_H__
