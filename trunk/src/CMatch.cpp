// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007-2010 Bernd Arnold
    Copyright (C) 2010 Markus Drescher

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
 *  \file CMatch.cpp
 *  \brief The match (bombers are playing)
 */

#include "StdAfx.h"
#include "CMatch.h"
#include "CDisplay.h"
#include "CInput.h"
#include "COptions.h"
#include "CBoard.h"
#include "CArena.h"
#include "CClock.h"
#include "CGame.h"
#include "CPauseMessage.h"
#include "CHurryMessage.h"

#include "CCommandChunk.h"
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define BLACKSCREEN_DURATION    0.750f  //!< Duration (in seconds) of each of the two black screens
#define PAUSE_BEGIN             1.0f    //!< Duration (in seconds) of the pause at the beginning of a match
#define PAUSE_DRAWGAME          2.5f    //!< Duration (in seconds) of the pause at match end when there is a draw game
#define PAUSE_WINNER            2.5f    //!< Duration (in seconds) of the pause at match end when there is a winner

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CCommandChunk CommandChunk;
float TimeElapsedSinceLastCommandChunk = 0.0f;
CArenaSnapshot Snapshot;

CMatch::CMatch(void) : CModeScreen()
{
    // Set the objects the board has to communicate with
    m_Board.SetClock(&m_Clock);
    m_Board.SetArena(&m_Arena);

    m_AiManager.SetArena(&m_Arena);

    m_pPauseMessage = NULL;
    m_pHurryMessage = NULL;
    
    m_MatchOver = false;
    m_WinnerTeam = NO_WINNER_TEAM;

    m_IsSongPlaying = false;
    m_NoticedTimeUp = false;
    m_ModeTime = 0.0f;
    m_HaveToExit = false;
    m_ForceDrawGame = false;    
    
#ifdef NETWORK_MODE
    m_pNetwork = NULL;
#endif

    m_CurrentSong = SONG_NONE;
    m_ExitModeTime = 0.0f;

    m_NoComputer = false;
    m_computerPlayersPresent = false;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMatch::~CMatch(void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CMatch, you must create it.

void CMatch::Create(void)
{
    CModeScreen::Create();

    // No match result for the moment
    m_MatchOver = false;
    m_WinnerTeam = NO_WINNER_TEAM;

    m_IsSongPlaying = false;
    m_NoticedTimeUp = false;

    // Reset mode time (no time has been elapsed in this mode yet)
    m_ModeTime = 0.0f;

    // Don't have to exit this mode yet
    m_HaveToExit = false;

    // Don't force a draw game at the beginning of the match
    m_ForceDrawGame = false;

#ifdef NETWORK_MODE
    if (m_pNetwork->NetworkMode() != NETWORKMODE_LOCAL)
    {
        m_pOptions->SetTimeStart(2, 35);
        m_pOptions->SetTimeUp(0, 30);

        m_pOptions->SetBomberType(2, BOMBERTYPE_OFF);
        m_pOptions->SetBomberType(3, BOMBERTYPE_OFF);
        m_pOptions->SetBomberType(4, BOMBERTYPE_OFF);
        m_pOptions->SetBattleCount(3);

        if (m_pNetwork->NetworkMode() == NETWORKMODE_SERVER)
        {
            m_pOptions->SetBomberType(0, BOMBERTYPE_MAN);
            m_pOptions->SetBomberType(1, BOMBERTYPE_NET);

#ifdef WIN32
            DWORD TickCount = GetTickCount();
#else
            DWORD TickCount = time(NULL);
#endif

            m_pNetwork->Send(SOCKET_CLIENT, (const char*)&TickCount, sizeof(DWORD));

            SEED_RANDOM(TickCount);
        }
        else if (m_pNetwork->NetworkMode() == NETWORKMODE_CLIENT)
        {
            m_pOptions->SetBomberType(0, BOMBERTYPE_NET);
            m_pOptions->SetBomberType(1, BOMBERTYPE_MAN);

            DWORD TickCount;

            m_pNetwork->Receive(SOCKET_SERVER, (char*)&TickCount, sizeof(DWORD));

        }
    }
#endif

    CreateMainComponents();

    // Set m_computerPlayersPresent to true when there are AI players in this match
    m_computerPlayersPresent = false;
    for (int i = 0; i < MAX_BOMBERS; i++) {
        if (m_pOptions->GetBomberType(i) == BOMBERTYPE_COM) {
            m_computerPlayersPresent = true;
        }
    }

    if (m_computerPlayersPresent) {
        m_AiManager.SetDisplay(m_pDisplay);
        m_AiManager.Create(m_pOptions);
    }

    for (int i = 0; i < MAX_TEAMS; i++)
    {
        m_Teams[i].SetTeamId(i);
        m_Teams[i].SetVictorious(false);
    }

#ifdef NETWORK_MODE

    // Each bomber is its own team
    for (int i = 0; i < MAX_BOMBERS; i++) {
        m_Arena.GetBomber(i).SetTeam(&m_Teams[i]);
    }

#else
    if (m_pOptions->GetBattleMode() == BATTLEMODE_TEAM)
    {
        // Set in selected team
        for (int i = 0; i < MAX_BOMBERS; i++) {
            if (m_pOptions->GetBomberTeam(i) == BOMBERTEAM_A)
                m_Arena.GetBomber(i).SetTeam(&m_Teams[0]);
            else if (m_pOptions->GetBomberTeam(i) == BOMBERTEAM_B)
                m_Arena.GetBomber(i).SetTeam(&m_Teams[1]);
        }
    }
    else
    {
        // Each bomber is its own team
        for (int i = 0; i < MAX_BOMBERS; i++) {
            m_Arena.GetBomber(i).SetTeam(&m_Teams[i]);
        }
    }
#endif

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMatch::CreateMainComponents(void)
{
    // Create the main components of the match
    m_Board.Create();
    m_Arena.Create();
    m_Clock.Create(CLOCKTYPE_COUNTDOWN,                    // Time decreases until zero
        CLOCKMODE_MS,                           // Compute minutes and seconds
        0,                                      // Start hours
        m_pOptions->GetTimeStartMinutes(),      // Start minutes
        m_pOptions->GetTimeStartSeconds(),      // Start seconds
        0);                                     // Start seconds100
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CMatch is not needed anymore, you should destroy it

void CMatch::Destroy(void)
{
    CModeScreen::Destroy();

    if (m_computerPlayersPresent) {
        m_AiManager.Destroy();
    }

    DestroyHurryUpMessage();
    DestroyPauseMessage();
    DestroyMainComponents();
    StopSong();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMatch::DestroyPauseMessage(void)
{
    // Delete the pause message object
    if (m_pPauseMessage != NULL)
    {
        delete m_pPauseMessage;
        m_pPauseMessage = NULL;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMatch::DestroyHurryUpMessage(void)
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

void CMatch::DestroyMainComponents(void)
{
    // Destroy the main components of the match
    m_Board.Destroy();
    m_Clock.Destroy();
    m_Arena.Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMatch::StopSong(void)
{
    // If the song is being played
    if (m_IsSongPlaying)
    {
        // Stop playing the match song
        m_pSound->StopSong(m_CurrentSong);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  Open the player inputs for each human playing player.
 *  The player inputs have to be opened before reading them.
 *
 *  @see CloseInput()
 */

void CMatch::OpenInput(void)
{
    // Scan the players
    for (int Player = 0; Player < MAX_PLAYERS; Player++)
    {
        // If this player plays and is human
        if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN
            && m_Arena.GetBomber(Player).Exist())
        {
            // Open its player input given current options
            m_pInput->GetPlayerInput(m_pOptions->GetPlayerInput(Player)).Open();
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  Close the player inputs for each human playing player.
 *  The player inputs should be closed when they are not needed anymore.
 *
 *  @see OpenInput()
 */

void CMatch::CloseInput(void)
{
    // Scan the players
    for (int Player = 0; Player < MAX_PLAYERS; Player++)
    {
        // If this player plays and is human
        if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN
            && m_Arena.GetBomber(Player).Exist())
        {
            // Close its player input given current options
            m_pInput->GetPlayerInput(m_pOptions->GetPlayerInput(Player)).Close();
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Start playing the match song if it hasn't started.
 */

void CMatch::PlaySong(void)
{
    // If there is no song playing right now
    if (!m_IsSongPlaying)
    {
        // Start playing the match song
        m_pSound->PlaySong(SONG_MATCH_MUSIC_1_NORMAL);

        // Save current song number
        m_CurrentSong = SONG_MATCH_MUSIC_1_NORMAL;

        // A song is currently playing
        m_IsSongPlaying = true;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Receive player commands.
 */

void CMatch::ProcessPlayerCommands(void)
{
    // If the match is not paused
    if (m_pPauseMessage == NULL)
    {
        // Do the AI stuff only when there are AI players
        if (m_computerPlayersPresent) {
            m_AiManager.Update(m_pTimer->GetDeltaTime());
        }

        // Scan the players
        for (int Player = 0; Player < MAX_PLAYERS; Player++)
        {
            // If this player plays and is a human
            if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN &&
                m_Arena.GetBomber(Player).Exist())
            {
                // If its bomber is still alive
                if (m_Arena.GetBomber(Player).IsAlive())
                {
                    EBomberMove BomberMove = BOMBERMOVE_NONE;
                    EBomberAction BomberAction = BOMBERACTION_NONE;

                    // Get his player input using the options object
                    int PlayerInput = m_pOptions->GetPlayerInput(Player);

                    // If this player input is opened
                    if (m_pInput->GetPlayerInput(PlayerInput).IsOpened())
                    {
                        // Update his player input
                        m_pInput->GetPlayerInput(PlayerInput).Update();

                        // Save the player's controls state
                        //! @see CInput, CPlayerInput::GetPlayerInput()
                        bool Up = m_pInput->GetPlayerInput(PlayerInput).TestUp();
                        bool Down = m_pInput->GetPlayerInput(PlayerInput).TestDown();
                        bool Left = m_pInput->GetPlayerInput(PlayerInput).TestLeft();
                        bool Right = m_pInput->GetPlayerInput(PlayerInput).TestRight();
                        bool Action1 = m_pInput->GetPlayerInput(PlayerInput).TestAction1();
                        bool Action2 = m_pInput->GetPlayerInput(PlayerInput).TestAction2();

                        // Determine the bomber move/action according to 
                        // which controls are activated
                        if (Up && Left)         BomberMove = BOMBERMOVE_UPLEFT;
                        else if (Up && Right)   BomberMove = BOMBERMOVE_UPRIGHT;
                        else if (Down && Left)  BomberMove = BOMBERMOVE_DOWNLEFT;
                        else if (Down && Right) BomberMove = BOMBERMOVE_DOWNRIGHT;
                        else if (Up)            BomberMove = BOMBERMOVE_UP;
                        else if (Down)          BomberMove = BOMBERMOVE_DOWN;
                        else if (Left)          BomberMove = BOMBERMOVE_LEFT;
                        else if (Right)         BomberMove = BOMBERMOVE_RIGHT;
                        else                    BomberMove = BOMBERMOVE_NONE;

                        if (Action1)        BomberAction = BOMBERACTION_ACTION1;
                        else if (Action2)   BomberAction = BOMBERACTION_ACTION2;
                        else                BomberAction = BOMBERACTION_NONE;

                        // Send these bomber move and bomber action to the bomber
                        m_Arena.GetBomber(Player).Command(BomberMove, BomberAction);
                    }
                    // If the player input is not opened
                    else
                    {
                        // Try to open the current player's player input
                        m_pInput->GetPlayerInput(PlayerInput).Open();

                        // If it's still not opened...
                        if (m_pInput->GetPlayerInput(PlayerInput).IsOpened())
                        {

                        }
                    }

#ifdef NETWORK_MODE
                    if (m_pNetwork->NetworkMode() == NETWORKMODE_CLIENT)
                    {
                        CommandChunk.Store(BomberMove, BomberAction, m_pTimer->GetDeltaTime());
                    }
#endif

                }
            }
            // If this player plays and is a network player
            else if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_NET)
            {

                // TODO:

            }
        }

#ifdef NETWORK_MODE
        TimeElapsedSinceLastCommandChunk += m_pTimer->GetDeltaTime();
        if (TimeElapsedSinceLastCommandChunk >= 0.050f)
        {

            if (m_pNetwork->NetworkMode() == NETWORKMODE_SERVER)
            {

                if (m_pNetwork->ReceiveCommandChunk(CommandChunk))
                {

                    // Scan all the players
                    for (int Player = 0; Player < MAX_PLAYERS; Player++)
                    {
                        // If this is the client's bomber
                        if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_NET)
                        {
                            // If the client's bomber is alive
                            if (m_Arena.GetBomber(Player).IsAlive())
                            {
                                // Apply the command chunk to the bomber
                                for (int Step = 0; Step < CommandChunk.GetNumberOfSteps(); Step++)
                                {
                                    m_Arena.GetBomber(Player).Command(CommandChunk.GetStepMove(Step), CommandChunk.GetStepAction(Step));
                                    m_Arena.UpdateSingleBomber(Player, CommandChunk.GetStepDuration(Step));
                                }

                                break;
                            }
                        }
                    }

                    // Make a snapshot of the arena and send it to the client
                    m_Arena.WriteSnapshot(Snapshot);

                    // Send snapshot to the client
                    m_pNetwork->SendSnapshot(Snapshot);

                }

            }
            else if (m_pNetwork->NetworkMode() == NETWORKMODE_CLIENT)
            {

                // Send client command chunk to the server
                m_pNetwork->SendCommandChunk(CommandChunk);

                // Command chunk was sent, reset it.
                CommandChunk.Reset();

                // If successfull apply it
                if (m_pNetwork->ReceiveSnapshot(Snapshot))
                    m_Arena.ReadSnapshot(Snapshot);

            }

            TimeElapsedSinceLastCommandChunk = 0.0f;
        }
#endif

    }

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//------------------
// Manage the pause
//------------------

void CMatch::ManagePauseMessage(void)
{
    // Check if a joystick pressed the "start" button
    // There is no check which joystick had requested the pause, so the pause can be ended
    // by every joystick and even the keyboard
    bool joystickRequestedPause = false;

    for (int Player = 0; Player < MAX_PLAYERS; Player++) {

        // If this player plays and is a human
        if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN &&
            m_Arena.GetBomber(Player).Exist())
        {
            // If its bomber is still alive
            if (m_Arena.GetBomber(Player).IsAlive())
            {
                // Get his player input using the options object
                int PlayerInputNr = m_pOptions->GetPlayerInput(Player);

                // If this player input is opened
                if (m_pInput->GetPlayerInput(PlayerInputNr).IsOpened()) {

                    if (m_pInput->GetPlayerInput(PlayerInputNr).TestMenuControl(JOYSTICK_BUTTON_BREAK)) {
                        joystickRequestedPause = true;
                    }

                }
            }
        }

    }

    // If the pause control is active
    if (m_pInput->GetMainInput().TestPause() || joystickRequestedPause)
    {
        // If the pause message is not created
        if (m_pPauseMessage == NULL)
        {
            // Create the pause message
            m_pPauseMessage = new CPauseMessage(m_pDisplay, m_pSound);
        }
        // If the pause message is created but is waiting for pause toggle
        else if (m_pPauseMessage->IsWaiting())
        {
            // Tell the pause message the pause is over
            m_pPauseMessage->GetOut();
        }
    }

    // If the pause message is created
    if (m_pPauseMessage != NULL)
    {
        // Update the pause message
        m_pPauseMessage->Update(m_pTimer->GetDeltaTime());

        // Update joysticks
        for (int i = 0; i < MAX_PLAYERS; i++) {
            m_pInput->GetPlayerInput(m_pOptions->GetPlayerInput(i)).Update();
        }

        // If the pause message has left the screen
        if (m_pPauseMessage->IsOutOfBounds())
        {
            // Delete the pause message object
            delete m_pPauseMessage;
            m_pPauseMessage = NULL;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMatch::UpdateMatch(void)
{
    // If the match is not paused
    if (m_pPauseMessage == NULL)
    {
        int AliveCount_Human = 0;   // Number of alive human bombers
        int AliveCount_AI = 0;   // Number of alive computer controlled bombers

        // Count human and AI bombers
        for (int Player = 0; Player < m_Arena.MaxBombers(); Player++)
        {
            // If this bomber exists
            if (m_Arena.GetBomber(Player).Exist())
            {
                // If this bomber is alive
                if (m_Arena.GetBomber(Player).IsAlive())
                {
                    // Count number of human and AI alive bombers
                    switch (m_Arena.GetBomber(Player).GetBomberType()) {
                    case BOMBERTYPE_MAN:
                    case BOMBERTYPE_NET:    AliveCount_Human++; break;
                    case BOMBERTYPE_COM:    AliveCount_AI++;    break;
                    default:                break; // #3078839
                    }

                }
            }
        }

        bool ForceArenaClosing = false;

        if (AliveCount_Human == 0 && AliveCount_AI > 1) {

            switch (m_pOptions->GetOption_ActionWhenOnlyAIPlayersLeft()) {
            case ACTIONONLYAIPLAYERSALIVE_ENDMATCHDRAWGAME: m_ForceDrawGame = true;     break;
            case ACTIONONLYAIPLAYERSALIVE_STARTCLOSING:     ForceArenaClosing = true;   break;
            case ACTIONONLYAIPLAYERSALIVE_SPEEDUPGAME:      m_pTimer->SetSpeed(7.0f); break;
            case ACTIONONLYAIPLAYERSALIVE_CONTINUEGAME:     break;
            default: ASSERT(false);
            }

            if (ForceArenaClosing) {
                // Stop the match music song (the normal one)
                m_pSound->StopSong(SONG_MATCH_MUSIC_1_NORMAL);
            }

        }

        // If the hurry up is enabled
        if (m_pOptions->GetTimeUpMinutes() != 0 || m_pOptions->GetTimeUpSeconds() != 0 || ForceArenaClosing)
        {
            //------------------------------------
            // Check if arena should close
            //------------------------------------

            // If the arena is not closing
            if (!m_NoticedTimeUp && !m_Arena.GetArenaCloser().IsClosing())
            {
                // If the clock's current time is less than (or equal to) to the timeup's time
                if (m_Clock.GetMinutes() < m_pOptions->GetTimeUpMinutes()
                    ||
                    (m_Clock.GetMinutes() == m_pOptions->GetTimeUpMinutes() &&
                    m_Clock.GetSeconds() <= m_pOptions->GetTimeUpSeconds())
                    ||
                    ForceArenaClosing)
                {
                    // Make the arena start closing
                    m_Arena.GetArenaCloser().Start();

                    // Start playing the fast match song
                    m_pSound->PlaySong(SONG_MATCH_MUSIC_1_FAST);

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

        m_Clock.Update(m_pTimer->GetDeltaTime());
        m_Board.Update();
        m_Arena.Update(m_pTimer->GetDeltaTime());
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Manage the hurry up message
 */

void CMatch::ManageHurryUpMessage(void)
{
    // If the match is not paused and the hurry up is enabled
    if (m_pPauseMessage == NULL && (m_pOptions->GetTimeUpMinutes() != 0 || m_pOptions->GetTimeUpSeconds() != 0))
    {
        // Dummy variables
        int ClockTotalSeconds = m_Clock.GetMinutes() * 60 + m_Clock.GetSeconds();
        int TimeUpTotalSeconds = m_pOptions->GetTimeUpMinutes() * 60 + m_pOptions->GetTimeUpSeconds();

        //// If the time is between TIMEUP and TIMEUP+1sec
        //if (ClockTotalSeconds >= TimeUpTotalSeconds && ClockTotalSeconds <= TimeUpTotalSeconds + 1)
        if (ClockTotalSeconds == TimeUpTotalSeconds + 1)
        {
            // If the hurry message doesn't exist
            if (m_pHurryMessage == NULL)
            {
                // Create the hurry message
                m_pHurryMessage = new CHurryMessage(m_pDisplay, m_pSound);
            }
        }
    }

    // If the hurry message exists
    if (m_pHurryMessage != NULL)
    {
        // Update the hurry message. If it has finished its behaviour
        if (m_pHurryMessage->Update(m_pTimer->GetDeltaTime()))
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

void CMatch::ManageMatchOver(void)
{
    //---------------------------------------------------
    // React if the match is over or is going to be over
    //---------------------------------------------------

    // If the match is not paused
    if (m_pPauseMessage == NULL)
    {
        int AliveCount = 0;     // Number of alive bombers
        int DyingCount = 0;     // Number of dying bombers

        // Count alive bombers and dying bombers
        for (int Player = 0; Player < m_Arena.MaxBombers(); Player++)
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

        int TeamCountAlive[MAX_TEAMS];     // Number of bombers on each team
        int TeamCountDying[MAX_TEAMS];     // Number of bombers on each team

        for (int Team = 0; Team < MAX_TEAMS; Team++)
        {
            TeamCountAlive[Team] = 0;
            TeamCountDying[Team] = 0;
        }

        for (int Player = 0; Player < m_Arena.MaxBombers(); Player++)
        {
            // If this bomber exists
            if (m_Arena.GetBomber(Player).Exist())
            {

                int Team = m_Arena.GetBomber(Player).GetTeam()->GetTeamId();

                if (m_Arena.GetBomber(Player).IsAlive())
                {
                    // Add one alive bomber in that team
                    TeamCountAlive[Team]++;
                }
                else if (m_Arena.GetBomber(Player).IsDying())
                {
                    // Add one dying bomber in that team
                    TeamCountDying[Team]++;
                }

            }
        }

        int CountTeamsAlive = 0;
        int CountTeamsDying = 0;

        for (int Team = 0; Team < MAX_TEAMS; Team++)
        {
            if (TeamCountAlive[Team] > 0)
                CountTeamsAlive++;

            if (TeamCountDying[Team] > 0)
                CountTeamsDying++;
        }

        // If no bomber is alive and there are only dying bombers
        if (AliveCount == 0 && DyingCount > 0)
        {
            // Stop the match song which was playing
            m_pSound->StopSong(m_CurrentSong);
        }
        // If no bomber is alive or dying then this is a draw game
        else if (AliveCount == 0 && DyingCount == 0)
        {
            // Match is over
            m_MatchOver = true;

            // There is no winner
            m_WinnerTeam = NO_WINNER_TEAM;

            // Tell the arena to stop closing if it is
            m_Arena.GetArenaCloser().Stop();

            // Make the board's clock animation stop
            m_Board.SetClockAnimation(false);

            // Determine mode time when we have to start the last black screen
            m_ExitModeTime = m_ModeTime + PAUSE_DRAWGAME;
        }
        // If only AI bombers are alive then this is also a draw game
        else if (m_ForceDrawGame)
        {
            // Stop the match song which was playing
            m_pSound->StopSong(m_CurrentSong);

            // Match is over
            m_MatchOver = true;

            // There is no winner
            m_WinnerTeam = NO_WINNER_TEAM;

            // Tell the arena to stop closing if it is
            m_Arena.GetArenaCloser().Stop();

            // Make the board's clock animation stop
            m_Board.SetClockAnimation(false);

            // Determine mode time when we have to start the last black screen
            m_ExitModeTime = m_ModeTime + PAUSE_DRAWGAME;

            // Tell the bombers there is no command
            for (int Player = 0; Player < m_Arena.MaxBombers(); Player++)
            {
                // If the bomber exists and is alive
                if (m_Arena.GetBomber(Player).Exist() &&
                    m_Arena.GetBomber(Player).IsAlive())
                {
                    // Send him no commands so as to avoid a bug where he keeps walking
                    m_Arena.GetBomber(Player).Command(BOMBERMOVE_NONE, BOMBERACTION_NONE);
                }
            }
        }
        // If one team is alive then that team has won the match
        else if (CountTeamsAlive == 1 && CountTeamsDying == 0)
        {

            // Match is over
            m_MatchOver = true;

            for (int Team = 0; Team < m_Arena.MaxTeams(); Team++)
            {
                if (TeamCountAlive[Team] > 0)
                {

                    // Save the winner player
                    m_WinnerTeam = Team;

                    // Tell the team it is victorious
                    m_Teams[Team].SetVictorious(true);

                    break;
                }
            }

            for (int Player = 0; Player < m_Arena.MaxBombers(); Player++)
            {
                // If the bomber exists and is alive
                if (m_Arena.GetBomber(Player).Exist() &&
                    m_Arena.GetBomber(Player).IsAlive())
                {
                    // Send him no commands so as to avoid a bug where he keeps walking
                    m_Arena.GetBomber(Player).Command(BOMBERMOVE_NONE, BOMBERACTION_NONE);
                }
            }

            // Play the bell sound (ding ding ding ding ding!)
            m_pSound->PlaySample(SAMPLE_RING_DING);

            // Stop the match song which was playing
            m_pSound->StopSong(m_CurrentSong);

            // Tell the arena to stop closing if it is
            m_Arena.GetArenaCloser().Stop();

            // Make the board's clock animation stop
            m_Board.SetClockAnimation(false);

            // Determine mode time when we have to start the last black screen
            m_ExitModeTime = m_ModeTime + PAUSE_WINNER;

        }
        else if (m_pOptions->GetTimeUpMinutes() == 0 && m_pOptions->GetTimeUpSeconds() == 0 &&
            (m_pOptions->GetTimeStartMinutes() != 0 || m_pOptions->GetTimeStartSeconds() != 0) &&
            m_Clock.GetMinutes() == 0 && m_Clock.GetSeconds() == 0)
        {
            // Match is over
            m_MatchOver = true;

            // There is no winner
            m_WinnerTeam = NO_WINNER_TEAM;

            // Seek the alive bombers
            for (int Player = 0; Player < m_Arena.MaxBombers(); Player++)
            {
                // If the bomber exists and is alive
                if (m_Arena.GetBomber(Player).Exist() &&
                    m_Arena.GetBomber(Player).IsAlive())
                {
                    // Send him no commands so as to avoid a bug where he keeps walking
                    m_Arena.GetBomber(Player).Command(BOMBERMOVE_NONE, BOMBERACTION_NONE);

                    // Tell the bomber that his team is "victorious"
                    m_Arena.GetBomber(Player).GetTeam()->SetVictorious(true);
                }
            }

            // Play the bell sound (ding ding ding ding ding!)
            m_pSound->PlaySample(SAMPLE_RING_DING);

            // Stop the match song which was playing
            m_pSound->StopSong(m_CurrentSong);

            // Tell the arena to stop closing if it is
            m_Arena.GetArenaCloser().Stop();

            // Make the board's clock animation stop
            m_Board.SetClockAnimation(false);

            // Determine mode time when we have to start the last black screen
            m_ExitModeTime = m_ModeTime + PAUSE_DRAWGAME;
        }

        if (m_MatchOver)
        {
            // Set the game speed to normal
            m_pTimer->SetSpeed(1.0f);
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EGameMode CMatch::Update(void)
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
        PlaySong();                        //!< @see PlaySong()
        ProcessPlayerCommands();           //!< @see ProcessPlayerCommands()
        ManagePauseMessage();              //!< @see ManagePauseMessage()
        UpdateMatch();                     //!< @see UpdateMatch()
        ManageHurryUpMessage();            //!< @see ManageHurryUpMessage()
        ManageMatchOver();                 //!< @see ManageMatchOver()
    }
    // If the match is over and we have make a pause before the last black screen
    else if (m_ModeTime <= m_ExitModeTime)
    {
        // Update the match
        m_Board.Update();
        m_Arena.Update(m_pTimer->GetDeltaTime());
    }
    // If the pause is over and we have to make the last black screen
    else if (m_ModeTime <= m_ExitModeTime + BLACKSCREEN_DURATION)
    {

    }
    // If the last black screen is over then ask for another game mode
    else
    {
        // If it's a draw game
        if (m_WinnerTeam == NO_WINNER_TEAM)
        {
            // Ask for a game mode change to draw game screen
            return GAMEMODE_DRAWGAME;
        }
        // If there is a winner
        else
        {
            // Ask for a game mode change to winner screen
            return GAMEMODE_WINNER;
        }
    }

    // Stay in this game mode
    return GAMEMODE_MATCH;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMatch::Display(void)
{
    // If we have to make the first black screen
    if (m_ModeTime <= BLACKSCREEN_DURATION)
    {
    }
    // If first black screen is done and we have to make a little 
    // pause to allow the players to see the arena before playing
    else if (m_ModeTime <= BLACKSCREEN_DURATION + PAUSE_BEGIN)
    {
        DisplayMatchScreen();

        // Reset display origin
        m_pDisplay->SetOrigin(0, 0);
    }
    // If match is currently playing and it's not over
    else if (!m_MatchOver)
    {
        // Reset display origin
        m_pDisplay->SetOrigin(0, 0);

        DisplayMatchScreen();
        DisplayHurryUpMessage();
        DisplayPauseMessage();
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

void CMatch::DisplayMatchScreen(void)
{
    m_Board.Display();
    m_Arena.Display();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMatch::DisplayHurryUpMessage(void)
{
    // If the hurry message exists
    if (m_pHurryMessage != NULL)
    {
        // Display the hurry message
        m_pHurryMessage->Display();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMatch::DisplayPauseMessage(void)
{
    // If the match is paused
    if (m_pPauseMessage != NULL)
    {
        // Display the pause message
        m_pPauseMessage->Display();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#ifdef _DEBUG_FLAG_1

void CMatch::_Debug_WriteBombsToLog() {

    theLog.WriteLine("CMatch::_Debug_WriteBombsToLog(): Bombs in m_Arena -- BEGIN --");

    for (int i = 0; i < m_Arena.MaxBombs(); i++) {
        theLog.WriteLine("--------------------------- Bomb %d -------------------------", i);
        m_Arena.GetBomb(i)._Debug_WriteToLog();
    }

    theLog.WriteLine("CMatch::_Debug_WriteBombsToLog(): Bombs in m_Arena -- END --");

}

#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

