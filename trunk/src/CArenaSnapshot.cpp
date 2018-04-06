// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2010 Markus Drescher

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
 *  \file CArenaSnapshot.cpp
 *  \brief Arena snapshot for network exchange
 */

#include "StdAfx.h"
#include "CArenaSnapshot.h"

#include <string.h>

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//#define ENABLE_PRINT_CONSOLE

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::Create (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::Destroy (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::Begin (void)
{
    m_Position = 0;

#ifdef ENABLE_PRINT_CONSOLE
    theConsole.Write("BEGIN SNAPSHOT\n");
#endif
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::ReadInteger (int* pValue)
{
    ASSERT(m_Position + sizeof(int) < ARENA_SNAPSHOT_SIZE);

    memcpy(pValue, &m_Buffer[m_Position], sizeof(int)); // #3078839
    
#ifdef ENABLE_PRINT_CONSOLE
    theConsole.Write("READ INT %d FROM POS %d\n", *pValue, m_Position);
#endif
    
    m_Position += sizeof(int);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::ReadFloat (float* pValue)
{
    ASSERT(m_Position + sizeof(float) < ARENA_SNAPSHOT_SIZE);

    memcpy(pValue, &m_Buffer[m_Position], sizeof(float)); // #3078839

#ifdef ENABLE_PRINT_CONSOLE
    theConsole.Write("READ FLOAT %f FROM POS %d\n", *pValue, m_Position);
#endif

    m_Position += sizeof(float);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::ReadBoolean (bool* pValue)
{
    ASSERT(m_Position + sizeof(bool) < ARENA_SNAPSHOT_SIZE);

    memcpy(pValue, &m_Buffer[m_Position], sizeof(bool)); // #3078839

#ifdef ENABLE_PRINT_CONSOLE
    theConsole.Write("READ BOOL %d FROM POS %d\n", *pValue, m_Position);
#endif

    m_Position += sizeof(bool);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::WriteInteger (int Value)
{
    ASSERT(m_Position + sizeof(int) < ARENA_SNAPSHOT_SIZE);
    
    memcpy(&m_Buffer[m_Position], &Value, sizeof(int)); // #3078839

#ifdef ENABLE_PRINT_CONSOLE
    theConsole.Write("WRITE INT %d TO POS %d\n", Value, m_Position);
#endif

    m_Position += sizeof(int);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::WriteFloat (float Value)
{
    ASSERT(m_Position + sizeof(float) < ARENA_SNAPSHOT_SIZE);

    memcpy(&m_Buffer[m_Position], &Value, sizeof(float)); // #3078839

#ifdef ENABLE_PRINT_CONSOLE
    theConsole.Write("WRITE FLOAT %f TO POS %d\n", Value, m_Position);
#endif

    m_Position += sizeof(float);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CArenaSnapshot::WriteBoolean (bool Value)
{
    ASSERT(m_Position + sizeof(bool) < ARENA_SNAPSHOT_SIZE);

    memcpy(&m_Buffer[m_Position], &Value, sizeof(bool)); // #3078839

#ifdef ENABLE_PRINT_CONSOLE
    theConsole.Write("WRITE BOOL %d TO POS %d\n", Value, m_Position);
#endif

    m_Position += sizeof(bool);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
