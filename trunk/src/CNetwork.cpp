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
