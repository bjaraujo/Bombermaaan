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
 *  \file CHelp.cpp
 *  \brief The control settings screen
 */

#include "StdAfx.h"
#include "CHelp.h"
#include "CGame.h"
#include "COptions.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CMosaic.h"
#include "CRandomMosaic.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define HELP_BLACKSCREEN_DURATION               0.750f  //!< Duration (in seconds) of the two black screens
                                                   
#define HELP_DISPLAY_ORIGIN_X                   0       //!< Display origin of the screen
#define HELP_DISPLAY_ORIGIN_Y                   0       
                                                        
#define HELP_TEXT_SPRITE_LAYER                  1       //!< Sprite layer where to draw strings
#define HELP_CURSOR_HAND_SPRITE_LAYER           1       //!< Sprite layer where to draw the cursor hand
                                                
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
#define SCREEN_TEXT_POSITION_X                    80

#define SCREEN_HELP_TITLE_STRING                "HELP"

#define ITEM_BOMB            8
#define ITEM_FLAME           9
#define ITEM_KICK            10
#define ITEM_ROLLER          11
#define ITEM_SKULL           12
#define ITEM_THROW           13
#define ITEM_PUNCH           14
#define ITEM_REMOTES         15
#define ITEM_SHIELD          16
#define ITEM_STRONGWEAK      17

#define FRAME_POSITION_X            30      //!< Position of the menu frame
#define FRAME_POSITION_Y            52
#define FRAME_SPRITE                0       //!< Sprite number of the menu frame sprite 
#define FRAME_PRIORITY              1       //!< Priority to use in the menu sprite layer when drawing the frame
#define FRAME_SPRITELAYER           0       //!< Sprite layer where to draw the menu frame

#define MOSAIC_SPRITE_LAYER                     0           //!< Sprite layer where to draw the mosaic tiles
#define MOSAIC_SPRITE_PRIORITY_IN_LAYER         0           //!< Priority to use in the sprite layer where to draw the mosaic tiles
#define MOSAIC_SPEED_X                         50.0f        //!< Speed of the mosaic background horizontally
#define MOSAIC_SPEED_Y                        -50.0f        //!< Speed of the mosaic background vertically

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CHelp::CHelp(void) : CModeScreen()
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

CHelp::~CHelp (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CHelp, you must create it.

void CHelp::Create (void)
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
    m_Font.SetSpriteLayer (HELP_TEXT_SPRITE_LAYER);

    // The cursor points to the first menu item
    m_Cursor = FIRST_MENU_ITEM;

    // Make a random purple mosaic object
    m_pMosaic = CRandomMosaic::CreateRandomMosaic (m_pDisplay, 
                                                   MOSAIC_SPRITE_LAYER, 
                                                   MOSAIC_SPRITE_PRIORITY_IN_LAYER, 
                                                   MOSAIC_SPEED_X, 
                                                   MOSAIC_SPEED_Y, 
                                                   MOSAICCOLOR_GREEN,
                                                   false);
}
    
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CHelp is not needed anymore, you should destroy it

void CHelp::Destroy (void)
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

void CHelp::OpenInput (void)
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CHelp::CloseInput (void)
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
// When the screen should continue, it returns GAMEMODE_HELP to keep this mode.
// When the screen has ended, it returns GAMEMODE_TITLE to start the title screen.

EGameMode CHelp::Update (void)
{
    // Increase elapsed time since mode has started
    m_ModeTime += m_pTimer->GetDeltaTime();

    // If we have to make the first black screen
    if (m_ModeTime <= HELP_BLACKSCREEN_DURATION)
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
    else if (m_ModeTime - m_ExitModeTime <= HELP_BLACKSCREEN_DURATION)
    {
        
    }
    // Last black screen is complete! Get out of here!
    else
    {
        return GAMEMODE_TITLE;
    }

    // Stay in this game mode
    return GAMEMODE_HELP;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CHelp::Display (void)
{
    // If we have to make the first black screen
    if (m_ModeTime <= HELP_BLACKSCREEN_DURATION)
    {
    }
    // If we don't have to exit yet
    else if (!m_HaveToExit)
    {

        // Set the position from which to display sprites
        m_pDisplay->SetOrigin (HELP_DISPLAY_ORIGIN_X, HELP_DISPLAY_ORIGIN_Y);

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
        m_Font.DrawCenteredX(0, VIEW_WIDTH, SCREEN_TITLE_POSITION_Y, SCREEN_HELP_TITLE_STRING);

        // Bomb help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 60,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_BOMB,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 60, "Increments number of bombs");

        // Flame help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 80,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_FLAME,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 80, "Increments flame size");

        // Roller help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 100,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_ROLLER,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 100, "Increses walk speed");

        // Kick help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 120,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_KICK,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 120, "Ability to kick bombs");

        // Throw help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 140,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_THROW,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 140, "Ability to throw bombs");

        // Punch help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 160,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_PUNCH,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 160, "Ability to punch bombs");

        // Remote help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 180,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_REMOTES,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 180, "Remote detonate");

        // Shield help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 200,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_SHIELD,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 200, "Shields one bomb blast");

        // Strong/weak help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 220,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_STRONGWEAK,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 220, "Strong/weak item");

        // Skull help
        m_pDisplay->DrawSprite(SCREEN_TEXT_POSITION_X,
            SCREEN_TITLE_POSITION_Y + 240,
            NULL,
            NULL,
            BMP_LEVEL_MINI_TILES,
            ITEM_SKULL,
            2,
            0);

        m_Font.SetTextColor(FONTCOLOR_YELLOW);
        m_Font.Draw(SCREEN_TEXT_POSITION_X + 30, SCREEN_TITLE_POSITION_Y + 240, "Sickness");

    }
    // We have to exit, so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= HELP_BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
