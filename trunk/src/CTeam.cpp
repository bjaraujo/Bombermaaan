// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CTeam.cpp
 *  \brief The team (bombers are playing)
 */

#include "StdAfx.h"

#include "CTeam.h"

CTeam::CTeam()
{

    m_TeamId = -1;
    m_Victorious = false;
}

CTeam::~CTeam() = default;

void CTeam::SetTeamId(int TeamId) { m_TeamId = TeamId; }

int CTeam::GetTeamId() { return m_TeamId; }

void CTeam::SetVictorious(bool Victorious) { m_Victorious = Victorious; }

bool CTeam::IsVictorious() { return m_Victorious; }
