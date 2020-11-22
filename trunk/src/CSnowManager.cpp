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
 *  \file CSnowManager.cpp
 *  \brief The snow manager
 */

#include "CSnowManager.h"
#include "CDisplay.h"
#include "StdAfx.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CSnowManager::CSnowManager()
{
    m_pDisplay = nullptr;

    for (int i = 0; i < NUMBER_OF_SNOWFLAKES; i++)
    {
        m_SnowSprites[i] = BMP_TITLE_SNOWFLAKE;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CSnowManager::~CSnowManager() { Destroy(); }

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSnowManager::Create()
{
    ASSERT(m_pDisplay != nullptr);

    // Set the sprite for each snow
    
    // Set the properties of each snow
    for (int i = 0; i < NUMBER_OF_SNOWFLAKES; i++)
    {
        m_SnowSprites[i] = BMP_TITLE_SNOWFLAKE;
        m_Snows[i].Create(rand() % VIEW_WIDTH, rand() % VIEW_HEIGHT, 21, 24, 0.0f, 50.0f + rand() % 20 * 1.0f, 8.0f);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSnowManager::Destroy()
{
    // Destroy the snows
    for (int i = 0; i < NUMBER_OF_SNOWFLAKES; i++)
    {
        m_Snows[i].Destroy();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSnowManager::Update(float DeltaTime)
{
    // Update the snows
    for (int i = 0; i < NUMBER_OF_SNOWFLAKES; i++)
    {
        m_Snows[i].Update(DeltaTime);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CSnowManager::Display()
{
    RECT Clip;

    // Display the snows
    for (int i = 0; i < NUMBER_OF_SNOWFLAKES; i++)
    {
        // We need to prepare a clip structure of the size of the game view
        // because of the tiled background which moves to animate
        Clip.left = 0;
        Clip.top = 0;
        Clip.right = VIEW_WIDTH;
        Clip.bottom = VIEW_HEIGHT;

        // Draw the scroller sprite
        m_pDisplay->DrawSprite(
            m_Snows[i].GetPositionX(), // Position of the current tile
            m_Snows[i].GetPositionY(),
            NULL, // Draw entire tile
            &Clip, // Clip with game view
            m_SnowSprites[i], 0, 0, 1);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
