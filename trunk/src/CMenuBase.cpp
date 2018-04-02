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
 *  \file CMenuBase.cpp
 *  \brief The menu base
 */

#include "StdAfx.h"
#include "CMenuBase.h"
#include "CMenu.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CTimer.h"
#include "CFont.h"
#include "CSound.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define TRANSITION_DURATION         0.350f    //!< Duration of the transition that happens before exiting

#define FONT_SPRITELAYER            1       //!< Sprite layer where to draw characters using the font

#define FRAME_POSITION_X            30      //!< Position of the menu frame
#define FRAME_POSITION_Y            52
#define FRAME_SPRITE                0       //!< Sprite number of the menu frame sprite 
#define FRAME_PRIORITY              1       //!< Priority to use in the menu sprite layer when drawing the frame
#define FRAME_SPRITELAYER           0       //!< Sprite layer where to draw the menu frame

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuBase::CMenuBase (void)
{
    // Initialize the pointers to NULL so that we 
    // can easily detect the ones we forgot to set.
    m_pDisplay = NULL;
    m_pInput = NULL;
    m_pSound = NULL;
    m_pOptions = NULL;
    m_pTimer = NULL;
    m_pFont = NULL;
    
    m_MenuModeTime = 0.0f;
    m_HaveToExit = false;
    m_ExitMenuAction = MENUACTION_NONE;
    m_ExitMenuModeTime = 0.0f;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuBase::~CMenuBase (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBase::Create (void)
{
    // Check if all the objects to communicate with are set
    ASSERT (m_pDisplay != NULL);
    ASSERT (m_pSound != NULL);
    ASSERT (m_pInput != NULL);
    ASSERT (m_pOptions != NULL);
    ASSERT (m_pTimer != NULL);
    ASSERT (m_pFont != NULL);

    // Reset menu mode time (no time has been elapsed in this menu mode yet)
    m_MenuModeTime = 0.0f;

    // Don't have to exit this menu mode yet
    m_HaveToExit = false;

    // Create and initialize the font for our needs
    m_pFont->Create ();
    m_pFont->SetShadow (false);
    m_pFont->SetSpriteLayer (FONT_SPRITELAYER);

    OnCreate();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBase::Destroy (void)
{
    // Destroy the font
    m_pFont->Destroy ();

    OnDestroy();
}
    
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EMenuAction CMenuBase::Update (void)
{
    // Increase time elapsed in this menu mode
    m_MenuModeTime += m_pTimer->GetDeltaTime ();

    // If we don't have to exit this menu mode yet
    if (!m_HaveToExit)
    {
        // If NEXT control is pressed
        if (m_pInput->GetMainInput().TestNext())
        {
            // Don't play menu next sound because the choices of the user
            // could not be correct and we may have to play an error sound instead.
            
            OnNext ();
        }
        // If PREVIOUS control is pressed
        else if (m_pInput->GetMainInput().TestPrevious())
        {
            // Play the menu previous sound
            m_pSound->PlaySample (SAMPLE_MENU_PREVIOUS);

            OnPrevious ();
        }
        // If UP control is pressed
        else if (m_pInput->GetMainInput().TestUp())
        {
            // Play the menu beep sound
            m_pSound->PlaySample (SAMPLE_MENU_BEEP);

            OnUp ();
        }
        // If DOWN control is pressed
        else if (m_pInput->GetMainInput().TestDown())
        {
            // Play the menu beep sound
            m_pSound->PlaySample (SAMPLE_MENU_BEEP);

            OnDown ();
        }
        // If LEFT control is pressed
        else if (m_pInput->GetMainInput().TestLeft())
        {
            // Play the menu beep sound
            m_pSound->PlaySample (SAMPLE_MENU_BEEP);

            OnLeft ();
        }
        // If RIGHT control is pressed
        else if (m_pInput->GetMainInput().TestRight())
        {
            // Play the menu beep sound
            m_pSound->PlaySample (SAMPLE_MENU_BEEP);

            OnRight ();
        }

        // Update the menu screen
        OnUpdate ();
    }
    // If the transition has been entirely done (enough time has elapsed)
    else if (m_MenuModeTime >= m_ExitMenuModeTime + TRANSITION_DURATION)
    {
        // It's OK to exit now!
        // Ask for the menu action we saved
        return m_ExitMenuAction;
    }

    // Don't have to change menu mode nor game mode
    return MENUACTION_NONE;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBase::Display (void)
{
    // If we don't have to exit this menu mode yet
    if (!m_HaveToExit)
    {
        m_pDisplay->SetOrigin (0, 0);

        // Draw the menu frame sprite
        m_pDisplay->DrawSprite (FRAME_POSITION_X, 
                                FRAME_POSITION_Y, 
                                NULL, 
                                NULL, 
                                BMP_MENU_FRAME_1, 
                                FRAME_SPRITE, 
                                FRAME_SPRITELAYER, 
                                FRAME_PRIORITY);

        // Display the menu screen
        OnDisplay ();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBase::Exit (EMenuAction ExitMenuAction)
{
    // Start exiting this menu mode
    m_HaveToExit = true;

    // Remember when we realized we had to exit this menu mode
    m_ExitMenuModeTime = m_MenuModeTime;

    // Remember what menu action to ask for when exiting
    m_ExitMenuAction = ExitMenuAction;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
