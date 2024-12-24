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
 *  \file CScores.cpp
 *  \brief The scores
 */

#include "CScores.h"
#include "COptions.h"
#include "StdAfx.h"

CScores::CScores()
{
    // Initialize the pointers to nullptr so that we
    // can easily detect the ones we forgot to set.
    m_pOptions = nullptr;

    Reset();
}

CScores::~CScores() = default;

void CScores::Reset()
{
    // Reset player scores
    for (int Player = 0; Player < MAX_PLAYERS; Player++)
    {
        m_Scores[Player] = 0;
    }

    // Reset draw games count
    m_DrawGamesCount = 0;
}

void CScores::RaisePlayerScore(int Player)
{
    // If maximum has not been reached
    if (m_Scores[Player] < m_pOptions->GetBattleCount())
    {
        // Increase score
        m_Scores[Player]++;
    }
}

void CScores::RaiseDrawGamesCount()
{
    // If maximum has not been reached
    if (m_DrawGamesCount < MAX_DRAWGAME_SCORE)
    {
        // Increase count
        m_DrawGamesCount++;
    }
}
