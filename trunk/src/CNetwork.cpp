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
*  \file CNetwork.cpp
*  \brief Network communication
*/

#include "StdAfx.h"
#include "CNetwork.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CNetwork::CNetwork()
{

    m_NetworkMode = NETWORKMODE_LOCAL;

    m_Socket = NULL;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CNetwork::~CNetwork(void)
{


}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

ENetworkMode CNetwork::NetworkMode()
{

    return m_NetworkMode;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CNetwork::SetNetworkMode(ENetworkMode NetworkMode)
{

    m_NetworkMode = NetworkMode;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::Connect(const char* IpAddressString, int port)
{

    if (SDLNet_Init() == SDL_ERROR)
    {
        theLog.Write("init failed: %s\n", SDLNet_GetError());

        return false;
    }

    if (m_NetworkMode == NETWORKMODE_SERVER)
    {

        IPaddress ip;

        if (SDLNet_ResolveHost(&ip, NULL, port) == SDL_ERROR)
        {
            theLog.Write("listen failed: %s\n", SDLNet_GetError());

            return false;
        }

        m_Socket = SDLNet_TCP_Open(&ip);

        if (!m_Socket)
        {
            theLog.Write("open failed: %s\n", SDLNet_GetError());

            return false;
        }

        m_socketSet = SDLNet_AllocSocketSet(2);

        SDLNet_TCP_AddSocket(m_socketSet, m_Socket);

        // Wait for the client
        while (1)
        {

            m_ClientSocket = SDLNet_TCP_Accept(m_Socket);

            Sleep(1000);

            if (m_ClientSocket)
                break;

        }

        SDLNet_TCP_AddSocket(m_socketSet, m_ClientSocket);

    }
    else if (m_NetworkMode == NETWORKMODE_CLIENT)
    {

        IPaddress ip;

        if (SDLNet_ResolveHost(&ip, IpAddressString, port) == SDL_ERROR)
        {
            theLog.Write("connection failed: %s\n", SDLNet_GetError());

            return false;
        }

        m_Socket = SDLNet_TCP_Open(&ip);

        if (!m_Socket)
        {
            theLog.Write("open failed: %s\n", SDLNet_GetError());

            return false;
        }

        m_socketSet = SDLNet_AllocSocketSet(1);

        SDLNet_TCP_AddSocket(m_socketSet, m_Socket);

    }

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::Disconnect()
{

    if (m_NetworkMode != NETWORKMODE_LOCAL)
    {

        SDLNet_TCP_Close(m_Socket);

        if (m_NetworkMode == NETWORKMODE_SERVER)
            SDLNet_TCP_Close(m_ClientSocket);

    }

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
*  \return true, if the send was successful
*
*  Send packet
*/

bool CNetwork::Send(ESocketType SocketType, const char* buf, int len)
{

    int Sent = 0;

    if (SocketType == SOCKET_SERVER)
        Sent = SDLNet_TCP_Send(m_Socket, buf, len);
    else if (SocketType == SOCKET_CLIENT)
        Sent = SDLNet_TCP_Send(m_ClientSocket, buf, len);

    if (Sent == SDL_ERROR)
    {
        theLog.Write("sent error: %s\n", SDLNet_GetError());
        return false;
    }

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
*  \return true, if the send was successful
*
*  Receive packet
*/

int CNetwork::Receive(ESocketType SocketType, char* buf, int len)
{

    if (SocketType == SOCKET_SERVER)
    {
        return SDLNet_TCP_Recv(m_Socket, buf, len);
    }
    else if (SocketType == SOCKET_CLIENT)
    {
        return SDLNet_TCP_Recv(m_ClientSocket, buf, len);
    }
    else
        return 0;

}

/**
*  \return true, if the send was successful
*
*  Receive packet
*/

int CNetwork::ReceiveNonBlocking(ESocketType SocketType, char* buf, int len)
{

    int active = SDLNet_CheckSockets(m_socketSet, 1);

    if (active > 0)
    {
        if (SocketType == SOCKET_SERVER)
        {
            if (SDLNet_SocketReady(m_Socket))
                return SDLNet_TCP_Recv(m_Socket, buf, len);
            else
                return 0;
        }
        else if (SocketType == SOCKET_CLIENT)
        {
            if (SDLNet_SocketReady(m_ClientSocket))
                return SDLNet_TCP_Recv(m_ClientSocket, buf, len);
            else
                return 0;
        }
        else
            return 0;

    }
    else
        return 0;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::SendCommandChunk(const CCommandChunk& CommandChunk)
{

    // Send checksum
    union {
        unsigned long LongValue;
        char ByteArray[4];
    } LongBytes;

    LongBytes.LongValue = this->CheckSum((const char*)&CommandChunk);
    this->Send(SOCKET_SERVER, (const char*)&LongBytes.ByteArray, 4);

    // Send client command chunk to the server
    this->Send(SOCKET_SERVER, (const char*)&CommandChunk, sizeof(CommandChunk));

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::ReceiveCommandChunk(CCommandChunk& CommandChunk)
{

    union {
        unsigned long LongValue;
        char ByteArray[4];
    } LongBytes;

    {
        int bufsize = 4;
        int Received = 0;

        // Receive checksum
        do {

            Received += this->Receive(SOCKET_CLIENT, &LongBytes.ByteArray[Received], bufsize);

            if (Received == SDL_ERROR)
            {
                theLog.Write("recieve error: %s\n", SDLNet_GetError());
                break;
            }

            if (Received > 0)
                bufsize -= Received;
            else
                break;

        } while (bufsize > 0);

    }

    // Receive client command chunk 
    {
        int bufsize = sizeof(CommandChunk);
        int Received = 0;

        char* recvBuf = new char[bufsize];

        do {

            Received += this->Receive(SOCKET_CLIENT, &recvBuf[Received], bufsize);

            if (Received == SDL_ERROR)
            {
                theLog.Write("recieve error: %s\n", SDLNet_GetError());
                delete[] recvBuf;
                return false;
            }

            bufsize -= Received;

        } while (Received < sizeof(CommandChunk));

        if (Received == sizeof(CommandChunk))
        {

            if (this->CheckSum(recvBuf) == LongBytes.LongValue)
            {
                memcpy((char *)&CommandChunk, recvBuf, sizeof(CommandChunk));
                delete[] recvBuf;
                return true;
            }

        }

        delete[] recvBuf;

    }

    return false;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::SendSnapshot(const CArenaSnapshot& Snapshot)
{

    // Send checksum
    union {
        unsigned long LongValue;
        char ByteArray[4];
    } LongBytes;

    LongBytes.LongValue = this->CheckSum((const char*)&Snapshot);
    this->Send(SOCKET_CLIENT, (const char*)&LongBytes.ByteArray, 4);

    // Send snapshot to the client
    return this->Send(SOCKET_CLIENT, (const char*)&Snapshot, sizeof(Snapshot));

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::ReceiveSnapshot(CArenaSnapshot& Snapshot)
{

    union {
        unsigned long LongValue;
        char ByteArray[4];
    } LongBytes;

    {
        int bufsize = 4;
        int Received = 0;

        // Receive checksum
        do {

            Received += this->Receive(SOCKET_SERVER, &LongBytes.ByteArray[Received], bufsize);

            if (Received == SDL_ERROR)
            {
                theLog.Write("recieve error: %s\n", SDLNet_GetError());
                break;
            }

            if (Received > 0)
                bufsize -= Received;
            else
                break;

        } while (bufsize > 0);

    }

    // Receive and apply the arena snapshot from the server
    {
        int bufsize = sizeof(Snapshot);
        int Received = 0;

        char* recvBuf = new char[bufsize];

        do {

            Received += this->Receive(SOCKET_SERVER, &recvBuf[Received], bufsize);

            if (Received == SDL_ERROR)
            {
                theLog.Write("sent error: %s\n", SDLNet_GetError());
                delete[] recvBuf;
                return false;
            }

            if (Received > 0)
                bufsize -= Received;
            else
                break;

        } while (bufsize > 0);

        if (Received == sizeof(Snapshot))
        {

            if (this->CheckSum(recvBuf) == LongBytes.LongValue)
            {
                memcpy((char *)&Snapshot, recvBuf, sizeof(Snapshot));
                delete[] recvBuf;
                return true;
            }
        }

        delete[] recvBuf;

    }

    return false;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

unsigned long CNetwork::CheckSum(const char *buf)
{
    unsigned long hash = 5381;
    int c;

    while (c = *buf++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

