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
 *  \file CHurryMessage.cpp
 *  \brief The hurry message
 */

#include "StdAfx.h"
#include "CHurryMessage.h"
#include "CSound.h"
#include "CScroller.h"
#include "CDisplay.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CHurryMessage::CHurryMessage (CDisplay* pDisplay, CSound* pSound)
{
    ASSERT (pDisplay != NULL);
    ASSERT (pSound != NULL);

    m_pDisplay = pDisplay;
    m_pSound = pSound;

    // Play the hurry up sound
    m_pSound->PlaySample (SAMPLE_HURRY);

    // Stop the match music song (the normal one)
    m_pSound->StopSong (SONG_MATCH_MUSIC_1_NORMAL);

    m_Scroller.Create (-68, 96, 69, 16, 308.0f, 0.0f, -1.0f);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CHurryMessage::~CHurryMessage (void)
{
    // Delete the scroller
    m_Scroller.Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \return true, if the scroller is out of the screen
 *
 *  @see SCroller::Update()
 */

bool CHurryMessage::Update (float DeltaTime)
{
    //! Update the scroller (move)
    m_Scroller.Update (DeltaTime);

    // Return whether the hurry message is over,
    // that is to say if the scroller is out of the screen.
    return m_Scroller.OutOfBounds();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CHurryMessage::Display (void)
{
    // We need to prepare a clip structure of the size of the game view
    // because of the tiled background which moves to animate
    RECT Clip;
    Clip.left   = 0;
    Clip.top    = 0;
    Clip.right  = VIEW_WIDTH;
    Clip.bottom = VIEW_HEIGHT;

    // Draw the hurry message
    m_pDisplay->DrawSprite (m_Scroller.GetPositionX(),   // Position of the current tile
                            m_Scroller.GetPositionY(),                      
                            NULL,                           // Draw entire tile
                            &Clip,                          // Clip with game view
                            BMP_HURRY,
                            0, 
                            700, 
                            -1);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
