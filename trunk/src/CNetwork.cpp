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

CNetwork::CNetwork ()
{

	m_NetworkMode = NETWORKMODE_LOCAL;	

    m_Socket = INVALID_SOCKET;
    m_ClientSocket = INVALID_SOCKET;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CNetwork::~CNetwork (void)
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

bool CNetwork::Connect (const char* IpAddressString)
{
        
#ifdef WIN32
	WSAData WsaData;

	if (WSAStartup(MAKEWORD(1, 1), &WsaData) != 0)
	{
		theConsole.Write("WSAStartup failed\n");
		return false;
	}
#endif

    if (m_NetworkMode == NETWORKMODE_SERVER)
    {

        m_Socket = socket(AF_INET, SOCK_STREAM, 0);

        SOCKADDR_IN SocketAddress;
        SocketAddress.sin_family = AF_INET;
        SocketAddress.sin_addr.s_addr = INADDR_ANY;
        SocketAddress.sin_port = htons(12345);

        theConsole.Write("bind\n");

        if (bind(m_Socket, (LPSOCKADDR)&SocketAddress, sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            theConsole.Write("bind failed\n");
#ifdef WIN32
            closesocket(m_Socket);
#else
            close(m_Socket);
#endif
            return false;
        }

        theConsole.Write("listen\n");

        if (listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)
        {
            theConsole.Write("listen failed\n");
#ifdef WIN32
            closesocket(m_Socket);
#else
            close(m_Socket);
#endif
            return false;
        }

        theConsole.Write("accept\n");

        SOCKADDR_IN Address;
        int Size = sizeof(SOCKADDR);

#ifdef WIN32
        m_ClientSocket = accept(m_Socket, (LPSOCKADDR)&Address, &Size);
#else
        m_ClientSocket = accept(m_Socket, (LPSOCKADDR)&Address, (socklen_t *)&Size);
#endif

        if (m_ClientSocket == INVALID_SOCKET)
        {
            theConsole.Write("accept failed\n");
            return false;
        }
    }
    else if (m_NetworkMode == NETWORKMODE_CLIENT)
    {
        
        m_Socket = socket(AF_INET, SOCK_STREAM, 0);

        theConsole.Write("connect to %s\n", IpAddressString);

        u_long RemoteAddress = inet_addr(IpAddressString);

        if (RemoteAddress == INADDR_NONE)
        {
            theConsole.Write("The address %s is not a dotted IP address.\n");
            return false;
        }

        SOCKADDR_IN SocketAddress;
        SocketAddress.sin_family = AF_INET;
        memcpy(&SocketAddress.sin_addr, &RemoteAddress, sizeof(u_long));
        SocketAddress.sin_port = htons(12345);

        if (connect(m_Socket, (LPSOCKADDR)&SocketAddress, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
        {
            theConsole.Write("connect failed\n");
            return false;
        }
    }
    
	return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::Disconnect ()
{

    if (m_NetworkMode != NETWORKMODE_LOCAL)
    {
#ifdef WIN32
        closesocket(m_Socket);
        closesocket(m_ClientSocket);

        if (WSACleanup() == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEINPROGRESS)
            {
                WSACancelBlockingCall();
                WSACleanup();
            }
        }
#else
        close(MySocket);
        close(ClientSocket);
#endif
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

bool CNetwork::Send (ESocketType SocketType, const char* buf, size_t len, int flags)
{

	int Sent = SOCKET_ERROR;

    if (SocketType == SOCKET_SERVER)
		Sent = send(m_Socket, buf, len, flags);
    else if (SocketType == SOCKET_CLIENT)
		Sent = send(m_ClientSocket, buf, len, flags);
    
#ifdef WIN32
	if (Sent == SOCKET_ERROR)
	{
		theConsole.Write("sent error: %d\n", WSAGetLastError());
		return false;
	}
#else
	if (Sent == -1)
	{
		theConsole.Write("sent error : %d\n", Sent);
		return false;
	}
#endif

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

int CNetwork::Receive (ESocketType SocketType, char* buf, size_t len, int flags)
{
        
    if (SocketType == SOCKET_SERVER)
        return recv(m_Socket, buf, len, 0);
	else if (SocketType == SOCKET_CLIENT)
		return recv(m_ClientSocket, buf, len, 0);
	else
		return 0;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

unsigned char CNetwork::GetCheckSum(const char* buf, size_t len)
{

	int sum = 0;

	if (len > 0)
	{
		for (int i = 0; i < len; ++i) {
			sum += buf[i];
		}

		sum %= 256;
	}

	char ch = sum;

	return ~ch + 1;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::SendCommandChunk(const CCommandChunk& CommandChunk)
{

	// Send header
	char* headerBuf = new char[20];

	// Packet start Id
	headerBuf[0] = 0x03;
	headerBuf[2] = 0x08;
	headerBuf[3] = 0x45;
	headerBuf[4] = 0x88;
	headerBuf[5] = 0x45;
	headerBuf[6] = 0x08;
	headerBuf[7] = 0xA6;
	headerBuf[8] = 0xA6;
	headerBuf[9] = 0x03;

	// Type Id
	headerBuf[10] = 'C';
	headerBuf[11] = 'O';
	headerBuf[10] = 'M';
	headerBuf[11] = 'M';
	headerBuf[12] = 'A';
	headerBuf[13] = 'N';
	headerBuf[14] = 'D';
	headerBuf[15] = 0x00;
	headerBuf[16] = 0x00;
	headerBuf[17] = 0x00;
	headerBuf[19] = 0x00;

	// Send header to the client
	this->Send(SOCKET_SERVER, (const char*)&headerBuf, 20, 0);

	delete[] headerBuf;

	// Send client command chunk to the server
	this->Send(SOCKET_SERVER, (char*)&CommandChunk, sizeof(CommandChunk), 0);

	// Send a checksum to the server
	unsigned char ckSum = this->GetCheckSum((const char*)&CommandChunk, sizeof(CommandChunk));
	this->Send(SOCKET_SERVER, (const char*)&ckSum, 1, 0);

	// Send a footer
	this->SendFooter(SOCKET_SERVER);

	return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::ReceiveCommandChunk(CCommandChunk& CommandChunk)
{

	// Receive client command chunk
	int bufsize = sizeof(CommandChunk) + 1;
	char* recvBuf = new char[bufsize];
	int Received = 0;

	do {
		Received += this->Receive(SOCKET_CLIENT, &recvBuf[Received], bufsize, 0);
#ifdef WIN32
		if (Received == SOCKET_ERROR)
		{
			theConsole.Write("recv error (server): %d\n", WSAGetLastError());
			return false;
		}
#else
		if (Received == -1)
		{
			theConsole.Write("recv error : %d\n", Received);
			return false;
		}
#endif

		bufsize -= Received;

	} while (Received < sizeof(CommandChunk));

	if (Received == sizeof(CommandChunk) + 1)
	{
		if ((int)this->GetCheckSum(recvBuf, sizeof(CommandChunk)) == (int)recvBuf[Received - 1])
		{
			memcpy((char *)&CommandChunk, recvBuf, sizeof(CommandChunk));
		}
	}
	else
	{
		this->ReceiveFooter(SOCKET_CLIENT);
	}

	delete[] recvBuf;

	return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::SendSnapshot(const CArenaSnapshot& Snapshot)
{

	// Send header
	char* headerBuf = new char[20];

	// Packet start Id
	headerBuf[0] = 0x03;
	headerBuf[2] = 0x08;
	headerBuf[3] = 0x45;
	headerBuf[4] = 0x88;
	headerBuf[5] = 0x45;
	headerBuf[6] = 0x08;
	headerBuf[7] = 0xA6;
	headerBuf[8] = 0xA6;
	headerBuf[9] = 0x03;

	// Type Id
	headerBuf[10] = 'S';
	headerBuf[11] = 'N';
	headerBuf[10] = 'A';
	headerBuf[11] = 'P';
	headerBuf[12] = 'S';
	headerBuf[13] = 'H';
	headerBuf[14] = 'O';
	headerBuf[15] = 'T';
	headerBuf[16] = 0x00;
	headerBuf[17] = 0x00;
	headerBuf[19] = 0x00;

	// Send header to the client
	this->Send(SOCKET_CLIENT, (const char*)&headerBuf, 20, 0);

	delete[] headerBuf;

	// Send snapshot to the client
	this->Send(SOCKET_CLIENT, (const char*)&Snapshot, sizeof(Snapshot), 0);

	// Send a checksum
	unsigned char ckSum = this->GetCheckSum((const char*)&Snapshot, sizeof(Snapshot));
	this->Send(SOCKET_CLIENT, (const char*)&ckSum, 1, 0);

	// Send a footer
	this->SendFooter(SOCKET_CLIENT);

	return true;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CNetwork::ReceiveSnapshot(CArenaSnapshot& Snapshot)
{

	// Receive and apply the arena snapshot from the server
	int bufsize = sizeof(Snapshot) + 21;
	char* recvBuf = new char[bufsize];
	int Received = 0;

	do {
		Received += this->Receive(SOCKET_SERVER, &recvBuf[Received], bufsize, 0);
#ifdef WIN32
		if (Received == SOCKET_ERROR)
		{
			theConsole.Write("recv error (client): %d\n", WSAGetLastError());
			break;
		}
#else
		if (Received == -1)
		{
			theConsole.Write("recv error : %d\n", Received);
			break;
		}
#endif

		bufsize -= Received;

	} while (Received < sizeof(Snapshot));

	if (Received == sizeof(Snapshot) + 1)
	{
		if ((int)this->GetCheckSum(recvBuf, sizeof(Snapshot)) == (int)recvBuf[Received - 1])
		{
			memcpy((char *)&Snapshot, recvBuf, sizeof(Snapshot));
		}
	}
	else
	{
		this->ReceiveFooter(SOCKET_SERVER);
	}

	delete[] recvBuf;

	return true;

}

bool CNetwork::SendFooter(ESocketType SocketType)
{

	char*footerBuf = new char[15];

	// Packet end Id
	footerBuf[0] = 0x13;
	footerBuf[1] = 0x18;
	footerBuf[2] = 0x55;
	footerBuf[3] = 0x98;
	footerBuf[4] = 0x55;
	footerBuf[5] = 0x18;
	footerBuf[6] = 0xA6;
	footerBuf[7] = 0xA6;
	footerBuf[8] = 0xA6;
	footerBuf[9] = 0x13;

	footerBuf[10] = 'E';
	footerBuf[11] = 'N';
	footerBuf[12] = 'D';
	footerBuf[13] = 0x00;
	footerBuf[14] = 0x00;

	// Send footer to the client
	this->Send(SocketType, (const char*)&footerBuf, 15, 0);

}

bool CNetwork::ReceiveFooter(ESocketType SocketType)
{

	char* ch;

	int Received = 0;

	// Read until the end of the packet
	do 
	{

		Received = this->Receive(SocketType, ch, 1, 0);

		if (*ch == 0x13)
		{

			char*footerBuf = new char[15];

			this->Receive(SocketType, footerBuf, 14, 0);

			for (int i = 0; i < 13; i++)
				footerBuf[i + 1] = footerBuf[i];

			bool isEnd = (footerBuf[0] == 0x13) &&
				(footerBuf[1] == 0x18) &&
				(footerBuf[2] == 0x55) &&
				(footerBuf[3] == 0x98) &&
				(footerBuf[4] == 0x55) &&
				(footerBuf[5] == 0x18) &&
				(footerBuf[6] == 0xA6) &&
				(footerBuf[7] == 0xA6) &&
				(footerBuf[8] == 0xA6) &&
				(footerBuf[9] == 0x13) &&
				(footerBuf[10] == 'E') &&
				(footerBuf[11] == 'N') &&
				(footerBuf[12] == 'D') &&
				(footerBuf[13] == 0x00) &&
				(footerBuf[14] == 0x00);

			if (isEnd)
				break;

		}

	} while (Received > 0);

}

