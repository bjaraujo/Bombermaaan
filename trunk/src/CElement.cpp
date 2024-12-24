// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer

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
 *  \file CElement.cpp
 *  \brief An element of the arena
 */

#include "CElement.h"
#include "CArenaSnapshot.h"
#include "StdAfx.h"

CElement::CElement()
{
    m_Exist = false;
    m_pDisplay = nullptr;
    m_pSound = nullptr;
    m_pArena = nullptr;
}

CElement::~CElement() { }

void CElement::Create()
{
    m_Exist = true;
    ASSERT(m_pArena != nullptr);
    ASSERT(m_pDisplay != nullptr);
    ASSERT(m_pSound != nullptr);
}

void CElement::Destroy()
{
    m_Exist = false;
    m_pDisplay = nullptr;
    m_pSound = nullptr;
    m_pArena = nullptr;
}

void CElement::WriteSnapshot(CArenaSnapshot& Snapshot)
{
    Snapshot.WriteBoolean(m_Exist);

    if (m_Exist)
        OnWriteSnapshot(Snapshot);
}

void CElement::ReadSnapshot(CArenaSnapshot& Snapshot)
{
    Snapshot.ReadBoolean(&m_Exist);

    if (m_Exist)
        OnReadSnapshot(Snapshot);
}
