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
 *  \file CCloudManager.cpp
 *  \brief The cloud manager
 */

#include "StdAfx.h"
#include "CCloudManager.h"
#include "CDisplay.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CCloudManager::CCloudManager (void)
{
    m_pDisplay = NULL;
    
    m_CloudSprites[0] = BMP_TITLE_CLOUD_1;
    m_CloudSprites[1] = BMP_TITLE_CLOUD_2;
    m_CloudSprites[2] = BMP_TITLE_CLOUD_3;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CCloudManager::~CCloudManager (void)
{
    Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCloudManager::Create (void)
{
    ASSERT (m_pDisplay != NULL);

    // Set the sprite for each cloud
    m_CloudSprites[0] = BMP_TITLE_CLOUD_1;
    m_CloudSprites[1] = BMP_TITLE_CLOUD_2;
    m_CloudSprites[2] = BMP_TITLE_CLOUD_3;

    // Set the properties of each cloud
    m_Clouds[0].Create (50, 18, 138, 46, 50.0f, 0.0f, 6.0f);
    m_Clouds[1].Create (150, 74, 106, 46, 40.0f, 0.0f, 3.0f);
    m_Clouds[2].Create (-100, 130, 66, 22, 60.0f, 0.0f, 5.0f);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCloudManager::Destroy (void)
{
    // Destroy the clouds
    for (int i = 0 ; i < NUMBER_OF_CLOUDS ; i++)
    {
        m_Clouds[i].Destroy();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCloudManager::Update (float DeltaTime)
{
    // Update the clouds
    for (int i = 0 ; i < NUMBER_OF_CLOUDS ; i++)
    {
        m_Clouds[i].Update (DeltaTime);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCloudManager::Display (void)
{
    RECT Clip;

    // Display the clouds
    for (int i = 0 ; i < NUMBER_OF_CLOUDS ; i++)
    {
        // We need to prepare a clip structure of the size of the game view
        // because of the tiled background which moves to animate
        Clip.left   = 0;
        Clip.top    = 0;
        Clip.right  = VIEW_WIDTH;
        Clip.bottom = VIEW_HEIGHT;

        // Draw the scroller sprite
        m_pDisplay->DrawSprite (m_Clouds[i].GetPositionX(),   // Position of the current tile
                                m_Clouds[i].GetPositionY(),                      
                                NULL,                           // Draw entire tile
                                &Clip,                          // Clip with game view
                                m_CloudSprites[i], 
                                0, 
                                0, 
                                1);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
