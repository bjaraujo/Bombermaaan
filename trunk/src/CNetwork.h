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

#ifdef WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <sys/io.h>
#include <sys/socket.h>
#endif
    
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

//! Manages the network communication
class CNetwork
{
private:
    
    ENetworkMode m_NetworkMode;
    
    SOCKET m_Socket;
    SOCKET m_ClientSocket;
    
public:

                   CNetwork ();
                   ~CNetwork (void);
    
    ENetworkMode   NetworkMode();
	void		   SetNetworkMode(ENetworkMode NetworkMode);

    bool           Connect(const char* IpAddressString);
	bool		   Disconnect();

	bool           Send(ESocketType SocketType, const char* buf, size_t len, int flags);
	int            Receive(ESocketType SocketType, char* buf, size_t len, int flags);
	unsigned char  GetCheckSum(const char* buf, size_t len);

	bool           SendCommandChunk(const CCommandChunk& CommandChunk);
	bool           ReceiveCommandChunk(CCommandChunk& CommandChunk);

	bool           SendSnapshot(const CArenaSnapshot& Snapshot);
	bool           ReceiveSnapshot(CArenaSnapshot& Snapshot);

	bool           SendFooter(ESocketType SocketType);
	bool           ReceiveFooter(ESocketType SocketType);

};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CNETWORK_H__
