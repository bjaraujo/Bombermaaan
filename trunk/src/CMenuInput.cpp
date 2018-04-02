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
 *  \file CMenuInput.cpp
 *  \brief Handling inputs during the menu
 */

#include "StdAfx.h"
#include "CMenuInput.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CFont.h"
#include "CMenu.h"
#include "CSound.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MENUINPUT_SPRITELAYER       1       //!< Sprite layer where to draw sprites

#define TITLE_TEXT_POSITION_Y       90      //!< Position Y of the title text that is centered on the X axis
#define INITIAL_TEXT_POSITION_X     191     //!< Initial position of the player input's name
#define INITIAL_TEXT_POSITION_Y     (77+90)

#define TEXT_SPACE_Y                21      //!< Y Space in pixels between each text Y position

#define BOMBER_HEAD_SPACE_X         -29     //!< Space in pixels between the player input name text position
#define BOMBER_HEAD_SPACE_Y         -7      //!< and the corresponding bomber head
#define BOMBER_HEAD_PRIORITY        0       
#define BOMBER_HEAD_PRIORITY        0       //!< Priority to use when drawing the menu's bomber head sprites

#define CURSOR_HAND_SPACE_X         -54     //!< Space in pixels between the player input name text position
#define CURSOR_HAND_SPACE_Y         -2      //!< and the cursor hand pointing to the corresponding bomber head
#define CURSOR_HAND_SPRITE          0       //!< Sprite number of the cursor hand in the sprite table
#define CURSOR_HAND_PRIORITY        0       //!< Priority to use when drawing the menu's bomber hand sprites

#define TITLE_STRING                "INPUT" //!< String of the menu's title centered on the X axis

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuInput::CMenuInput (void) : CMenuBase ()
{
    
    m_CursorPlayer = 0;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuInput::~CMenuInput (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnCreate (void)
{
    // Make the hand cursor point to the first player
    m_CursorPlayer = 0;

    // While the cursor is not pointing on a human player
    while (m_pOptions->GetBomberType(m_CursorPlayer) != BOMBERTYPE_MAN)
    {
        // Make the cursor go down
        m_CursorPlayer++;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnDestroy (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnUp (void)
{
    do
    {
        // Make the cursor go up
        m_CursorPlayer--;

        // If it is now out of bounds
        if (m_CursorPlayer < 0)
        {
            // Wrap : make the cursor point to the last player
            m_CursorPlayer = MAX_PLAYERS - 1;
        }
    }
    // While the cursor is not pointing on a human player
    while (m_pOptions->GetBomberType(m_CursorPlayer) != BOMBERTYPE_MAN);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnDown (void)
{
    do
    {
        // Make the cursor go down
        m_CursorPlayer++;

        // If it is now out of bounds
        if (m_CursorPlayer > MAX_PLAYERS - 1)
        {
            // Wrap : make the cursor point to the first player
            m_CursorPlayer = 0;
        }
    }
    // While the cursor is pointing on a human player
    while (m_pOptions->GetBomberType(m_CursorPlayer) != BOMBERTYPE_MAN);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnLeft (void)
{
    // If the current player's current player input is the first one
    if (m_pOptions->GetPlayerInput(m_CursorPlayer) - 1 < 0)
    {
        // Set the last player input for the current player
        m_pOptions->SetPlayerInput (m_CursorPlayer, m_pInput->GetPlayerInputCount() - 1);
    }
    // If the current player's current player input is not the first one
    else
    {
        // Set the previous player input for the current player
        m_pOptions->SetPlayerInput (m_CursorPlayer, m_pOptions->GetPlayerInput(m_CursorPlayer) - 1);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnRight (void)
{
    // If the current player's current player input is the last one
    if (m_pOptions->GetPlayerInput(m_CursorPlayer) + 1 >= m_pInput->GetPlayerInputCount())
    {
        // Set the first player input for the current player
        m_pOptions->SetPlayerInput (m_CursorPlayer, 0);
    }
    // If the current player's current player input is not the last one
    else
    {
        // Set the next player input for the current player
        m_pOptions->SetPlayerInput (m_CursorPlayer, m_pOptions->GetPlayerInput(m_CursorPlayer) + 1);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnPrevious (void)
{
    // Go to the previous screen
    Exit (MENUACTION_PREVIOUS);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnNext (void)
{
    // Play the menu next sound
    m_pSound->PlaySample (SAMPLE_MENU_NEXT);
    
    // Go to the next screen
    Exit (MENUACTION_NEXT);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnUpdate (void)
{ 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuInput::OnDisplay (void)
{
    // Set the right font text color and write the menu title string
    m_pFont->SetTextColor (FONTCOLOR_WHITE);
    m_pFont->DrawCenteredX (0, VIEW_WIDTH - 1, TITLE_TEXT_POSITION_Y, TITLE_STRING); 

    // Y Position where to write the text with the font object
    int PositionY = INITIAL_TEXT_POSITION_Y;

    // Scan the players
    for (int Player = 0 ; Player < MAX_PLAYERS ; Player++)
    {
        // If the current player is a human
        if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN)
        {
            int PlayerInput = m_pOptions->GetPlayerInput(Player);
            
            // Set the right font text color and write the current player input name
            m_pFont->SetTextColor (FONTCOLOR_GREEN);
            m_pFont->Draw (INITIAL_TEXT_POSITION_X, 
                           PositionY, 
                           m_pInput->GetPlayerInput(PlayerInput).GetName()); 

            // Draw the bomber head corresponding to the current player
            m_pDisplay->DrawSprite (INITIAL_TEXT_POSITION_X + BOMBER_HEAD_SPACE_X,
                                    PositionY + BOMBER_HEAD_SPACE_Y,
                                    NULL,
                                    NULL,
                                    BMP_MENU_BOMBER,
                                    Player,
                                    MENUINPUT_SPRITELAYER,
                                    BOMBER_HEAD_PRIORITY);

            // If the cursor hand is pointing to the current player
            if (m_CursorPlayer == Player)
            {
                // Draw the cursor hand sprite in front of the corresponding bomber head
                m_pDisplay->DrawSprite (INITIAL_TEXT_POSITION_X + CURSOR_HAND_SPACE_X,
                                        PositionY + CURSOR_HAND_SPACE_Y,
                                        NULL,
                                        NULL,
                                        BMP_MENU_HAND,
                                        CURSOR_HAND_SPRITE,
                                        MENUINPUT_SPRITELAYER,
                                        CURSOR_HAND_PRIORITY);
            }

            // Go down
            PositionY += TEXT_SPACE_Y;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
