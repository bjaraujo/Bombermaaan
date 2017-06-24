// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2008 Bernd Arnold

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
 *  \file CDemo.cpp
 *  \brief The demo
 */

#include "StdAfx.h"
#include "CDemo.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CBoard.h"
#include "CArena.h"
#include "CClock.h"
#include "CGame.h"
#include "CPauseMessage.h"
#include "CHurryMessage.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define BLACKSCREEN_DURATION    0.350f  //!< Duration (in seconds) of each of the two black screens
#define PAUSE_BEGIN             1.0f    //!< Duration (in seconds) of the pause at the beginning of a match
#define PAUSE_DRAWGAME          2.5f    //!< Duration (in seconds) of the pause at match end when there is a draw game
#define PAUSE_WINNER            2.5f    //!< Duration (in seconds) of the pause at match end when there is a winner

#define DEMO_TEXT_FLASH_TIME    0.2f                //!< Flash rate (in seconds) of the demo text 
#define DEMO_TEXT_POSITION_X    4                   //!< Position X where to draw the demo text
#define DEMO_TEXT_POSITION_Y    VIEW_HEIGHT - 14    //!< Position Y where to draw the demo text
#define DEMO_TEXT_STRING        "DEMO"              //!< Demo text to draw

#define LEVEL_FILENAME_POSITION_X   30
#define LEVEL_FILENAME_POSITION_Y   ((VIEW_HEIGHT-14)/2)

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDemo::CDemo (void) : CModeScreen()
{
    // Set the objects the board has to communicate with
    m_Board.SetClock (&m_Clock);
    m_Board.SetArena (&m_Arena);

    m_AiManager.SetArena (&m_Arena);

    m_pHurryMessage = NULL;
    
    m_MatchOver = false;
    m_CurrentSong = SONG_NONE;
    m_IsSongPlaying = false;
    m_NoticedTimeUp = false;
    m_DemoTextTime = 0.0f;
    m_DrawDemoText = false;
    m_ModeTime = 0.0f;
    m_ExitModeTime = 0.0f;
    m_ExitGameMode = 0;
    m_HaveToExit = false;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDemo::~CDemo (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CDemo, you must create it.

void CDemo::Create (void)
{
    CModeScreen::Create();

    // The match has not begun yet
    m_MatchOver = false;
    
    m_IsSongPlaying = false;
    m_NoticedTimeUp = false;

    // Reset mode time (no time has been elapsed in this mode yet)
    m_ModeTime = 0.0f;

    // Don't have to exit this mode yet
    m_HaveToExit = false;

    // Initialize demo text variables
    m_DemoTextTime = 0.0f;
    m_DrawDemoText = false;

    SetupOptions();             // Setting up the options must be done FIRST
    CreateMainComponents();
    CreateFont();

    m_AiManager.SetDisplay(m_pDisplay);
    m_AiManager.Create(&m_Options);

    for (int i = 0; i < MAX_TEAMS; i++)
    {
        m_Teams[i].SetTeamId(i);
        m_Teams[i].SetVictorious(false);
    }

    // Each bomber is its own team
    for (int i = 0; i < MAX_BOMBERS; i++) {
        m_Arena.GetBomber(i).SetTeam(&m_Teams[i]);
    }

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Modify the options to suit the demo mode.
 *
 *  We are working on a copy of the game options, so we can the settings without influencing the game options.
 *
 *  \sa CDemo::SetOptions
 */

void CDemo::SetupOptions (void)
{
    //! For each player
    for (int Player = 0 ; Player < MAX_PLAYERS ; Player++)
    {
        //! - This player is the computer
        m_Options.SetBomberType (Player, BOMBERTYPE_COM);
    }

    //! Play in a random level.
    m_Options.SetLevel (RANDOM(m_Options.GetNumberOfLevels()));

    //! Set default start time (1:00 min.) and time up (35 sec.).
    m_Options.SetTimeStart (1, 0);
    m_Options.SetTimeUp (0, 35);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::CreateMainComponents (void)
{
    // Create the main components of the match
    m_Board.Create ();
    m_Arena.Create ();
    m_Clock.Create (CLOCKTYPE_COUNTDOWN,                    // Time decreases until zero
                    CLOCKMODE_MS,                           // Compute minutes and seconds
                    0,                                      // Start hours
                    m_Options.GetTimeStartMinutes(),        // Start minutes
                    m_Options.GetTimeStartSeconds(),        // Start seconds
                    0);                                     // Start seconds100
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::CreateFont (void)
{
    // Create and initialize the font for our needs
    m_Font.Create ();
    m_Font.SetShadow (true);
    m_Font.SetShadowColor (FONTCOLOR_BLACK);
    m_Font.SetShadowDirection (SHADOWDIRECTION_DOWNRIGHT);
    m_Font.SetSpriteLayer (800);
    m_Font.SetTextColor(FONTCOLOR_WHITE);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CDemo is not needed anymore, you should destroy it

void CDemo::Destroy (void)
{
    CModeScreen::Destroy();

    m_Font.Destroy();
    m_AiManager.Destroy();

    StopSong();
    DestroyHurryUpMessage();
    DestroyMainComponents();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::DestroyMainComponents (void)
{
    // Destroy the main components of the match
    m_Board.Destroy ();
    m_Clock.Destroy ();
    m_Arena.Destroy ();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::StopSong (void)
{   
    // If the song is being played
    if (m_IsSongPlaying)
    {
        // Stop playing the match song
        m_pSound->StopSong (m_CurrentSong);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::DestroyHurryUpMessage (void)
{
    // Delete the hurry message object
    if (m_pHurryMessage != NULL)
    {
        delete m_pHurryMessage;
        m_pHurryMessage = NULL;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::OpenInput (void)
{
    m_pInput->GetMainInput().Open();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::CloseInput (void)
{
    m_pInput->GetMainInput().Close();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EGameMode CDemo::Update (void) 
{
    // Increase elapsed time since mode has started
    m_ModeTime += m_pTimer->GetDeltaTime();
    
    // If we have to make the first black screen
    if (m_ModeTime <= BLACKSCREEN_DURATION)
    {

    }
    // If the first black screen is done and we have to make a little 
    // pause to allow the players to see the arena before playing
    else if (m_ModeTime <= BLACKSCREEN_DURATION + PAUSE_BEGIN)
    {

    }
    // If match is currently playing and it's not over
    else if (!m_MatchOver)
    {
        PlaySong();
        ProcessPlayerCommands();
        ManageExit();
        UpdateMatch();
        UpdateDemoText();
        ManageHurryUpMessage();
        ManageMatchOver();        
    }
    // If the match is over and we have make a pause before the last black screen
    else if (m_ModeTime <= m_ExitModeTime)
    {
        m_Board.Update ();
        m_Arena.Update (m_pTimer->GetDeltaTime());
    }
    // If the pause is over and we have to make the last black screen
    else if (m_ModeTime <= m_ExitModeTime + BLACKSCREEN_DURATION)
    {

    }
    // If the last black screen is over then ask for another game mode
    else
    {
        // Ask for a switch to the title screen
        return GAMEMODE_TITLE;
    }

    // Stay in this game mode
    return GAMEMODE_DEMO; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::PlaySong (void)
{
    // If there is no song playing right now
    if (!m_IsSongPlaying)
    {
        // Start playing the match song
        m_pSound->PlaySong (SONG_MATCH_MUSIC_1_NORMAL);

        // Save current song number
        m_CurrentSong = SONG_MATCH_MUSIC_1_NORMAL;

        // A song is currently playing
        m_IsSongPlaying = true;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::ProcessPlayerCommands (void)
{
    m_AiManager.Update(m_pTimer->GetDeltaTime());
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::ManageExit (void)
{
    // If the user activates the break control
    if (m_pInput->GetMainInput().TestBreak())
    {
        // The match is "over"
        m_MatchOver = true;

        // We have to start the last black screen right now
        m_ExitModeTime = m_ModeTime;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::UpdateMatch (void)
{
    // If the hurry up is enabled
    if (m_Options.GetTimeUpMinutes() != 0 || m_Options.GetTimeUpSeconds() != 0)
    {
        // If the arena is not closing
        if (!m_NoticedTimeUp && !m_Arena.GetArenaCloser().IsClosing())
        {
            // If the clock's current time is less than (or equal to) to the timeup's time
            if (m_Clock.GetMinutes() < m_Options.GetTimeUpMinutes() 
                ||
                (m_Clock.GetMinutes() == m_Options.GetTimeUpMinutes() && 
                 m_Clock.GetSeconds() <= m_Options.GetTimeUpSeconds()))
            {
                // Make the arena start closing
                m_Arena.GetArenaCloser().Start ();

                // Start playing the fast match song
                m_pSound->PlaySong (SONG_MATCH_MUSIC_1_FAST);

                // Save current song number
                m_CurrentSong = SONG_MATCH_MUSIC_1_FAST;

                // Don't do this more than once
                m_NoticedTimeUp = true;
            }
        }
    }

    //------------------------------------
    // Update the match components
    //------------------------------------

    m_Clock.Update (m_pTimer->GetDeltaTime());
    m_Board.Update ();
    m_Arena.Update (m_pTimer->GetDeltaTime());
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::UpdateDemoText (void)
{
    // Update the time we have spent drawing (or not) the demo text
    m_DemoTextTime += m_pTimer->GetDeltaTime();

    // If we must start or stop displaying it
    if (m_DemoTextTime >= DEMO_TEXT_FLASH_TIME)
    {
        // Start/Stop displaying it
        m_DrawDemoText = !m_DrawDemoText;
        m_DemoTextTime = 0.0f;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::ManageHurryUpMessage (void)
{
    // If the hurry up is enabled
    if (m_Options.GetTimeUpMinutes() != 0 || m_Options.GetTimeUpSeconds() != 0)
    {
        // Dummy variables
        int ClockTotalSeconds = m_Clock.GetMinutes() * 60 + m_Clock.GetSeconds();
        int TimeUpTotalSeconds = m_Options.GetTimeUpMinutes() * 60 + m_Options.GetTimeUpSeconds();

        //// If the time is between TIMEUP and TIMEUP+1sec
        //if (ClockTotalSeconds >= TimeUpTotalSeconds && ClockTotalSeconds <= TimeUpTotalSeconds + 1)
        if (ClockTotalSeconds == TimeUpTotalSeconds + 1)
        {
            // If the hurry message doesn't exist
            if (m_pHurryMessage == NULL)
            {
                // Create the hurry message
                m_pHurryMessage = new CHurryMessage (m_pDisplay, m_pSound);
            }
        }
    }

    // If the hurry message exists
    if (m_pHurryMessage != NULL)
    {
        // Update the hurry message. If it has finished its behaviour
        if (m_pHurryMessage->Update (m_pTimer->GetDeltaTime()))
        {
            // Delete the hurry message object
            delete m_pHurryMessage;
            m_pHurryMessage = NULL;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::ManageMatchOver (void)
{
    int AliveCount = 0;     // Number of alive bombers
    int DyingCount = 0;     // Number of dying bombers

    // Count alive bombers and dying bombers
    for (int Player = 0 ; Player < m_Arena.MaxBombers() ; Player++)
    {
        // If this bomber exists
        if (m_Arena.GetBomber(Player).Exist())
        {
            // If this bomber is alive
            if (m_Arena.GetBomber(Player).IsAlive())        
            {
                // Add one alive bomber
                AliveCount++;
            }
            // If this bomber is dying
            else if (m_Arena.GetBomber(Player).IsDying())
            {
                // Add one dying bomber
                DyingCount++;
            }
        }
    }

    // If no bomber is alive and there are only dying bombers
    if (AliveCount == 0 && DyingCount > 0)
    {
        // Stop the match song which was playing
        m_pSound->StopSong (m_CurrentSong);
    }
    // If no bomber is alive or dying then this is a draw game
    else if (AliveCount == 0 && DyingCount == 0)
    {
        // Match is over
        m_MatchOver = true;
    
        // Tell the arena to stop closing if it is
        m_Arena.GetArenaCloser().Stop ();

        // Make the board's clock animation stop
        m_Board.SetClockAnimation (false);

        // Determine mode time when we have to start the last black screen
        m_ExitModeTime = m_ModeTime + PAUSE_DRAWGAME;
    }
    // If one bomber is alive and none is dying then a bomber has won the match
    else if (AliveCount == 1 && DyingCount == 0) 
    {
        // Match is over
        m_MatchOver = true;

        // Seek the only bomber that is still alive
        for (int Player = 0 ; Player < m_Arena.MaxBombers() ; Player++)
        {
            // If the bomber exists and is alive
            if (m_Arena.GetBomber(Player).Exist() &&
                m_Arena.GetBomber(Player).IsAlive())
            {
                // Send him no commands so as to avoid a bug where he keeps walking
                m_Arena.GetBomber(Player).Command (BOMBERMOVE_NONE, BOMBERACTION_NONE);

                // Tell the bomber he is victorious
                m_Arena.GetBomber(Player).GetTeam()->SetVictorious(true);
            
                // Get out since there is only one bomber
                break;
            }
        }

        // Play the bell sound (ding ding ding ding ding!)
        m_pSound->PlaySample (SAMPLE_RING_DING);
    
        // Stop the match song which was playing
        m_pSound->StopSong (m_CurrentSong);

        // Tell the arena to stop closing if it is
        m_Arena.GetArenaCloser().Stop ();

        // Make the board's clock animation stop
        m_Board.SetClockAnimation (false);

        // Determine mode time when we have to start the last black screen
        m_ExitModeTime = m_ModeTime + PAUSE_WINNER;
    }
    // If time is up (match is finished)
    else if (m_Options.GetTimeUpMinutes() == 0 && m_Options.GetTimeUpSeconds() == 0 &&
             (m_Options.GetTimeStartMinutes() != 0 || m_Options.GetTimeStartSeconds() != 0) &&
             m_Clock.GetMinutes() == 0 && m_Clock.GetSeconds() == 0)
    {
        // Match is over
        m_MatchOver = true;

        // Seek the alive bombers
        for (int Player = 0 ; Player < m_Arena.MaxBombers() ; Player++)
        {
            // If the bomber exists and is alive
            if (m_Arena.GetBomber(Player).Exist() &&
                m_Arena.GetBomber(Player).IsAlive())
            {
                // Send him no commands so as to avoid a bug where he keeps walking
                m_Arena.GetBomber(Player).Command (BOMBERMOVE_NONE, BOMBERACTION_NONE);

                // Tell the bomber he is "victorious"
                m_Arena.GetBomber(Player).GetTeam()->SetVictorious(true);
            }
        }

        // Play the bell sound (ding ding ding ding ding!)
        m_pSound->PlaySample (SAMPLE_RING_DING);
    
        // Stop the match song which was playing
        m_pSound->StopSong (m_CurrentSong);

        // Tell the arena to stop closing if it is
        m_Arena.GetArenaCloser().Stop ();

        // Make the board's clock animation stop
        m_Board.SetClockAnimation (false);

        // Determine mode time when we have to start the last black screen
        m_ExitModeTime = m_ModeTime + PAUSE_DRAWGAME;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::Display (void)
{
    // If we have to make the first black screen
    if (m_ModeTime <= BLACKSCREEN_DURATION)
    {
        // This would display the file name of the chosen level:
        // (Assumed: 8 is the width of a character in pixels)
        // m_Font.Draw ((VIEW_WIDTH - 8*strlen(m_Options.GetLevelName())) / 2, LEVEL_FILENAME_POSITION_Y, m_Options.GetLevelName());
    }
    // If first black screen is done and we have to make a little 
    // pause to allow the players to see the arena before playing
    else if (m_ModeTime <= BLACKSCREEN_DURATION + PAUSE_BEGIN)
    {
        // Display the board and the arena
        m_Board.Display ();
        m_Arena.Display ();

        // Reset display origin
        m_pDisplay->SetOrigin (0, 0);
    }
    // If match is currently playing and it's not over
    else if (!m_MatchOver)
    {
        // Reset display origin
        m_pDisplay->SetOrigin (0, 0);

        DisplayDemoText();
        DisplayMatchScreen();
        DisplayHurryUpMessage();
    }
    // If the match is over and we have make a pause before the last black screen
    else if (m_ModeTime <= m_ExitModeTime)
    {
        DisplayMatchScreen();
    }
    // If the pause is over and we have to make the last black screen
    else if (m_ModeTime <= m_ExitModeTime + BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::DisplayDemoText (void)
{
    // If we have to display the demo text
    if (m_DrawDemoText)
    {
        // Display the demo text
        m_Font.Draw (DEMO_TEXT_POSITION_X, DEMO_TEXT_POSITION_Y, DEMO_TEXT_STRING);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::DisplayMatchScreen (void)
{
    m_Board.Display ();
    m_Arena.Display ();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDemo::DisplayHurryUpMessage (void)
{   
    // If the hurry message exists
    if (m_pHurryMessage != NULL)
    {
        // Display the hurry message
        m_pHurryMessage->Display ();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
