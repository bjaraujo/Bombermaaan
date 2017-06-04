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

    m_NetworkMode = NETWORKMODE_UNKNOWN;
    m_udpSocket = NULL;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CNetwork::~CNetwork(void)
{


}

void CNetwork::Sleep(const int ms)
{
     
#ifdef __linux__
    usleep(ms * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#else
    ::Sleep(ms);
#endif

}

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

bool CNetwork::Initialize()
{

    if (SDLNet_Init() == SDL_ERROR)
    {
        std::cout << "Init failed: " << SDLNet_GetError() << std::endl;
        return false;
    }

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::Send(const char* buf, int len)
{

    UDPpacket packet;
    packet.address = m_ip;
    packet.channel = -1;
    packet.data = (Uint8*)buf;
    packet.len = len;

    //std::cout << "Sending: " << data << std::endl;

    int numSent = SDLNet_UDP_Send(m_udpSocket, packet.channel, &packet);

    if (!numSent) {
        std::cout << "SDLNet_UDP_Send: " << SDLNet_GetError() << std::endl;
        return false;
    }

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

int CNetwork::Receive(char* buf)
{

    int numRecv = -1;

    UDPpacket* packet;
    
    if (!(packet = SDLNet_AllocPacket(1024)))
    {
        std::cout << "SDLNet_AllocPacket: " << SDLNet_GetError() << std::endl;
        return numRecv;
    }

    numRecv = SDLNet_UDP_Recv(m_udpSocket, packet);

    if (numRecv) 
    {
        buf = (char*)packet->data;
    }

    SDLNet_FreePacket(packet);
    
    return numRecv;

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

    m_udpSocket = SDLNet_UDP_Open(port);

    if (!m_udpSocket) {
        std::cout << "UDP open failed:" << SDLNet_GetError() << std::endl;
        return false;
    }

    if (SDLNet_ResolveHost(&m_ip, IpAddressString, port) == SDL_ERROR)
    {
        std::cout << "Connection failed: " << SDLNet_GetError() << std::endl;
        return false;
    }

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::Disconnect()
{

    SDLNet_UDP_Close(m_udpSocket);

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::SendCommandChunk(const CCommandChunk& CommandChunk)
{

    union {
        unsigned long LongValue;
        char ByteArray[4];
    } LongBytes;

    // Send checksum
    LongBytes.LongValue = CheckSum((const char*)&CommandChunk);
    Send((const char*)&LongBytes.ByteArray, 4);

    // Send client command chunk to the server
    Send((const char*)&CommandChunk, sizeof(CommandChunk));

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::ReceiveCommandChunk(CCommandChunk& CommandChunk)
{

    bool res = false;

    union {
        unsigned long LongValue;
        char ByteArray[4];
    } LongBytes;

    int BufSize = 4 + sizeof(CommandChunk);
    int Received = 0;

    char* recvBuf = new char[BufSize];

    // Receive checksum and data
    do {

        Received += this->Receive(&recvBuf[Received]);

        if (Received == SDL_ERROR)
        {
            theLog.Write("CommandChunk: recieve error: %s\n", SDLNet_GetError());
            break;
        }

    } while (Received < BufSize);

    if (Received == BufSize)
    {

        memcpy(LongBytes.ByteArray, &recvBuf[0], 4);

        if (CheckSum(&recvBuf[4]) == LongBytes.LongValue)
        {
            memcpy((char *)&CommandChunk, &recvBuf[4], sizeof(CommandChunk));
            res = true;
        }

    }

    delete[] recvBuf;

    return res;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::SendSnapshot(const CArenaSnapshot& Snapshot)
{

    union {
        unsigned long LongValue;
        char ByteArray[4];
    } LongBytes;

    // Send checksum
    LongBytes.LongValue = this->CheckSum((const char*)&Snapshot);
    Send((const char*)&LongBytes.ByteArray, 4);

    // Send snapshot to the client
    Send((const char*)&Snapshot, sizeof(Snapshot));

    return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::ReceiveSnapshot(CArenaSnapshot& Snapshot)
{

    bool res = false;

    union {
        unsigned long LongValue;
        char ByteArray[4];
    } LongBytes;

    int BufSize = 4 + sizeof(Snapshot);
    int Received = 0;

    char* recvBuf = new char[BufSize];

    do {

        Received += this->Receive(&recvBuf[Received]);

        if (Received == SDL_ERROR)
        {
            theLog.Write("Snapshot: recieve error: %s\n", SDLNet_GetError());
            break;
        }

    } while (Received < BufSize);

    if (Received == BufSize)
    {

        memcpy(LongBytes.ByteArray, &recvBuf[0], 4);

        if (CheckSum(&recvBuf[4]) == LongBytes.LongValue)
        {
            memcpy((char *)&Snapshot, &recvBuf[4], sizeof(Snapshot));
            res = true;
        }

    }

    delete[] recvBuf;

    return res;

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

