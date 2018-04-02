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
 *  \file CMenuBomber.cpp
 *  \brief The menu where you can choose the bombers
 */

#include "StdAfx.h"
#include "CMenuBomber.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CFont.h"
#include "CMenu.h"
#include "CSound.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MENUBOMBER_SPRITELAYER      1       //!< Sprite layer where to draw sprites

#define TITLE_TEXT_POSITION_Y       90      //!< Position Y of the title text that is centered on the X axis

#define INITIAL_TEXT_POSITION_X     191     //!< Initial position of the text "BOMBER"
#define INITIAL_TEXT_POSITION_Y     (77+90)
#define TYPE_TEXT_SPACE_X           75      //!< X Space in pixels between the "BOMBER" text X position and the type's text X position
#define TEXT_SPACE_Y                21      //!< Y Space in pixels between each "BOMBER" text Y position

#define BOMBER_HEAD_SPACE_X         -29     //!< Space in pixels between the "BOMBER" text position
#define BOMBER_HEAD_SPACE_Y         -7      //!< and the corresponding bomber head
#define BOMBER_HEAD_PRIORITY        0       //!< Priority to use when drawing the menu's bomber head sprites

#define CURSOR_HAND_SPACE_X         -54     //!< Space in pixels between the "BOMBER" text position
#define CURSOR_HAND_SPACE_Y         -2      //!< and the cursor hand pointing to the corresponding bomber head
#define CURSOR_HAND_SPRITE_TABLE    32      //!< Sprite table where the menu's cursor hand sprites are contained
#define CURSOR_HAND_SPRITE          0       //!< Sprite number of the cursor hand in the sprite table
#define CURSOR_HAND_PRIORITY        0       //!< Priority to use when drawing the menu's bomber hand sprites

#define TITLE_STRING                "BOMBER TYPE"   //!< String of the menu's title centered on the X axis
#define BOMBER_STRING               "BOMBER"        //!< String of the text between the bomber head and the bomber type text
#define BOMBERTYPE_OFF_STRING       "OFF"           //!< String for the OFF bomber type
#define BOMBERTYPE_MAN_STRING       "MAN"           //!< String for the MAN bomber type
#define BOMBERTYPE_COM_STRING       "COM"           //!< String for the COM bomber type

