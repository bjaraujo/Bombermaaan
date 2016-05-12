/************************************************************************************

    Copyright (C) 2016 Billy Araujo

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
 *  \file CMenuTeam.cpp
 *  \brief The menu where you can choose the team
 */

#include "StdAfx.h"
#include "CMenuTeam.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CFont.h"
#include "CMenu.h"
#include "CSound.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MENUTEAM_SPRITELAYER       1                //!< Sprite layer where to draw sprites
                                                            
#define TITLE_TEXT_POSITION_Y       90      //!< Position Y of the title text that is centered on the X axis

#define INITIAL_TEXT_POSITION_X     191     //!< Initial position of the text "BOMBER"
#define INITIAL_TEXT_POSITION_Y     (77+90)
#define TYPE_TEXT_SPACE_X           75      //!< X Space in pixels between the "BOMBER" text X position and the type's text X position
#define TEXT_SPACE_Y                21      //!< Y Space in pixels between each "BOMBER" text Y position

#define BOMBER_HEAD_SPACE_X         -29     //!< Space in pixels between the "BOMBER" text position
#define BOMBER_HEAD_SPACE_Y         -7      //!< and the corresponding bomber head
#define BOMBER_HEAD_SPRITE_TABLE    30      //!< Sprite table where the menu's bomber head sprites are contained
#define BOMBER_HEAD_PRIORITY        0       //!< Priority to use when drawing the menu's bomber head sprites

#define CURSOR_HAND_SPACE_X         -54     //!< Space in pixels between the "BOMBER" text position
#define CURSOR_HAND_SPACE_Y         -2      //!< and the cursor hand pointing to the corresponding bomber head
#define CURSOR_HAND_SPRITE_TABLE    32      //!< Sprite table where the menu's cursor hand sprites are contained
#define CURSOR_HAND_SPRITE          0       //!< Sprite number of the cursor hand in the sprite table
#define CURSOR_HAND_PRIORITY        0       //!< Priority to use when drawing the menu's bomber hand sprites

#define TEAM_VS_TEXT_POSITION_Y     140     //!< Position Y of the vs text that is centered on the X axis

#define BOMBER_TEAM_1_COLX			0       //!< Column Team A
#define BOMBER_TEAM_2_COLX			140     //!< Column Team B

#define TITLE_STRING                "TEAM"         //!< String of the menu's title centered on the X axis
#define TEAM_VS_STRING              "VS"           //!< String of a menu item centered on the X axis

#define BLINKING_TIME                           0.100f      //!< Time (in seconds) the bomber head has to spend blinking
#define NOT_BLINKING_MINIMUM_TIME               3.0f        //!< Minimum time (in seconds) the bomber head has to spend without blinking
#define NOT_BLINKING_MAXIMUM_ADDITIONAL_TIME    5000        //!< Maximum additional time (Caution : IN MILLISECONDS)

        
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuTeam::CMenuTeam (void) : CMenuBase ()
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuTeam::~CMenuTeam (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuTeam::OnCreate (void)
{
    // Make the hand cursor point to the first option
    m_CursorPlayer = 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuTeam::OnDestroy (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuTeam::OnUp (void)
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

void CMenuTeam::OnDown(void)
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

void CMenuTeam::OnLeft(void)
{
    
	if (m_pOptions->GetBomberTeam(m_CursorPlayer) == BOMBERTEAM_2)
		m_pOptions->SetBomberTeam(m_CursorPlayer, BOMBERTEAM_1);

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuTeam::OnRight(void)
{
    
	if (m_pOptions->GetBomberTeam(m_CursorPlayer) == BOMBERTEAM_1)
		m_pOptions->SetBomberTeam(m_CursorPlayer, BOMBERTEAM_2);

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuTeam::OnPrevious(void)
{
    // Go to the previous screen
    Exit (MENUACTION_PREVIOUS);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuTeam::OnNext(void)
{
    
    // Play the menu next sound
    m_pSound->PlaySample (SAMPLE_MENU_NEXT);

    // Go to the next screen
    Exit (MENUACTION_NEXT);

 }

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuTeam::OnUpdate(void)
{ 

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuTeam::OnDisplay (void)
{
    // Set the right font text color and write the menu title string
    m_pFont->SetTextColor (FONTCOLOR_WHITE);
    m_pFont->DrawCenteredX (0, VIEW_WIDTH - 1, TITLE_TEXT_POSITION_Y, TITLE_STRING); 

	m_pFont->SetTextColor(FONTCOLOR_GREEN);
	m_pFont->DrawCenteredX(0, VIEW_WIDTH - 1, TEAM_VS_TEXT_POSITION_Y, TEAM_VS_STRING);

    // Y Position where to write the text with the font object
    int PositionY = INITIAL_TEXT_POSITION_Y;

    // Scan the players
    for (int Player = 0 ; Player < MAX_PLAYERS ; Player++)
    {

		// TODO:
		
		int PositionX = 0;

		if (m_pOptions->GetBomberTeam(Player) == BOMBERTEAM_1)
			PositionX = BOMBER_TEAM_1_COLX;
		else if (m_pOptions->GetBomberTeam(Player) == BOMBERTEAM_2)
			PositionX = BOMBER_TEAM_2_COLX;

		// Draw the bomber head corresponding to the current player
		m_pDisplay->DrawSprite(INITIAL_TEXT_POSITION_X + BOMBER_HEAD_SPACE_X + PositionX,
								PositionY + BOMBER_HEAD_SPACE_Y,
								NULL,
								NULL,
								BOMBER_HEAD_SPRITE_TABLE,
								Player, // Blinking bomber head sprite or not
								MENUTEAM_SPRITELAYER,
								BOMBER_HEAD_PRIORITY);
		
        // If the cursor hand is pointing to the current player
        if (m_CursorPlayer == Player)
        {
            // Draw the cursor hand sprite in front of the corresponding bomber head
            m_pDisplay->DrawSprite (INITIAL_TEXT_POSITION_X + CURSOR_HAND_SPACE_X,
                                    PositionY + CURSOR_HAND_SPACE_Y,
                                    NULL,
                                    NULL,
                                    CURSOR_HAND_SPRITE_TABLE,
                                    CURSOR_HAND_SPRITE,
                                    MENUTEAM_SPRITELAYER,
                                    CURSOR_HAND_PRIORITY);
        }

        // Go down
        PositionY += TEXT_SPACE_Y;
    }

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
