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
 *  \file CControls.cpp
 *  \brief The control settings screen
 */

#include "StdAfx.h"
#include "CControls.h"
#include "CGame.h"
#include "COptions.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CMosaic.h"
#include "CRandomMosaic.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define CONTROLS_BLACKSCREEN_DURATION           0.750f  //!< Duration (in seconds) of the two black screens
                                                   
#define CONTROLS_DISPLAY_ORIGIN_X               0       //!< Display origin of the screen
#define CONTROLS_DISPLAY_ORIGIN_Y               0       
                                                        
#define CONTROLS_TEXT_SPRITE_LAYER              1       //!< Sprite layer where to draw strings
#define CONTROLS_CURSOR_HAND_SPRITE_LAYER       1       //!< Sprite layer where to draw the cursor hand

#define SPRITE_CURSOR_HAND                      0
                                                
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
                                                
#define DEVICE_MENU_ITEM_STRING                 "DEVICE"
#define UP_MENU_ITEM_STRING                     "UP"
#define DOWN_MENU_ITEM_STRING                   "DOWN"
#define LEFT_MENU_ITEM_STRING                   "LEFT"
#define RIGHT_MENU_ITEM_STRING                  "RIGHT"
#define ACTION1_MENU_ITEM_STRING                "ACTION1"
#define ACTION2_MENU_ITEM_STRING                "ACTION2"
                                                
#define FIRST_PLAYER_INPUT                      0
                                                
#define SCREEN_TITLE_POSITION_Y                 (20+80)
#define FIRST_MENU_ITEM_POSITION_Y              (60+95)
#define ALL_MENU_ITEMS_POSITION_X               (33+110)
#define SPACE_Y_BETWEEN_MENU_ITEMS              21
#define SPACE_X_FROM_MENU_ITEM_TO_VALUE         90
#define SPACE_X_FROM_MENU_ITEM_TO_CURSOR_HAND   -25
#define SPACE_Y_FROM_MENU_ITEM_TO_CURSOR_HAND   -2
#define MAX_MENU_ITEM_NAME_LENGTH               50
#define MAX_MENU_ITEM_VALUE_NAME_LENGTH         50

#define SCREEN_CONTROLS_TITLE_STRING            "CONTROLS"

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

