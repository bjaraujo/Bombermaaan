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
 *  \file CMenuMatch.cpp
 *  \brief The menu where you can set match options
 */

#include "StdAfx.h"
#include "CMenuMatch.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CFont.h"
#include "CMenu.h"
#include "CSound.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MENUMATCH_SPRITELAYER       1               //!< Sprite layer where to draw sprites
                                                    
#define TITLE_TEXT_POSITION_Y       90              //!< Position Y of the title text that is centered on the X axis
                                                    
#define INITIAL_TEXT_POSITION_X     162             //!< Initial position of the text "BATTLE"
#define INITIAL_TEXT_POSITION_Y     (77+90)
#define TYPE_TEXT_SPACE_X           75              //!< X Space in pixels between the "BOMBER" text X position and the type's text X position
#define TEXT_SPACE_Y                21              //!< Y Space in pixels between each "BOMBER" text Y position
                                                    
#define CURSOR_HAND_SPACE_X         -25             //!< Space in pixels between the "BOMBER" text position
#define CURSOR_HAND_SPACE_Y         -2              //!< and the cursor hand pointing to the corresponding bomber head
#define CURSOR_HAND_SPRITE          0               //!< Sprite number of the cursor hand in the sprite table
#define CURSOR_HAND_PRIORITY        0               //!< Priority to use when drawing the menu's bomber hand sprites
                                                    
#define TITLE_STRING                "MATCH"         //!< String of the menu's title centered on the X axis
#define BATTLE_STRING               "BATTLE"        //!< String of a menu item centered on the X axis
#define MODE_STRING                 "MODE"          //!< String of a menu item centered on the X axis
#define TIMESTART_STRING            "START"         //!< String of a menu item centered on the X axis
#define TIMEUP_STRING               "HURRY"         //!< String of a menu item centered on the X axis

#define TEAM_MODE_STRING             "TEAM"         //!< String option - team
#define SINGLE_MODE_STRING           "SINGLE"       //!< String option - single