#define BLINKING_TIME                           0.100f      //!< Time (in seconds) the bomber head has to spend blinking
#define NOT_BLINKING_MINIMUM_TIME               3.0f        //!< Minimum time (in seconds) the bomber head has to spend without blinking
#define NOT_BLINKING_MAXIMUM_ADDITIONAL_TIME    5000        //!< Maximum additional time (Caution : IN MILLISECONDS)


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuBomber::CMenuBomber (void) : CMenuBase ()
{
    // Initialize the blink values
    for (int i = 0 ; i < MAX_PLAYERS ; i++)
    {
        m_Blinking[i] = false;
        m_BlinkTimer[i] = 0.0f;
    }
    
    m_CursorPlayer = 0;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuBomber::~CMenuBomber (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnCreate (void)
{
    // Make the hand cursor point to the first player
    m_CursorPlayer = 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnDestroy (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnUp (void)
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

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnDown (void)
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

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnLeft (void)
{
    // Set the previous bomber type (and wrap if necessary)
    switch (m_pOptions->GetBomberType(m_CursorPlayer))
    {
        case BOMBERTYPE_OFF : m_pOptions->SetBomberType (m_CursorPlayer, BOMBERTYPE_COM); break;
        case BOMBERTYPE_MAN : m_pOptions->SetBomberType (m_CursorPlayer, BOMBERTYPE_OFF); break;
        case BOMBERTYPE_COM : m_pOptions->SetBomberType (m_CursorPlayer, BOMBERTYPE_MAN); break;
        default:                                                                          break;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnRight (void)
{
    // Set the next bomber type (and wrap if necessary)
    switch (m_pOptions->GetBomberType(m_CursorPlayer))
    {
        case BOMBERTYPE_OFF : m_pOptions->SetBomberType (m_CursorPlayer, BOMBERTYPE_MAN); break;
        case BOMBERTYPE_MAN : m_pOptions->SetBomberType (m_CursorPlayer, BOMBERTYPE_COM); break;
        case BOMBERTYPE_COM : m_pOptions->SetBomberType (m_CursorPlayer, BOMBERTYPE_OFF); break;
        default:                                                                          break;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnPrevious (void)
{
    // Go to the previous menu mode
    Exit (MENUACTION_PREVIOUS);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnNext (void)
{
    // Variables used to count human and computer players
    int ManCount = 0;
    int ComCount = 0;

    // Scan the players
    for (int Player = 0 ; Player < MAX_PLAYERS ; Player++)
    {
        // If this player is a human player
        if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN)
        {
            // Increase the number of human players
            ManCount++;
        }
        // If this player is a computer player
        else if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_COM)
        {
            // Increase the number of computer players
            ComCount++;
        }
    }

    // If there are at least two real players
    if (ManCount + ComCount >= 2)
    {
        // Play the menu next sound
        m_pSound->PlaySample (SAMPLE_MENU_NEXT);
        
        // The choices in this menu are correct, we can now exit this menu mode
        Exit (MENUACTION_NEXT);
    }
    // If there are no enough real players (less than 2)
    else
    {
        // Play the menu error sound
        m_pSound->PlaySample (SAMPLE_MENU_ERROR);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnUpdate (void)
{ 
    // Scan the bomber heads
    for (int i = 0 ; i < MAX_PLAYERS ; i++)
    {
        // Decrease the time left before the blink state for this bomber head changes
        m_BlinkTimer[i] -= m_pTimer->GetDeltaTime();

        // If the blink state for this bomber head has to change
        if (m_BlinkTimer[i] <= 0.0f)
        {
            // Toggle the blink state
            m_Blinking[i] = !m_Blinking[i];

            // Set the blink time left again
            // If the bomber head is blinking
            if (m_Blinking[i])
            {
                // Set a short time
                m_BlinkTimer[i] = BLINKING_TIME;
            }
            // If the bomber head is notblinking
            else
            {
                // Set a long random time
                m_BlinkTimer[i] = NOT_BLINKING_MINIMUM_TIME + (float) RANDOM(NOT_BLINKING_MAXIMUM_ADDITIONAL_TIME) / 1000.0f;
            }
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuBomber::OnDisplay (void)
{
    // Set the right font text color and write the menu title string
    m_pFont->SetTextColor (FONTCOLOR_WHITE);
    m_pFont->DrawCenteredX (0, VIEW_WIDTH - 1, TITLE_TEXT_POSITION_Y, TITLE_STRING); 

    // Y Position where to write the text with the font object
    int PositionY = INITIAL_TEXT_POSITION_Y;

    // Scan the players
    for (int Player = 0 ; Player < MAX_PLAYERS ; Player++)
    {
        // Set the right font text color and write the bomber string
        m_pFont->SetTextColor (FONTCOLOR_GREEN);
        m_pFont->Draw (INITIAL_TEXT_POSITION_X, PositionY, BOMBER_STRING); 

        // Write a different bomber type string according to the 
        // bomber type of the current player in the options object
        switch (m_pOptions->GetBomberType(Player))
        {
            case BOMBERTYPE_OFF :
            {
                // Set the font text color and write the bomber type string
                m_pFont->SetTextColor (FONTCOLOR_RED);
                m_pFont->Draw (INITIAL_TEXT_POSITION_X + TYPE_TEXT_SPACE_X, PositionY, BOMBERTYPE_OFF_STRING);
    
                break;
            }

            case BOMBERTYPE_MAN :
            {
                // Set the font text color and write the bomber type string
                m_pFont->SetTextColor (FONTCOLOR_YELLOW);
                m_pFont->Draw (INITIAL_TEXT_POSITION_X + TYPE_TEXT_SPACE_X, PositionY, BOMBERTYPE_MAN_STRING);

                break;
            }

            case BOMBERTYPE_COM :
            {
                // Set the font text color and write the bomber type string
                m_pFont->SetTextColor (FONTCOLOR_BLUE);
                m_pFont->Draw (INITIAL_TEXT_POSITION_X + TYPE_TEXT_SPACE_X, PositionY, BOMBERTYPE_COM_STRING);

                break;
            }
            default :
                break;
        }

        // Draw the bomber head corresponding to the current player
        m_pDisplay->DrawSprite (INITIAL_TEXT_POSITION_X + BOMBER_HEAD_SPACE_X,
                                PositionY + BOMBER_HEAD_SPACE_Y,
                                NULL,
                                NULL,
                                BMP_MENU_BOMBER,
                                Player + (m_Blinking[Player] ? MAX_PLAYERS : 0), // Blinking bomber head sprite or not
                                MENUBOMBER_SPRITELAYER,
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
                                    MENUBOMBER_SPRITELAYER,
                                    CURSOR_HAND_PRIORITY);
        }

        // Go down
        PositionY += TEXT_SPACE_Y;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
