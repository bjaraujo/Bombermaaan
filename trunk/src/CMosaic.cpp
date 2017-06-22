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
 *  \file CMosaic.cpp
 *  \brief Mosaic background in menus
 */

#include "StdAfx.h"
#include "CMosaic.h"
#include "CDisplay.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMosaic::CMosaic (void)
{
    m_pDisplay = NULL;
    
    m_SpriteTable = 0;
    m_Sprite = 0;
    m_SpriteLayer = 0;
    m_PriorityInLayer = 0;
    m_SpriteWidth = 0;
    m_SpriteHeight = 0;
    m_TileCountX = 0; 
    m_TileCountY = 0; 
    m_SpeedX = 0.0f;
    m_SpeedY = 0.0f;
    m_BackgroundPositionX = 0.0f;
    m_BackgroundPositionY = 0.0f;
    m_iBackgroundPositionX = 0;
    m_iBackgroundPositionY = 0;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMosaic::~CMosaic (void)
{
    Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMosaic::Create (int SpriteTable, int Sprite, int SpriteLayer, int PriorityInLayer, int SpriteWidth, int SpriteHeight, int TileCountX, int TileCountY, float SpeedX, float SpeedY)
{
    ASSERT (m_pDisplay != NULL);
    
    // Currently, this class has to be used with a positive SpeedX and a negative SpeedY.
    ASSERT (SpeedX >= 0.0f);
    ASSERT (SpeedY <= 0.0f);

    m_SpriteTable = SpriteTable;
    m_Sprite = Sprite;
    m_SpriteLayer = SpriteLayer;
    m_PriorityInLayer = PriorityInLayer;
    m_SpriteWidth = SpriteWidth;
    m_SpriteHeight = SpriteHeight;
    m_TileCountX = TileCountX; 
    m_TileCountY = TileCountY; 
    m_SpeedX = SpeedX;
    m_SpeedY = SpeedY;
    m_BackgroundPositionX = 0.0f;
    m_BackgroundPositionY = 0.0f;
    m_iBackgroundPositionX = 0;
    m_iBackgroundPositionY = 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMosaic::Destroy (void)
{
  
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMosaic::Update (float DeltaTime)
{
    // If the mosaic has to scroll
    if (m_SpeedX > 0.0f || m_SpeedY > 0.0f)
    {    
        // Update tiled background position (and therefore animate the background on the game view)
        m_BackgroundPositionX += m_SpeedX * DeltaTime;
        m_BackgroundPositionY += m_SpeedY * DeltaTime;

        // Compute new integer position of the background
        m_iBackgroundPositionX = int (m_BackgroundPositionX);
        m_iBackgroundPositionY = int (m_BackgroundPositionY);
    
        // While the background is too much on the right
        while (m_iBackgroundPositionX > 0)
        {
            // Decrease background X position (make the background go to the left)
            m_iBackgroundPositionX -= m_SpriteWidth;
        }
    
        // While the background is too much above
        while (m_iBackgroundPositionY + m_SpriteHeight < 0)
        {
            // Increase background Y position (make the background go down)
            m_iBackgroundPositionY += m_SpriteHeight;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMosaic::Display (void)
{
    // We need to prepare a clip structure of the size of the game view
    // because of the tiled background which moves to animate
    RECT Clip;
    Clip.left   = 0;
    Clip.top    = 0;
    Clip.right  = VIEW_WIDTH;
    Clip.bottom = VIEW_HEIGHT;

    // Prepare position variables to work with in order to 
    // draw every tile of the background at current position
    int X = m_iBackgroundPositionX;
    int Y = m_iBackgroundPositionY;

    // For each tile row to draw
    for (int TileY = 0 ; TileY < m_TileCountY ; TileY++)
    {
        // For each tile column to draw
        for (int TileX = 0 ; TileX < m_TileCountX ; TileX++)
        {
            // Draw the background tile
            m_pDisplay->DrawSprite (X,                      // Position of the current tile
                                    Y,                      
                                    NULL,                   // Draw entire tile
                                    &Clip,                  // Clip with game view
                                    m_SpriteTable, 
                                    m_Sprite, 
                                    m_SpriteLayer, 
                                    m_PriorityInLayer);

            // Next tile on the tile row to draw
            X += m_SpriteWidth;
        }

        // Go back to first tile in tile row
        X = m_iBackgroundPositionX;

        // Next tile on the tile column
        Y += m_SpriteHeight;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