#define VALUE_TEXT_SPACE_X          95              //!< Horizontal space between the menu item name position and its value position
        
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuMatch::CMenuMatch (void) : CMenuBase ()
{
    m_Options[0] = OPTION_MODE;
    m_Options[1] = OPTION_BATTLE;
    m_Options[2] = OPTION_TIME;
    m_Options[3] = OPTION_TIMEUP;
    
    m_CursorOption = 0;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenuMatch::~CMenuMatch (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnCreate (void)
{
    // Make the hand cursor point to the first option
    m_CursorOption = 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnDestroy (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnUp (void)
{
    // Make the cursor go up
    m_CursorOption--;

    // If it is now out of bounds
    if (m_CursorOption < 0)
    {
        // Wrap : make the cursor point to the last option
        m_CursorOption = MENUMATCH_OPTIONS_COUNT - 1;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnDown (void)
{
    // Make the cursor go down
    m_CursorOption++;

    // If it is now out of bounds
    if (m_CursorOption > MENUMATCH_OPTIONS_COUNT - 1)
    {
        // Wrap : make the cursor point to the first option
        m_CursorOption = 0;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnLeft (void)
{
    // Modify the value of the option field that is currently pointed by the cursor hand
    switch (m_Options[m_CursorOption])
    {

        case OPTION_MODE:
        {

            if (m_pOptions->GetBattleMode() == BATTLEMODE_TEAM)
                m_pOptions->SetBattleMode(BATTLEMODE_SINGLE);
            else if (m_pOptions->GetBattleMode() == BATTLEMODE_SINGLE)
                m_pOptions->SetBattleMode(BATTLEMODE_TEAM);

            break;
        }

        case OPTION_BATTLE :
        {
            // If the current number of battles is the minimum
            if (m_pOptions->GetBattleCount() == 1)
            {
                // Wrap : set the maximum number of battles
                m_pOptions->SetBattleCount(5);
            }
            // If the current number of battles is not the minimum
            else
            {
                // Decrease the number of battles
                m_pOptions->SetBattleCount(m_pOptions->GetBattleCount() - 1);
            }

            break;
        }

        case OPTION_TIME:
        {
            // If there are no time start seconds left and we can decrease the time start
            if (m_pOptions->GetTimeStartSeconds() == 0 && m_pOptions->GetTimeStartMinutes() > 0)
            {
                // Decrease the time start minutes and set the time start seconds to 55
                m_pOptions->SetTimeStart(m_pOptions->GetTimeStartMinutes() - 1, 55); 
            }
            // If there are enough seconds
            else if (m_pOptions->GetTimeStartSeconds() > 0)
            {
                // Just decrease the number of seconds
                m_pOptions->SetTimeStart(m_pOptions->GetTimeStartMinutes(), m_pOptions->GetTimeStartSeconds() - 5); 
            }

            break;
        }

        case OPTION_TIMEUP:
        {
            // If there are no time up seconds left and we can decrease the time up
            if (m_pOptions->GetTimeUpSeconds() == 0 && m_pOptions->GetTimeUpMinutes() > 0)
            {
                // Decrease the time up minutes and set the time up seconds to 55
                m_pOptions->SetTimeUp(m_pOptions->GetTimeUpMinutes() - 1, 55); 
            }
            // If there are enough seconds
            else if (m_pOptions->GetTimeUpSeconds() > 0)
            {
                // Just decrease the number of seconds
                m_pOptions->SetTimeUp(m_pOptions->GetTimeUpMinutes(), m_pOptions->GetTimeUpSeconds() - 5); 
            }

            break;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnRight (void)
{
    // Modify the value of the option field that is currently pointed by the cursor hand
    switch (m_Options[m_CursorOption])
    {

        case OPTION_MODE:
        {

            if (m_pOptions->GetBattleMode() == BATTLEMODE_TEAM)
                m_pOptions->SetBattleMode(BATTLEMODE_SINGLE);
            else if (m_pOptions->GetBattleMode() == BATTLEMODE_SINGLE)
                m_pOptions->SetBattleMode(BATTLEMODE_TEAM);

            break;

        }

        case OPTION_BATTLE :
        {
            // If the current number of battles is the maximum
            if (m_pOptions->GetBattleCount() == 5)
            {
                // Wrap : set the minimum number of battles
                m_pOptions->SetBattleCount (1);
            }
            // If the current number of battles is not the maximum
            else
            {
                // Increase the number of battles
                m_pOptions->SetBattleCount (m_pOptions->GetBattleCount() + 1);
            }

            break;
        }

        case OPTION_TIME:
        {
            // If we are about to add a minute
            if (m_pOptions->GetTimeStartSeconds() == 55)
            {
                // Add a minute
                if (m_pOptions->GetTimeStartMinutes() < 9)
                    m_pOptions->SetTimeStart(m_pOptions->GetTimeStartMinutes() + 1, 0);
            }
            // If we are not about to add a minute
            else
            {
                // Add seconds
                m_pOptions->SetTimeStart(m_pOptions->GetTimeStartMinutes(), m_pOptions->GetTimeStartSeconds() + 5);
            }

            break;
        }

        case OPTION_TIMEUP:
        {
            // If we are about to add a minute
            if (m_pOptions->GetTimeUpSeconds() == 55)
            {
                // Add a minute
                m_pOptions->SetTimeUp(m_pOptions->GetTimeUpMinutes() + 1, 0); 
            }
            // If we are not about to add a minute
            else
            {
                // Add seconds
                m_pOptions->SetTimeUp(m_pOptions->GetTimeUpMinutes(), m_pOptions->GetTimeUpSeconds() + 5); 
            }

            break;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnPrevious (void)
{
    // Go to the previous screen
    Exit (MENUACTION_PREVIOUS);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnNext (void)
{
    // If the time up is lower (or equal) than the time start
    if (m_pOptions->GetTimeUpMinutes() < m_pOptions->GetTimeStartMinutes() ||
        (m_pOptions->GetTimeUpMinutes() == m_pOptions->GetTimeStartMinutes() &&
         m_pOptions->GetTimeUpSeconds() <=m_pOptions->GetTimeStartSeconds()))
    {
        // Play the menu next sound
        m_pSound->PlaySample (SAMPLE_MENU_NEXT);

        // Go to the next screen
        Exit (MENUACTION_NEXT);
    }
    else
    {
        // Play the menu next sound
        m_pSound->PlaySample (SAMPLE_MENU_ERROR);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnUpdate (void)
{ 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenuMatch::OnDisplay (void)
{
    // Set the right font text color and write the menu title string
    m_pFont->SetTextColor (FONTCOLOR_WHITE);
    m_pFont->DrawCenteredX (0, VIEW_WIDTH - 1, TITLE_TEXT_POSITION_Y, TITLE_STRING); 

    // Y Position where to write the text with the font object
    int PositionY = INITIAL_TEXT_POSITION_Y;

    // Scan the options
    for (int Option = 0 ; Option < MENUMATCH_OPTIONS_COUNT ; Option++)
    {
        switch (m_Options[Option])
        {

            case OPTION_MODE:
            {
                // Set the right font text color and write the text for the name of current field
                m_pFont->SetTextColor(FONTCOLOR_GREEN);
                m_pFont->Draw(INITIAL_TEXT_POSITION_X, PositionY, MODE_STRING);

                // Set the right font text color and write the value of the field
                m_pFont->SetTextColor(FONTCOLOR_YELLOW);
                m_pFont->Draw(INITIAL_TEXT_POSITION_X + VALUE_TEXT_SPACE_X,
                    PositionY,
                    "%s",
                    m_pOptions->GetBattleMode() == BATTLEMODE_TEAM ? TEAM_MODE_STRING : SINGLE_MODE_STRING);
                break;
            }

            case OPTION_BATTLE :
            {
                // Set the right font text color and write the text for the name of current field
                m_pFont->SetTextColor (FONTCOLOR_GREEN);
                m_pFont->Draw (INITIAL_TEXT_POSITION_X, PositionY, BATTLE_STRING); 
                
                // Set the right font text color and write the value of the field
                m_pFont->SetTextColor (FONTCOLOR_YELLOW);
                m_pFont->Draw (INITIAL_TEXT_POSITION_X + VALUE_TEXT_SPACE_X, 
                               PositionY, 
                               "%d", 
                               m_pOptions->GetBattleCount());
                break;
            }

            case OPTION_TIME :
            {
                // Set the right font text color and write the text for the name of current field
                m_pFont->SetTextColor (FONTCOLOR_GREEN);
                m_pFont->Draw (INITIAL_TEXT_POSITION_X, PositionY, TIMESTART_STRING); 
                
                // Set the right font text color and write the value of the field
                m_pFont->SetTextColor (FONTCOLOR_YELLOW);
                
                // If there is no time start
                if (m_pOptions->GetTimeStartMinutes() == 0 && m_pOptions->GetTimeStartSeconds() == 0)
                {
                    // Display no time start
                    m_pFont->Draw (INITIAL_TEXT_POSITION_X + VALUE_TEXT_SPACE_X, PositionY, "-:--");
                }
                // If there is a time start
                else
                {
                    // Display the time start
                    m_pFont->Draw (INITIAL_TEXT_POSITION_X + VALUE_TEXT_SPACE_X, 
                                   PositionY, 
                                   "%d:%02d", 
                                   m_pOptions->GetTimeStartMinutes(),
                                   m_pOptions->GetTimeStartSeconds());
                }
                break;
            }

            case OPTION_TIMEUP :
            {
                // Set the right font text color and write the text for the name of current field
                m_pFont->SetTextColor (FONTCOLOR_GREEN);
                m_pFont->Draw (INITIAL_TEXT_POSITION_X, PositionY, TIMEUP_STRING); 
                
                // Set the right font text color and write the value of the field
                m_pFont->SetTextColor (FONTCOLOR_YELLOW);
                
                // If there is no time up
                if (m_pOptions->GetTimeUpMinutes() == 0 && m_pOptions->GetTimeUpSeconds() == 0)
                {
                    // Display no time up
                    m_pFont->Draw (INITIAL_TEXT_POSITION_X + VALUE_TEXT_SPACE_X, PositionY, "-:--");
                }
                // If there is a time up
                else
                {
                    // Display the time up
                    m_pFont->Draw (INITIAL_TEXT_POSITION_X + VALUE_TEXT_SPACE_X, 
                                   PositionY, 
                                   "%d:%02d", 
                                   m_pOptions->GetTimeUpMinutes(),
                                   m_pOptions->GetTimeUpSeconds());
                }

                break;
            }
        }
        
        // If the cursor hand is pointing to the current option
        if (m_CursorOption == Option)
        {
            // Draw the cursor hand sprite in front of the corresponding option text
            m_pDisplay->DrawSprite (INITIAL_TEXT_POSITION_X + CURSOR_HAND_SPACE_X,
                                    PositionY + CURSOR_HAND_SPACE_Y,
                                    NULL,
                                    NULL,
                                    BMP_MENU_HAND,
                                    CURSOR_HAND_SPRITE,
                                    MENUMATCH_SPRITELAYER,
                                    CURSOR_HAND_PRIORITY);
        }
        
        // Go down
        PositionY += TEXT_SPACE_Y;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
