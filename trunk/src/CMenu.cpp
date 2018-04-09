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
 *  \file CMenu.cpp
 *  \brief Menu
 */

#include "StdAfx.h"
#include "CMenu.h"
#include "CGame.h"
#include "CDisplay.h"
#include "CScores.h"
#include "CMosaic.h"
#include "CRandomMosaic.h"
#include "COptions.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MENU_BLACKSCREEN_DURATION          0.350f      //!< Duration (in seconds) of the two black screens

#define MOSAIC_SPRITE_LAYER                  0         //!< Sprite layer where to draw the mosaic tiles
#define MOSAIC_SPRITE_PRIORITY_IN_LAYER      0         //!< Priority to use in the sprite layer where to draw the mosaic tiles
#define MOSAIC_SPEED_X                      50.0f      //!< Speed of the mosaic background horizontally
#define MOSAIC_SPEED_Y                     -50.0f      //!< Speed of the mosaic background vertically

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenu::CMenu(void) : CModeScreen()
{
    // Initialize the pointers to NULL so that we 
    // can easily detect the ones we forgot to set.
    m_pScores = NULL;

    // Set the font object the menu screen objects have to communicate with
    m_MenuBomber.SetFont(&m_Font);
    m_MenuInput.SetFont(&m_Font);
    m_MenuMatch.SetFont(&m_Font);
    m_MenuTeam.SetFont(&m_Font);
    m_MenuLevel.SetFont(&m_Font);

    // The menu mode to start with will be set ONCE here 
    // (not on menu creation with Create() method). The 
    // reason is we have to keep the menu mode in memory 
    // even when we finish or start the game menu mode. 
    // This allows to get back to the last menu mode when 
    // a battle is over for example.
    m_MenuMode = MENUMODE_BOMBER;
    
    m_GameModeTime = 0.0f;
    m_HaveToExit = false;
    m_ExitGameMode = GAMEMODE_NONE;
    m_ExitGameModeTime = 0.0f;
    m_SongStarted = false;
    m_pMosaic = NULL;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMenu::~CMenu(void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenu::Create(void)
{
    CModeScreen::Create();

    // Check if all the objects to communicate with are set
    ASSERT(m_pScores != NULL);

    // Reset game mode time (no time has been elapsed in this game mode yet)
    m_GameModeTime = 0.0f;

    // Don't have to exit this game mode yet
    m_HaveToExit = false;

    // We didn't start playing the song yet
    m_SongStarted = false;

    // Make a random blue mosaic object
    m_pMosaic = CRandomMosaic::CreateRandomMosaic(m_pDisplay,
        MOSAIC_SPRITE_LAYER,
        MOSAIC_SPRITE_PRIORITY_IN_LAYER,
        MOSAIC_SPEED_X,
        MOSAIC_SPEED_Y,
        MOSAICCOLOR_BLUE,
        false);

    // Assure all scores are set to zero
    m_pScores->Reset();

    // Start the current menu mode! Even if the CMenu object
    // has never been created/destroyed, a menu mode was set
    // in the object's constructor. If it has already been
    // created/destroyed, then the current menu mode is the
    // last one that was set.
    StartMenuMode(m_MenuMode);

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenu::Destroy(void)
{
    CModeScreen::Destroy();

    // Delete the scrolling mosaic background
    m_pMosaic->Destroy();
    delete m_pMosaic;

    // If the song is playing
    if (m_SongStarted)
    {
        // Stop playing the menu song
        m_pSound->StopSong(SONG_MENU_MUSIC);
    }

    // Terminate menu mode. The current menu mode remains
    // the same so that it can be used in the next call to
    // the Create() method. This allows to get back to the 
    // last menu mode when a battle is over for example.
    FinishMenuMode();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenu::OpenInput(void)
{
    m_pInput->GetMainInput().Open();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenu::CloseInput(void)
{
    m_pInput->GetMainInput().Close();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EGameMode CMenu::Update(void)
{
    // Increase elapsed time since this game mode has started
    m_GameModeTime += m_pTimer->GetDeltaTime();

    // If we have to make the first black screen
    if (m_GameModeTime <= MENU_BLACKSCREEN_DURATION)
    {
    }
    // If minimum duration of the mode has not elapsed OR we don't have to exit yet
    else if (!m_HaveToExit)
    {
        // If we didn't start playing the song yet
        if (!m_SongStarted)
        {
            // Start playing the menu song
            m_pSound->PlaySong(SONG_MENU_MUSIC);

            // We started playing the song
            m_SongStarted = true;
        }

        // Update the scrolling mosaic background
        m_pMosaic->Update(m_pTimer->GetDeltaTime());

        //-------------------------------------
        // Update current menu object
        //-------------------------------------

        // We have to save the menu action to perform
        EMenuAction MenuAction = MENUACTION_NONE;

        // Update the object corresponding to the current menu mode
        switch (m_MenuMode)
        {
        case MENUMODE_BOMBER: MenuAction = m_MenuBomber.Update(); break;
        case MENUMODE_INPUT: MenuAction = m_MenuInput.Update(); break;
        case MENUMODE_MATCH: MenuAction = m_MenuMatch.Update(); break;
        case MENUMODE_TEAM: MenuAction = m_MenuTeam.Update(); break;
        case MENUMODE_LEVEL: MenuAction = m_MenuLevel.Update(); break;
        }

        //-------------------------------------
        // Perform menu action if needed
        //-------------------------------------

        // If there is a menu action to perform
        if (MenuAction != MENUACTION_NONE)
        {
            // Set the menu mode according to menu action to perform
            switch (MenuAction)
            {
                // We have to go back
            case MENUACTION_PREVIOUS:
            {
                // Perform the action according to the current menu mode
                switch (m_MenuMode)
                {
                case MENUMODE_BOMBER:
                {
                    // There is no previous menu mode. We have to switch to the
                    // game title screen (and therefore ask for another game mode). 
                    // Before that we have to make a black screen.

                    // We have to make a nice black screen and exit this game mode
                    m_HaveToExit = true;

                    // Stop playing the menu song
                    m_pSound->StopSong(SONG_MENU_MUSIC);

                    // Remember the game mode to ask for when exiting
                    m_ExitGameMode = GAMEMODE_TITLE;

                    break;
                }

                case MENUMODE_INPUT:
                {
                    // Save the previous menu mode to set
                    EMenuMode MenuMode = MENUMODE_BOMBER;

                    // Change the menu mode
                    FinishMenuMode();
                    StartMenuMode(MenuMode);

                    break;
                }

                case MENUMODE_MATCH:
                {
                    // Number of human players
                    int ManCount = 0;

                    // Scan the players
                    for (int Player = 0; Player < MAX_PLAYERS; Player++)
                    {
                        // If this player is a human player
                        if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN)
                        {
                            // Increase the number of human players
                            ManCount++;
                        }

                        int PlayerInput = m_pOptions->GetPlayerInput(Player);

                        // If the current input is a joystick, verify that this joystick is plugged in
                        if (PlayerInput >= m_pInput->GetPlayerInputCount())
                        {
                            // reset to keyboard 1
                            PlayerInput = 0;
                            m_pOptions->SetPlayerInput(Player, PlayerInput);
                        }

                    }

                    // Save the previous menu mode to set
                    // Skip the input menu if there is no human player
                    EMenuMode MenuMode = (ManCount > 0 ? MENUMODE_INPUT : MENUMODE_BOMBER);

                    // Change the menu mode
                    FinishMenuMode();
                    StartMenuMode(MenuMode);

                    break;
                }

                case MENUMODE_TEAM:
                {
                    // Save the previous menu mode to set
                    EMenuMode MenuMode = MENUMODE_MATCH;

                    // Change the menu mode
                    FinishMenuMode();
                    StartMenuMode(MenuMode);

                    break;
                }

                case MENUMODE_LEVEL:
                {
                    // Save the previous menu mode to set
                    EMenuMode MenuMode = (m_pOptions->GetBattleMode() == BATTLEMODE_TEAM ? MENUMODE_TEAM : MENUMODE_MATCH);

                    // Change the menu mode
                    FinishMenuMode();
                    StartMenuMode(MenuMode);

                    break;
                }
                }

                break;
            }

            // We have to go forward
            case MENUACTION_NEXT:
            {
                // Perform the action according to the current menu mode
                switch (m_MenuMode)
                {
                case MENUMODE_BOMBER:
                {
                    // Number of human players
                    int ManCount = 0;

                    // Scan the players
                    for (int Player = 0; Player < MAX_PLAYERS; Player++)
                    {
                        // If this player is a human player
                        if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN)
                        {
                            // Increase the number of human players
                            ManCount++;
                        }

                        int PlayerInput = m_pOptions->GetPlayerInput(Player);

                        // If the current input is a joystick, verify that this joystick is plugged in
                        if (PlayerInput >= m_pInput->GetPlayerInputCount())
                        {
                            // reset to keyboard 1
                            PlayerInput = 0;
                            m_pOptions->SetPlayerInput(Player, PlayerInput);
                        }

                    }

                    // Save the next menu mode to set
                    // Skip the input menu if there is no human player
                    EMenuMode MenuMode = (ManCount > 0 ? MENUMODE_INPUT : MENUMODE_MATCH);

                    // Change the menu mode
                    FinishMenuMode();
                    StartMenuMode(MenuMode);

                    break;
                }

                case MENUMODE_INPUT:
                {
                    // Save the next menu mode to set
                    EMenuMode MenuMode = MENUMODE_MATCH;

                    // Change the menu mode
                    FinishMenuMode();
                    StartMenuMode(MenuMode);

                    break;
                }

                case MENUMODE_MATCH:
                {
                    // Save the next menu mode to set
                    EMenuMode MenuMode = (m_pOptions->GetBattleMode() == BATTLEMODE_TEAM ? MENUMODE_TEAM : MENUMODE_LEVEL);

                    // Change the menu mode
                    FinishMenuMode();
                    StartMenuMode(MenuMode);

                    break;
                }

                case MENUMODE_TEAM:
                {
                    // Save the next menu mode to set
                    EMenuMode MenuMode = MENUMODE_LEVEL;

                    // Change the menu mode
                    FinishMenuMode();
                    StartMenuMode(MenuMode);

                    break;
                }

                case MENUMODE_LEVEL:
                {
                    // There is no next menu mode. We have to switch to the
                    // game match screen (and therefore ask for another game mode). 
                    // Before that we have to make a black screen.

                    // We have to make a nice black screen and exit this game mode
                    m_HaveToExit = true;

                    // Stop playing the menu song
                    m_pSound->StopSong(SONG_MENU_MUSIC);

                    // Remember the game mode to ask for when exiting
                    m_ExitGameMode = GAMEMODE_MATCH;

                    // Remember when we decided to exit this game mode
                    m_ExitGameModeTime = m_GameModeTime;

                    break;
                }
                }

                break;
            }
            default:
                break;
            }
        }
    }
    // The minimum mode duration has elapsed AND we have to exit, 
    // so we have to make the last black screen
    else if (m_GameModeTime - m_ExitGameModeTime <= MENU_BLACKSCREEN_DURATION)
    {

    }
    // Last black screen is complete! Get out of here!
    else
    {
        // Ask for the game mode that was saved when performing the menu action
        return m_ExitGameMode;
    }

    // Stay in this game mode
    return GAMEMODE_MENU;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Set a new menu mode. Creates the object 
// corresponding to the new menu mode.

void CMenu::StartMenuMode(EMenuMode MenuMode)
{
    // Set the new menu mode
    m_MenuMode = MenuMode;

    // Create the object corresponding to the new menu mode
    switch (m_MenuMode)
    {
    case MENUMODE_BOMBER: m_MenuBomber.Create(); break;
    case MENUMODE_INPUT: m_MenuInput.Create(); break;
    case MENUMODE_MATCH: m_MenuMatch.Create(); break;
    case MENUMODE_TEAM: m_MenuTeam.Create(); break;
    case MENUMODE_LEVEL: m_MenuLevel.Create(); break;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Set no menu mode. Destroys the object 
// corresponding to the current menu mode.

void CMenu::FinishMenuMode(void)
{
    // Destroy the object corresponding to the current menu mode
    switch (m_MenuMode)
    {
    case MENUMODE_BOMBER: m_MenuBomber.Destroy(); break;
    case MENUMODE_INPUT: m_MenuInput.Destroy(); break;
    case MENUMODE_MATCH: m_MenuMatch.Destroy(); break;
    case MENUMODE_TEAM: m_MenuTeam.Destroy(); break;
    case MENUMODE_LEVEL: m_MenuLevel.Destroy(); break;
    }

    // Don't modify current menu mode! Leave it as it is.
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenu::SetMenuMode(EMenuMode MenuMode)
{
    // If this menu mode is not already set
    if (m_MenuMode != MenuMode)
    {
        // Set this menu mode
        FinishMenuMode();
        StartMenuMode(MenuMode);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMenu::Display(void)
{
    // If we have to make the first black screen
    if (m_GameModeTime <= MENU_BLACKSCREEN_DURATION)
    {
    }
    // If minimum duration of the mode has not elapsed OR we don't have to exit yet
    else if (!m_HaveToExit)
    {
        //-------------------------------------
        // Draw the scrolling tiled background
        //-------------------------------------

        m_pMosaic->Display();

        //---------------------------------
        // Display the current menu screen
        //---------------------------------

        // Display the object corresponding to the current menu mode
        switch (m_MenuMode)
        {
        case MENUMODE_BOMBER: m_MenuBomber.Display(); break;
        case MENUMODE_INPUT: m_MenuInput.Display();  break;
        case MENUMODE_MATCH: m_MenuMatch.Display();  break;
        case MENUMODE_TEAM: m_MenuTeam.Display();  break;
        case MENUMODE_LEVEL: m_MenuLevel.Display();  break;
        }
    }
    // The minimum mode duration has elapsed AND we have to exit, 
    // so we have to make the last black screen
    else if (m_GameModeTime - m_ExitGameModeTime <= MENU_BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
