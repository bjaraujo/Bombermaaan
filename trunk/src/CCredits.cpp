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
 *  \file CCredits.cpp
 *  \brief The control settings screen
 */

#include "StdAfx.h"
#include "CCredits.h"
#include "CGame.h"
#include "COptions.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CMosaic.h"
#include "CRandomMosaic.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define CREDITS_BLACKSCREEN_DURATION           0.750f  //!< Duration (in seconds) of the two black screens
                                                   
#define CREDITS_DISPLAY_ORIGIN_X               0       //!< Display origin of the screen
#define CREDITS_DISPLAY_ORIGIN_Y               0       
                                                        
#define CREDITS_TEXT_SPRITE_LAYER              1       //!< Sprite layer where to draw strings
#define CREDITS_CURSOR_HAND_SPRITE_LAYER       1       //!< Sprite layer where to draw the cursor hand
                                                
#define TIME_TO_WAIT_BEFORE_READING_INPUT       0.200f
                                                
#define NUMBER_OF_MENU_ITEMS                    7
#define FIRST_MENU_ITEM                         0
#define LAST_MENU_ITEM                          (NUMBER_OF_MENU_ITEMS - 1)
                                                
#define DEVICE_MENU_ITEM                        FIRST_MENU_ITEM
#define UP_MENU_ITEM                            1
#define DOWN_MENU_ITEM                          2
#define LEFT_MENU_ITEM                          3
#define RIGHT_MENU_ITEM                         4
#define ACTION1_MENU_ITEM                       5
#define ACTION2_MENU_ITEM                       6

#define SCREEN_TITLE_POSITION_Y                 (20+80)
#define SCREEN_TEXT_POSITION_X                    50

#define SCREEN_CREDITS_TITLE_STRING             "CREDITS"

#define FRAME_POSITION_X            30      //!< Position of the menu frame
#define FRAME_POSITION_Y            52
#define FRAME_SPRITE                0       //!< Sprite number of the menu frame sprite 
#define FRAME_PRIORITY              1       //!< Priority to use in the menu sprite layer when drawing the frame
#define FRAME_SPRITELAYER           0       //!< Sprite layer where to draw the menu frame