CControls::CControls (void) : CModeScreen()
{

    m_ModeTime = 0.0f;
    m_HaveToExit = false;
    m_ExitModeTime = 0.0f;
    m_Cursor = 0;
    m_PlayerInput = 0;
    m_WaitingForInput = false;
    m_WaitingForInputAfter = 0.0f;
    m_SongStarted = false;
    m_pMosaic = NULL;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CControls::~CControls (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CControls, you must create it.

void CControls::Create (void)
{
    // The first player input is selected
    m_PlayerInput = FIRST_PLAYER_INPUT;
    
    CModeScreen::Create();

    // Reset mode time (no time has been elapsed in this mode yet)
    m_ModeTime = 0.0f;

    // Don't have to exit this mode yet
    m_HaveToExit = false;

    // We didn't start playing the song yet
    m_SongStarted = false;

    // Not waiting for the user to activate the control he wants to reconfigure
    m_WaitingForInput = false;
    m_WaitingForInputAfter = 0.0f;

    // Initialize the font
    m_Font.Create ();
    m_Font.SetShadow (false);
    m_Font.SetSpriteLayer (CONTROLS_TEXT_SPRITE_LAYER);

    // The cursor points to the first menu item
    m_Cursor = FIRST_MENU_ITEM;

    // Make a random purple mosaic object
    m_pMosaic = CRandomMosaic::CreateRandomMosaic (m_pDisplay, 
                                                   MOSAIC_SPRITE_LAYER, 
                                                   MOSAIC_SPRITE_PRIORITY_IN_LAYER, 
                                                   MOSAIC_SPEED_X, 
                                                   MOSAIC_SPEED_Y, 
                                                   MOSAICCOLOR_PURPLE,
                                                   false);

}
    
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CControls is not needed anymore, you should destroy it

void CControls::Destroy (void)
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

void CControls::OpenInput (void)
{
    // Open the current selected player input
    m_pInput->GetPlayerInput(m_PlayerInput).Open();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CControls::CloseInput (void)
{
    // Close the current selected player input
    m_pInput->GetPlayerInput(m_PlayerInput).Close();
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
// When the screen should continue, it returns GAMEMODE_CONTROLS to keep this mode.
// When the screen has ended, it returns GAMEMODE_TITLE to start the title screen.

EGameMode CControls::Update (void)
{
    // Increase elapsed time since mode has started
    m_ModeTime += m_pTimer->GetDeltaTime();

    // If we have to make the first black screen
    if (m_ModeTime <= CONTROLS_BLACKSCREEN_DURATION)
    {
        
    }
    // If we don't have to exit yet
    else if (!m_HaveToExit)
    {
        // If we didn't start playing the song yet
        if (!m_SongStarted)
        {
            // Start playing the controls song
            m_pSound->PlaySong (SONG_CONTROLS_MUSIC);

            // We started playing the song
            m_SongStarted = true;
        }

        // Update the scrolling mosaic background
        m_pMosaic->Update (m_pTimer->GetDeltaTime());
        
        // If we are not about to wait for the user input
        // and we are not waiting for the user to activate the control he wants to reconfigure
        if (m_WaitingForInputAfter <= 0.0f && !m_WaitingForInput)
        {
            // Then react to the menu controls...

            // If the ESCAPE control is active
            if (m_pInput->GetMainInput().TestBreak())
            {
                // Stop playing the song
                m_pSound->StopSong (SONG_CONTROLS_MUSIC);
                
                // Remember we have to exit this mode
                m_HaveToExit = true;

                // Remember the mode time
                m_ExitModeTime = m_ModeTime;
            }
            // If the NEXT control is active
            else if (m_pInput->GetMainInput().TestNext())
            {
                // If the cursor is pointing to a control (and therefore not on the 'device' item)
                if (m_Cursor != 0)
                {
                    // Wait a little bit then wait for the user to activate the control he wants to reconfigure
                    m_WaitingForInputAfter = TIME_TO_WAIT_BEFORE_READING_INPUT; // Wait a little bit then listen to the input
                }
            }
            // If the UP control is active
            else if (m_pInput->GetMainInput().TestUp())
            {
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
            // If the DOWN control is active
            else if (m_pInput->GetMainInput().TestDown())
            {
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
            // If the LEFT control is active
            else if (m_pInput->GetMainInput().TestLeft())
            {
                // If the cursor is pointing on the DEVICE menu item
                if (m_Cursor == DEVICE_MENU_ITEM)
                {
                    // Play the menu beep sound
                    m_pSound->PlaySample (SAMPLE_MENU_BEEP);

                    // Close the player input that is now unselected
                    m_pInput->GetPlayerInput(m_PlayerInput).Close();
                    
                    // If the current player input is the first one
                    if (m_PlayerInput == FIRST_PLAYER_INPUT)
                    {
                        // Set the last player input
                        m_PlayerInput = m_pInput->GetPlayerInputCount() - 1;
                    }
                    // If the current player input is not the first one
                    else
                    {
                        // Set the previous player input
                        m_PlayerInput--;
                    }

                    // Open the player input that is now selected
                    m_pInput->GetPlayerInput(m_PlayerInput).Open();
                }
            }
            // If the RIGHT control is active
            else if (m_pInput->GetMainInput().TestRight())
            {
                // If the cursor is pointing on the DEVICE menu item
                if (m_Cursor == DEVICE_MENU_ITEM)
                {
                    // Play the menu beep sound
                    m_pSound->PlaySample (SAMPLE_MENU_BEEP);

                    // Close the player input that is now unselected
                    m_pInput->GetPlayerInput(m_PlayerInput).Close();
                    
                    // If the current player input is the last one
                    if (m_PlayerInput == m_pInput->GetPlayerInputCount() - 1)
                    {
                        // Set the first player input
                        m_PlayerInput = FIRST_PLAYER_INPUT;
                    }
                    // If the current player input is not the last one
                    else
                    {
                        // Set the next player input
                        m_PlayerInput++;
                    }

                    // Open the player input that is now selected
                    m_pInput->GetPlayerInput(m_PlayerInput).Open();
                }
            }
        }
        // If we are not waiting for the user to activate the control he wants to reconfigure
        else
        {
            // If we have to wait before waiting for the user to activate a control
            if (m_WaitingForInputAfter > 0.0f)
            {
                // Reduce the time left to wait
                m_WaitingForInputAfter -= m_pTimer->GetDeltaTime();
            
                // If we waited enough time
                if (m_WaitingForInputAfter <= 0.0f)
                {
                    // Now read the input and wait for the user to activate the control he wants to reconfigure
                    m_WaitingForInputAfter = 0.0f;
                    m_WaitingForInput = true;
                }
            }

            // If we have to read the input and wait for the user to activate a control
            if (m_WaitingForInput)
            {
                // Read the input on the current selected player input.
                // The user can only activate a control on the selected player input.
                m_pInput->GetPlayerInput(m_PlayerInput).Update();
                
                // Get the identification number of the control that is currently activated on the selected player input
                // If there is no activated control, it will be -1.
                int ActivatedControl = m_pInput->GetPlayerInput(m_PlayerInput).GetActivatedControl();

                // If there is an activated control on the selected player input
                if (ActivatedControl != NO_ACTIVATED_CONTROL)
                {
                    // Replace the control identification number in the options with the one the user activated
                    m_pOptions->SetControl (m_PlayerInput, m_Cursor - 1, ActivatedControl);
                    
                    // Not waiting for the user and reading the input anymore
                    m_WaitingForInput = false;
                }
            }
        }
    }
    // We have to exit, so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= CONTROLS_BLACKSCREEN_DURATION)
    {
        
    }
    // Last black screen is complete! Get out of here!
    else
    {
        return GAMEMODE_TITLE;
    }

    // Stay in this game mode
    return GAMEMODE_CONTROLS;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CControls::Display (void)
{
    // If we have to make the first black screen
    if (m_ModeTime <= CONTROLS_BLACKSCREEN_DURATION)
    {
    }
    // If we don't have to exit yet
    else if (!m_HaveToExit)
    {
        // Set the position from which to display sprites
        m_pDisplay->SetOrigin (CONTROLS_DISPLAY_ORIGIN_X, CONTROLS_DISPLAY_ORIGIN_Y);

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
        m_Font.DrawCenteredX(0, VIEW_WIDTH, SCREEN_TITLE_POSITION_Y, SCREEN_CONTROLS_TITLE_STRING);

        // Position Y on the screen of the first menu item in this screen
        int MenuItemPositionY = FIRST_MENU_ITEM_POSITION_Y;

        char ItemString[MAX_MENU_ITEM_NAME_LENGTH];             // Name of a menu item
        char ValueString[MAX_MENU_ITEM_VALUE_NAME_LENGTH];      // Name of the value corresponding to a menu item

        // Display each menu item of the screen
        for (int MenuItemIndex = 0 ; MenuItemIndex < NUMBER_OF_MENU_ITEMS ; MenuItemIndex++)
        {
            // According to the current menu item we have to display,
            // store the strings (name of the menu item and name of the value) we have to display
            switch (MenuItemIndex)
            {
                case DEVICE_MENU_ITEM :
                {
                    strcpy (ItemString, DEVICE_MENU_ITEM_STRING);
                    
                    // Value : name of the selected player input
                    strcpy (ValueString, 
                            m_pInput->GetPlayerInput(m_PlayerInput).GetName());
                    break;
                }

                case UP_MENU_ITEM :
                {
                    strcpy (ItemString, UP_MENU_ITEM_STRING);

                    // Value : name of the control on the selected player input
                    strcpy (ValueString, m_pInput->GetPlayerInput(m_PlayerInput).GetControlName(m_pOptions->GetControl(m_PlayerInput, CONTROL_UP)));
                    break;
                }

                case DOWN_MENU_ITEM :
                {
                    strcpy (ItemString, DOWN_MENU_ITEM_STRING);

                    // Value : name of the control on the selected player input
                    strcpy (ValueString, 
                            m_pInput->GetPlayerInput(m_PlayerInput).GetControlName(m_pOptions->GetControl(m_PlayerInput, CONTROL_DOWN)));
                    break;
                }

                case LEFT_MENU_ITEM :
                {
                    strcpy (ItemString, LEFT_MENU_ITEM_STRING);

                    // Value : name of the control on the selected player input
                    strcpy (ValueString, 
                            m_pInput->GetPlayerInput(m_PlayerInput).GetControlName(m_pOptions->GetControl(m_PlayerInput, CONTROL_LEFT)));
                    break;
                }

                case RIGHT_MENU_ITEM :
                {
                    strcpy (ItemString, RIGHT_MENU_ITEM_STRING);

                    // Value : name of the control on the selected player input
                    strcpy (ValueString, 
                            m_pInput->GetPlayerInput(m_PlayerInput).GetControlName(m_pOptions->GetControl(m_PlayerInput, CONTROL_RIGHT)));
                    break;
                }

                case ACTION1_MENU_ITEM :
                {
                    strcpy (ItemString, ACTION1_MENU_ITEM_STRING);

                    // Value : name of the control on the selected player input
                    strcpy (ValueString, 
                            m_pInput->GetPlayerInput(m_PlayerInput).GetControlName(m_pOptions->GetControl(m_PlayerInput, CONTROL_ACTION1)));
                    break;
                }

                case ACTION2_MENU_ITEM :
                {
                    strcpy (ItemString, ACTION2_MENU_ITEM_STRING);

                    // Value : name of the control on the selected player input
                    strcpy (ValueString, 
                            m_pInput->GetPlayerInput(m_PlayerInput).GetControlName(m_pOptions->GetControl(m_PlayerInput, CONTROL_ACTION2)));
                    break;
                }
            }

            // Draw the name of the menu item
            m_Font.SetTextColor (FONTCOLOR_GREEN);
            m_Font.Draw (ALL_MENU_ITEMS_POSITION_X, MenuItemPositionY, ItemString);
            
            // Don't display the value of the menu item if the users currently wants to reconfigure a control.
            // If we are not about to wait for input and not waiting for input -> then display
            // Or if we are, but the cursor hand is not pointing to the current menu item -> then display
            if ((m_WaitingForInputAfter <= 0.0f && !m_WaitingForInput) || m_Cursor != MenuItemIndex)
            {
                // Draw the name of the value corresponding to the menu item
                m_Font.SetTextColor (FONTCOLOR_YELLOW);
                m_Font.Draw (ALL_MENU_ITEMS_POSITION_X + SPACE_X_FROM_MENU_ITEM_TO_VALUE, MenuItemPositionY, ValueString);
            }
            
            // If the cursor hand is pointing to the current menu item
            if (m_Cursor == MenuItemIndex)
            {
                // Display the cursor hand
                m_pDisplay->DrawSprite (ALL_MENU_ITEMS_POSITION_X + SPACE_X_FROM_MENU_ITEM_TO_CURSOR_HAND,
                                        MenuItemPositionY + SPACE_Y_FROM_MENU_ITEM_TO_CURSOR_HAND,
                                        NULL,
                                        NULL,
                                        BMP_MENU_HAND,
                                        SPRITE_CURSOR_HAND,
                                        CONTROLS_CURSOR_HAND_SPRITE_LAYER,
                                        PRIORITY_UNUSED);
            }

            // Go down : next menu item to display
            MenuItemPositionY += SPACE_Y_BETWEEN_MENU_ITEMS;
        }
    }
    // We have to exit, so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= CONTROLS_BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
