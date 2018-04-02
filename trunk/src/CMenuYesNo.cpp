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
 *  \file CMenuYesNo.cpp
 *  \brief The yes/no menu
 */

#include "StdAfx.h"
#include "CMenuYesNo.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CSound.h"
#include "CGame.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define INITIAL_TEXT_POSITION_X     226     //!< Position X of the first option's text in the menu
#define INITIAL_TEXT_POSITION_Y     117     //!< Position Y of the first option's text in the menu
#define TEXT_SPACE_Y                15      //!< Space Y in pixels between two options text.

#define CURSOR_HAND_SPACE_X         -25     //!< Space X to apply to text position to get the cursor hand sprite position X
#define CURSOR_HAND_SPACE_Y         -2      //!< Space Y to apply to text position to get the cursor hand sprite position Y
#define CURSOR_HAND_SPRITE          0       //!< Sprite number to use for the cursor hand

#define SPRITE_LAYER_FRAME          800     //!< Sprite layer where to draw the menu frame sprite
#define SPRITE_LAYER_ABOVE_FRAME    801     //!< Sprite layer where to draw the sprites that should be above the menu frame

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuYesNo::CMenuYesNo (void)
{
    m_pDisplay = NULL;
    m_pInput = NULL;
    m_pSound = NULL;
    m_pTimer = NULL;

    m_YesNo = true;
    m_Active = false;
    m_WasSoundPaused = false;
    m_Title[0] = '\0';
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuYesNo::~CMenuYesNo (void)
{
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuYesNo::Create (void)
{
    // Check if the links to the objects to communicate with are valid
    ASSERT (m_pDisplay != NULL);
    ASSERT (m_pInput != NULL);
    ASSERT (m_pSound != NULL);
    ASSERT (m_pTimer != NULL);
    
    // Initialize the font
    m_Font.Create ();
    m_Font.SetShadow (false);
    m_Font.SetSpriteLayer (SPRITE_LAYER_ABOVE_FRAME);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuYesNo::Destroy (void)
{   
    // Uninitialize the font
    m_Font.Destroy ();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EGameMode CMenuYesNo::Update (EGameMode CurrentGameMode)
{
    // This new variable will be the function's return value 
    // and will indicate the new game mode to set. For the moment,
    // do not ask for a new game mode after this update.
    EGameMode NextGameMode = CurrentGameMode;
    
    // If the menu yes/no can be activated (it can't if the game mode doesn't allow it)
    if (CurrentGameMode != GAMEMODE_CONTROLS && 
        CurrentGameMode != GAMEMODE_DEMO &&
        CurrentGameMode != GAMEMODE_GREETS &&
        CurrentGameMode != GAMEMODE_HELP)
    {
        // If the escape control is active
        if (m_pInput->GetMainInput().TestBreak ())
        {
            // If the menu yes/no is not active
            if (!m_Active)
            {
                // Remember if the sound is paused when we activate 
                // the menu in order to restore it when the menu gets inactive
                m_WasSoundPaused = m_pSound->IsPaused ();

                // Pause the samples and songs being played
                m_pSound->SetPause (true);

                // According to the current game mode, set the title string of the menu yes/no
                switch (CurrentGameMode)
                {
                    case GAMEMODE_MENU :  strcpy (m_Title, "EXIT MENU");  break;
                    case GAMEMODE_TITLE : strcpy (m_Title, "EXIT GAME");  break;
                    default :             strcpy (m_Title, "EXIT MATCH"); break;
                }

                // The menu yes/no is now active, so play a sound on creation
                m_pSound->PlaySample (SAMPLE_BREAK_1);
            }
            // If the menu yes/no is active
            else
            {
                // Restore the sound pause state
                m_pSound->SetPause (m_WasSoundPaused);

                // The menu yes/no is now inactive, so play a sound on destruction
                m_pSound->PlaySample (SAMPLE_BREAK_2);
            }

            // Activate or inactivate the menu yes/no
            m_Active = !m_Active;
        }
    }
    
     // If the menu yes/no is active    
    if (m_Active)
    {
        // If the UP control is active
        if (m_pInput->GetMainInput().TestUp())
        {
            // Switch between the "yes" and "no" answer
            m_YesNo = !m_YesNo;

            // Play the menu beep sound (change of selection)
            m_pSound->PlaySample (SAMPLE_MENU_BEEP);
        }
        // If the DOWN control is active
        else if (m_pInput->GetMainInput().TestDown())
        {
            // Switch between the "yes" and "no" answer
            m_YesNo = !m_YesNo;

            // Play the menu beep sound (change of selection)
            m_pSound->PlaySample (SAMPLE_MENU_BEEP);
        }
        // If the NEXT control is active
        else if (m_pInput->GetMainInput().TestNext())
        {
            // Restore the sound pause state
            m_pSound->SetPause (m_WasSoundPaused);
            
            // The menu yes/no is now inactive
            m_Active = false;
            
            // If the user accepted by choosing "yes"
            if (m_YesNo)
            {
                // According to the current game mode, ask for another game mode
                // since the user made his choice.
                switch (CurrentGameMode)
                {
                    case GAMEMODE_MENU :  NextGameMode = GAMEMODE_TITLE; break;
                    case GAMEMODE_TITLE : NextGameMode = GAMEMODE_EXIT;  break;
                    default:              NextGameMode = GAMEMODE_TITLE;  break;  // NextGameMode = GAMEMODE_MENU
                }
            }

            // Reset cursor position
            m_YesNo = true;
        }
    }

    // Ask for a game mode :
    // - either the same game mode we are in (no change)
    // - or another game mode because the user accepted by choosing "yes"
    return NextGameMode;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuYesNo::Display (void)
{
    // If the menu is active
    if (m_Active)
    {
        // Set position of origin where to draw sprites from
        m_pDisplay->SetOrigin (0, 0);

        // Draw the menu frame sprite
        m_pDisplay->DrawSprite (164, 73, NULL, NULL, BMP_MENU_FRAME_2, 0, 800, -1);

        m_Font.SetTextColor (FONTCOLOR_WHITE);
        m_Font.DrawCenteredX (0, VIEW_WIDTH, 93, m_Title);
    
        m_pDisplay->DrawSprite (INITIAL_TEXT_POSITION_X + CURSOR_HAND_SPACE_X,
                                INITIAL_TEXT_POSITION_Y + (m_YesNo ? 0 : TEXT_SPACE_Y) + CURSOR_HAND_SPACE_Y,
                                NULL,
                                NULL,
                                BMP_MENU_HAND,
                                CURSOR_HAND_SPRITE,
                                801,
                                -1);
    
        m_Font.SetTextColor ((m_YesNo ? FONTCOLOR_YELLOW : FONTCOLOR_WHITE));
        m_Font.Draw (INITIAL_TEXT_POSITION_X, INITIAL_TEXT_POSITION_Y, "YES");
    
        m_Font.SetTextColor ((m_YesNo ? FONTCOLOR_WHITE : FONTCOLOR_YELLOW));
        m_Font.Draw (INITIAL_TEXT_POSITION_X, INITIAL_TEXT_POSITION_Y + TEXT_SPACE_Y, "NO");
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