#define MOSAIC_SPRITE_LAYER                     0           //!< Sprite layer where to draw the mosaic tiles
#define MOSAIC_SPRITE_PRIORITY_IN_LAYER         0           //!< Priority to use in the sprite layer where to draw the mosaic tiles
#define MOSAIC_SPEED_X                          50.0f       //!< Speed of the mosaic background horizontally
#define MOSAIC_SPEED_Y                          -50.0f      //!< Speed of the mosaic background vertically

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CCredits::CCredits(void) : CModeScreen()
{

    m_HaveToExit = false;
    m_ModeTime = 0.0f;
    m_ExitModeTime = 0.0f;
    m_Cursor = 0;
    m_SongStarted = false;
    m_pMosaic = NULL;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CCredits::~CCredits (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CCredits, you must create it.

void CCredits::Create (void)
{

    CModeScreen::Create();

    // Reset mode time (no time has been elapsed in this mode yet)
    m_ModeTime = 0.0f;

    // Don't have to exit this mode yet
    m_HaveToExit = false;

    // We didn't start playing the song yet
    m_SongStarted = false;

    // Initialize the font
    m_Font.Create ();
    m_Font.SetShadow (false);
    m_Font.SetSpriteLayer (CREDITS_TEXT_SPRITE_LAYER);

    // The cursor points to the first menu item
    m_Cursor = FIRST_MENU_ITEM;

    // Make a random purple mosaic object
    m_pMosaic = CRandomMosaic::CreateRandomMosaic (m_pDisplay, 
                                                   MOSAIC_SPRITE_LAYER, 
                                                   MOSAIC_SPRITE_PRIORITY_IN_LAYER, 
                                                   MOSAIC_SPEED_X, 
                                                   MOSAIC_SPEED_Y, 
                                                   MOSAICCOLOR_RED,
                                                   false);
}
    
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CCredits is not needed anymore, you should destroy it

void CCredits::Destroy (void)
{
    CModeScreen::Destroy();
    
    // Uninitialize the font
    m_Font.Destroy ();

    // Delete the scrolling mosaic background
    m_pMosaic->Destroy();
    delete m_pMosaic;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCredits::OpenInput (void)
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCredits::CloseInput (void)
{

}
                   
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// This updates the controls screen.
// There are 3 parts in this screen :
// - a black screen
// - an update of the winner screen (animations, display...)
//   until an action control is pressed (input is only checked after a minimum duration of the mode)
// - a black screen
// It finally returns the game mode that should be set in the parent CGame object.
// When the screen should continue, it returns GAMEMODE_GREETS to keep this mode.
// When the screen has ended, it returns GAMEMODE_TITLE to start the title screen.

EGameMode CCredits::Update (void)
{
    // Increase elapsed time since mode has started
    m_ModeTime += m_pTimer->GetDeltaTime();

    // If we have to make the first black screen
    if (m_ModeTime <= CREDITS_BLACKSCREEN_DURATION)
    {
        
    }
    // If we don't have to exit yet
    else if (!m_HaveToExit)
    {
        // If we didn't start playing the song yet
        if (!m_SongStarted)
        {
            // Start playing the controls song
            m_pSound->PlaySong(SONG_GREET_MUSIC);

            // We started playing the song
            m_SongStarted = true;
        }

        // Update the scrolling mosaic background
        m_pMosaic->Update (m_pTimer->GetDeltaTime());

        // If the ESCAPE control is active
        if (m_pInput->GetMainInput().TestBreak())
        {
            // Stop playing the song
            m_pSound->StopSong(SONG_GREET_MUSIC);

            // Remember we have to exit this mode
            m_HaveToExit = true;

            // Remember the mode time
            m_ExitModeTime = m_ModeTime;
        }
    }
    // We have to exit, so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= CREDITS_BLACKSCREEN_DURATION)
    {
        
    }
    // Last black screen is complete! Get out of here!
    else
    {
        return GAMEMODE_TITLE;
    }

    // Stay in this game mode
    return GAMEMODE_GREETS;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CCredits::Display (void)
{
    // If we have to make the first black screen
    if (m_ModeTime <= CREDITS_BLACKSCREEN_DURATION)
    {
    }
    // If we don't have to exit yet
    else if (!m_HaveToExit)
    {

        // Set the position from which to display sprites
        m_pDisplay->SetOrigin (CREDITS_DISPLAY_ORIGIN_X, CREDITS_DISPLAY_ORIGIN_Y);

        // Display the scrolling mosaic background
        m_pMosaic->Display ();

        // Draw the menu frame sprite
        m_pDisplay->DrawSprite(FRAME_POSITION_X,
            FRAME_POSITION_Y,
            NULL,
            NULL,
            BMP_MENU_FRAME_1,
            FRAME_SPRITE,
            FRAME_SPRITELAYER,
            FRAME_PRIORITY);

        // Draw the title of the screen
        m_Font.SetTextColor (FONTCOLOR_WHITE);
        m_Font.DrawCenteredX(0, VIEW_WIDTH, SCREEN_TITLE_POSITION_Y, SCREEN_CREDITS_TITLE_STRING);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.DrawCenteredX(0, VIEW_WIDTH, SCREEN_TITLE_POSITION_Y + 60,  "Thibaut Tollemer");
        m_Font.DrawCenteredX(0, VIEW_WIDTH, SCREEN_TITLE_POSITION_Y + 80,  "Bernd Arnold");
        m_Font.DrawCenteredX(0, VIEW_WIDTH, SCREEN_TITLE_POSITION_Y + 100, "Jerome Bigot");
        m_Font.DrawCenteredX(0, VIEW_WIDTH, SCREEN_TITLE_POSITION_Y + 120, "Markus Drescher");
        m_Font.DrawCenteredX(0, VIEW_WIDTH, SCREEN_TITLE_POSITION_Y + 140, "Billy Araujo");

    }
    // We have to exit, so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= CREDITS_BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
