// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold

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
 *  \file CTitle.cpp
 *  \brief Title screen (main screen of Bombermaaan at program start)
 */

#include "StdAfx.h"
#include "CTitle.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CGame.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define BLACKSCREEN_DURATION                    0.350f  // Duration (in seconds) of each of the two black screens
                                                
#define DISPLAY_ORIGIN_X                        0
#define DISPLAY_ORIGIN_Y                        0
                                                
#define BACKGROUND_POSITION_X                   0
#define BACKGROUND_POSITION_Y                   0
#define BACKGROUND_SPRITE                       0
#define BACKGROUND_SPRITE_LAYER                 0
#define BACKGROUND_SPRITE_PRIORITY_IN_LAYER     0

#define BOMBERS_POSITION_X                      0
#define BOMBERS_POSITION_Y                      (VIEW_HEIGHT - 126)
#define BOMBERS_SPRITE                          0
#define BOMBERS_SPRITE_LAYER                    0
#define BOMBERS_SPRITE_PRIORITY_IN_LAYER        2

#define TITLE_POSITION_X                        7
#define TITLE_POSITION_Y                        7
#define TITLE_SPRITE                            0
#define TITLE_SPRITE_LAYER                      0
#define TITLE_SPRITE_PRIORITY_IN_LAYER          2

#define NUMBER_OF_MENU_ITEMS                    6
#define FIRST_MENU_ITEM                         0
#define LAST_MENU_ITEM                          (NUMBER_OF_MENU_ITEMS - 1)
#define FIRST_MENU_ITEM_POSITION_Y              180
#define ALL_MENU_ITEMS_POSITION_X               195
#define MENU_ITEM_SPRITE_LAYER                  0
#define MENU_ITEM_SPRITE_PRIORITY_IN_LAYER      2
#define SPACE_X_FROM_MENU_ITEM_TO_CURSOR_HAND   -40
#define CURSOR_HAND_SPRITE                      0
#define CURSOR_HAND_SPRITE_LAYER                0
#define CURSOR_HAND_SPRITE_PRIORITY_IN_LAYER    2
#define SPACE_Y_BETWEEN_MENU_ITEMS              30

// Definitions for the animated hand
#define MOVING_HAND_TIMEPERIOD                  0.06f   //!< Time period for calculating the moving hand position
#define MOVING_HAND_DISTANCE_1                  (-20)
#define MOVING_HAND_DISTANCE_2                  (-18)
#define MOVING_HAND_DISTANCE_3                  (-13)
#define MOVING_HAND_DISTANCE_4                  (-6)
#define MOVING_HAND_DISTANCE_5                  (-2)
#define MOVING_HAND_DISTANCE_6                  (0)

#define MENU_ITEM_GAME                          0
#define MENU_ITEM_DEMO                          1
#define MENU_ITEM_OPTIONS                       2
#define MENU_ITEM_CREDITS                       3
#define MENU_ITEM_HELP                          4
#define MENU_ITEM_EXIT                          5

