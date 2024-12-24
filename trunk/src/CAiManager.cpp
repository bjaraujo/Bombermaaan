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
 *  \file CAiManager.cpp
 *  \brief The AI management functions
 */

#include "CAiManager.h"
#include "CAiBomber.h"
#include "COptions.h"
#include "StdAfx.h"

CAiManager::CAiManager()
{
    m_pDisplay = nullptr;
    for (int Player = 0; Player < MAX_PLAYERS; Player++)
        m_pBombers[Player] = nullptr;
}

CAiManager::~CAiManager() { Destroy(); }

void CAiManager::Create(COptions* pOptions)
{
    for (int Player = 0; Player < MAX_PLAYERS; Player++)
    {
        if (pOptions->GetBomberType(Player) == BOMBERTYPE_COM && m_Arena.GetArena()->GetBomber(Player).Exist())
        {
            m_pBombers[Player] = new CAiBomber;
            m_pBombers[Player]->SetArena(&m_Arena);
            m_pBombers[Player]->SetDisplay(m_pDisplay);
            m_pBombers[Player]->Create(Player);
        }
    }

    m_Arena.SetDisplay(m_pDisplay);
    m_Arena.Create();
}

void CAiManager::Destroy()
{
    m_Arena.Destroy();

    for (int Player = 0; Player < MAX_PLAYERS; Player++)
    {
        if (m_pBombers[Player] != nullptr)
        {
            delete m_pBombers[Player];
            m_pBombers[Player] = nullptr;
        }
    }
}

void CAiManager::Update(float DeltaTime)
{
    m_Arena.Update(DeltaTime);

    for (int Player = 0; Player < MAX_PLAYERS; Player++)
        if (m_pBombers[Player] != nullptr)
            m_pBombers[Player]->Update(DeltaTime);
}