#define MAX_IDLE_TIME                            30.0f   // Duration (in seconds) of maximum idle time

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CTitle::CTitle (void) : CModeScreen()
{
    // The cursor points to the first menu item
    m_Cursor = FIRST_MENU_ITEM;
    
    m_SongStarted = false;
    m_ModeTime = 0.0f;
    m_HaveToExit = false;
    m_MovingHandDistance = 0;
    m_MovingHandTimer = 0.0f;    
    
    m_ExitModeTime = 0.0f;
    m_ExitGameMode = GAMEMODE_NONE;

    m_IdleTime = 0.0f;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CTitle::~CTitle (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CTitle::Create (void)
{
    CModeScreen::Create();
    
    // We didn't start playing the song yet
    m_SongStarted = false;

    // Reset mode time (no time has been elapsed in this mode yet)
    m_ModeTime = 0.0f;

    // Don't have to exit this mode yet
    m_HaveToExit = false;

    // Initialize the moving hand
    m_MovingHandDistance = 0;
    m_MovingHandTimer = 0.0f;

    // Don't initialize the cursor to the first item.
    // This allows keeping the last cursor position
    // when switching to another screen and then coming
    // back to the title screen.

    // Create the clouds in the sky
    m_CloudManager.Create();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CTitle::Destroy (void)
{
    // Destroy the clouds in the sky
    m_CloudManager.Destroy();

    CModeScreen::Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CTitle::OpenInput (void)
{
    m_pInput->GetMainInput().Open();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CTitle::CloseInput (void)
{
    m_pInput->GetMainInput().Close();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EGameMode CTitle::Update (void) 
{
    // Increase elapsed time since mode has started
    m_ModeTime += m_pTimer->GetDeltaTime();
    m_IdleTime += m_pTimer->GetDeltaTime();

    // If we have to make the first black screen
    if (m_ModeTime <= BLACKSCREEN_DURATION)
    {

    }
    // If the title screen should be active
    else if (!m_HaveToExit)
    {
        // If we didn't start playing the song yet
        if (!m_SongStarted)
        {
            // Start playing the title song
            m_pSound->PlaySong (SONG_TITLE_MUSIC);

            // We started playing the song
            m_SongStarted = true;
        }

        // Update the clouds in the sky
        m_CloudManager.Update (m_pTimer->GetDeltaTime());
    
        // Update the moving hand
        m_MovingHandTimer += m_pTimer->GetDeltaTime();
               if ( m_MovingHandTimer < MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_1;
        } else if ( m_MovingHandTimer < 2 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_2;
        } else if ( m_MovingHandTimer < 3 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_3;
        } else if ( m_MovingHandTimer < 4 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_4;
        } else if ( m_MovingHandTimer < 5 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_5;
        } else if ( m_MovingHandTimer < 6 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_6;
        } else if ( m_MovingHandTimer < 7 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_5;
        } else if ( m_MovingHandTimer < 8 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_4;
        } else if ( m_MovingHandTimer < 9 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_3;
        } else if ( m_MovingHandTimer < 10 * MOVING_HAND_TIMEPERIOD ) {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_2;
        } else {
            m_MovingHandDistance = MOVING_HAND_DISTANCE_1;
            m_MovingHandTimer = 0.0f;
        }

        // If the NEXT control is pressed
        if (m_pInput->GetMainInput().TestNext())
        {

            m_IdleTime = 0.0f;
            
            // Which menu item is the cursor pointing to?
            // Determine the game mode to ask for when exiting
            switch (m_Cursor)
            {
                case MENU_ITEM_GAME    : m_ExitGameMode = GAMEMODE_MENU;     break;
                case MENU_ITEM_DEMO    : m_ExitGameMode = GAMEMODE_DEMO;     break;
                case MENU_ITEM_OPTIONS : m_ExitGameMode = GAMEMODE_CONTROLS; break;
                case MENU_ITEM_CREDITS : m_ExitGameMode = GAMEMODE_GREETS;   break;
                case MENU_ITEM_HELP    : m_ExitGameMode = GAMEMODE_HELP;     break;
                case MENU_ITEM_EXIT    : m_ExitGameMode = GAMEMODE_EXIT;     break;
            }

            // If the game mode should change
            if (m_ExitGameMode != GAMEMODE_TITLE)
            {
                // Stop playing the song
                m_pSound->StopSong (SONG_TITLE_MUSIC);
            
                // We have to exit this game mode
                m_HaveToExit = true;

                // Remember when we decided to exit (needed for the blackscreen)
                m_ExitModeTime = m_ModeTime;

                // Play the menu next sound
                m_pSound->PlaySample (SAMPLE_MENU_NEXT);
            }
            // If there should not be any change of the game mode
            else
            {
                // Play the menu error sound
                m_pSound->PlaySample (SAMPLE_MENU_ERROR);
            }
        }
        else if (m_pInput->GetMainInput().TestUp())
        {

            m_IdleTime = 0.0f;

            // Play the menu beep sound
            m_pSound->PlaySample (SAMPLE_MENU_BEEP);
        
            // Make the cursor go up (menu item above)
            m_Cursor--;

            // If the cursor is now out of bounds
            if (m_Cursor < FIRST_MENU_ITEM)
            {
                // Wrap : make the cursor point to the last menu item
                m_Cursor = LAST_MENU_ITEM;
            }
        }
        else if (m_pInput->GetMainInput().TestDown())
        {

            m_IdleTime = 0.0f;

            // Play the menu beep sound
            m_pSound->PlaySample (SAMPLE_MENU_BEEP);

            // Make the cursor go down (menu item below)
            m_Cursor++;

            // If the cursor is now out of bounds
            if (m_Cursor > LAST_MENU_ITEM)
            {
                // Wrap : make the cursor point to the first menu item
                m_Cursor = FIRST_MENU_ITEM;
            }
        }

        // If idle too long go to Demo mode automatically
        if (m_IdleTime >= MAX_IDLE_TIME)
        {

            m_IdleTime = 0.0f;

            m_ExitGameMode = GAMEMODE_DEMO;

            // Stop playing the song
            m_pSound->StopSong(SONG_TITLE_MUSIC);

            // We have to exit this game mode
            m_HaveToExit = true;

            // Remember when we decided to exit (needed for the blackscreen)
            m_ExitModeTime = m_ModeTime;

        }

    }
    // If we have to make the last black screen
    else if (m_ModeTime <= m_ExitModeTime + BLACKSCREEN_DURATION)
    {

    }
    // If the last black screen is over then ask for another game mode
    else
    {
        // Ask for the new game mode
        return m_ExitGameMode;
    }

    // Stay in this game mode
    return GAMEMODE_TITLE; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CTitle::Display (void)
{
    // If we have to make the first black screen
    if (m_ModeTime <= BLACKSCREEN_DURATION)
    {
    }
    // If the title screen should be active
    else if (!m_HaveToExit)
    {
        // Set the position from which to display sprites
        m_pDisplay->SetOrigin (DISPLAY_ORIGIN_X, DISPLAY_ORIGIN_Y);

        // Display the clouds in the sky
        m_CloudManager.Display();

        // Draw the background sprite
        m_pDisplay->DrawSprite (BACKGROUND_POSITION_X,
                                BACKGROUND_POSITION_Y,
                                NULL,
                                NULL,
                                BMP_TITLE_BACKGROUND,
                                BACKGROUND_SPRITE,
                                BACKGROUND_SPRITE_LAYER,
                                BACKGROUND_SPRITE_PRIORITY_IN_LAYER);

        // Draw the bombers
        m_pDisplay->DrawSprite (BOMBERS_POSITION_X,
                                BOMBERS_POSITION_Y,
                                NULL,
                                NULL,
                                BMP_TITLE_BOMBERS,
                                BOMBERS_SPRITE,
                                BOMBERS_SPRITE_LAYER,
                                BOMBERS_SPRITE_PRIORITY_IN_LAYER);

        // Draw the title of the game
        m_pDisplay->DrawSprite (TITLE_POSITION_X,
                                TITLE_POSITION_Y,
                                NULL,
                                NULL,
                                BMP_TITLE_TITLE,
                                TITLE_SPRITE,
                                TITLE_SPRITE_LAYER,
                                TITLE_SPRITE_PRIORITY_IN_LAYER);

        // Position Y on the screen of the first menu item in this screen
        int MenuItemPositionY = FIRST_MENU_ITEM_POSITION_Y;

        // Display each menu item of the screen
        for (int MenuItemIndex = 0 ; MenuItemIndex < NUMBER_OF_MENU_ITEMS ; MenuItemIndex++)
        {
            // Display the menu item
            m_pDisplay->DrawSprite (ALL_MENU_ITEMS_POSITION_X,
                                    MenuItemPositionY,
                                    NULL,
                                    NULL,
                                    BMP_TITLE_MENU_ITEMS,
                                    MenuItemIndex * 2 + (m_Cursor == MenuItemIndex ? 1 : 0), // Sprite of the menu item + color
                                    MENU_ITEM_SPRITE_LAYER,
                                    MENU_ITEM_SPRITE_PRIORITY_IN_LAYER);

            // If the cursor hand is pointing to the current menu item
            if (m_Cursor == MenuItemIndex)
            {
                // Display the cursor hand
                m_pDisplay->DrawSprite (ALL_MENU_ITEMS_POSITION_X + SPACE_X_FROM_MENU_ITEM_TO_CURSOR_HAND + m_MovingHandDistance,
                                        MenuItemPositionY,
                                        NULL,
                                        NULL,
                                        BMP_MENU_HAND_TITLE,
                                        CURSOR_HAND_SPRITE,
                                        CURSOR_HAND_SPRITE_LAYER,
                                        CURSOR_HAND_SPRITE_PRIORITY_IN_LAYER);
            }

            // Go down : next menu item to display
            MenuItemPositionY += SPACE_Y_BETWEEN_MENU_ITEMS;
        }
    }
    // If the pause is over and we have to make the last black screen
    else if (m_ModeTime <= m_ExitModeTime + BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
