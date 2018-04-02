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
 *  \file CBoard.cpp
 *  \brief The board during a match showing time and bombers' health and wins
 */

#include "StdAfx.h"
#include "CBoard.h"
#include "CClock.h"
#include "CDisplay.h"
#include "COptions.h"
#include "CScores.h"
#include "CArena.h"

#include "CTimer.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Sprites stuff
#define SPRITE_SEMICOLON                10      // Sprite number of semicolon (:)
#define SPRITE_DASH                     11      // Sprite number of dash (-)
#define SPRITE_BOARD_BACKGROUND         0       // Sprite number of board background

// Sprite layers & priority stuff
#define BOARD_SPRITELAYER               100     // Sprite layer of all boards sprites (this layer must be above any layer)
#define BOARD_BACKGROUND_PRIORITY       0       // Background sprite priority in board sprite layer
#define BOARD_OBJECTS_PRIORITY          1       // Other board objects sprite priority in board sprite layer

// Clock animation sprites
#define ANIMCLOCKBOTTOM_SPRITE0         0
#define ANIMCLOCKBOTTOM_SPRITE1         1
#define ANIMCLOCKBOTTOM_SPRITE2         2
#define ANIMCLOCKBOTTOM_SPRITE3         3
#define ANIMCLOCKBOTTOM_SPRITE4         4
#define ANIMCLOCKBOTTOM_SPRITE5         5
#define ANIMCLOCKBOTTOM_SPRITE6         6
#define ANIMCLOCKBOTTOM_SPRITE7         7
#define ANIMCLOCKTOP_SPRITE0            0
#define ANIMCLOCKTOP_SPRITE1            1

// Clock animation time
#define ANIMCLOCKBOTTOM_NORMAL          0.090f  // How many seconds should elapse between two frames in normal clockbottom animation
#define ANIMCLOCKBOTTOM_FAST            0.045f  // How many seconds should elapse between two frames in fast clockbottom animation
#define ANIMCLOCKTOP_TIME               0.040f  // How many seconds should elapse between two frames in clocktop animation

// Pixel positions stuff
#define BOARD_POSITION_X                0       // Position of the board from origin
#define BOARD_POSITION_Y                0
#define CLOCKTOP_POSITION_X             4       // Position of the board clock from board origin
#define CLOCKTOP_POSITION_Y             2
#define CLOCKBOTTOM_POSITION_X          4       // Position of the board clock from board origin
#define CLOCKBOTTOM_POSITION_Y          9
#define TIME_DIGIT_SPACE                8       // X offset to add for next digit when drawing board time
#define SCORE_INITIAL_POSITION_X        79      // Position of the first bomber head to draw in the board
#define SCORE_INITIAL_POSITION_Y        5
#define SCORE_NEXT_X_OFFSET             35      // X offset to add for next bomber head position in the board
#define HEAD_TO_SCORE_X_OFFSET          17      // Offset to add to go from the bomber head position to the corresponding score
#define HEAD_TO_SCORE_Y_OFFSET          4
#define BOARD_BACKGROUND_POSITION_X     0       // Position on the board background from board origin
#define BOARD_BACKGROUND_POSITION_Y     0
#define TIME_POSITION_X                 23      // Position of the board time from board origin
#define TIME_POSITION_Y                 8

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CBoard::CBoard (void)
{
    // Initialize the pointers to NULL so that we 
    // can easily detect the ones we forgot to set.
    m_pClock = NULL;
    m_pDisplay = NULL;
    m_pOptions = NULL;
    m_pTimer = NULL;
    m_pScores = NULL;
    m_pArena = NULL;
    
    m_ClockBottomTimer = 0.0f;
    m_ClockTopTimer = 0.0f;
    m_ClockBottomSprite = 0;
    m_ClockTopSprite = 0;
    m_AnimateClock = false;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CBoard::~CBoard (void)
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CBoard, it must be created.

void CBoard::Create (void)
{
    // Check if all the objects to communicate with are set
    ASSERT (m_pDisplay != NULL);
    ASSERT (m_pClock != NULL);
    ASSERT (m_pOptions != NULL);
    ASSERT (m_pTimer != NULL);
    ASSERT (m_pScores != NULL);
    ASSERT (m_pArena != NULL);

    // Reset clock animation timers
    m_ClockBottomTimer = 0.0f;
    m_ClockTopTimer = 0.0f;

    // Set first clock top/bottom sprites in case the board is displayed and has never been updated
    m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE0;
    m_ClockTopSprite = ANIMCLOCKTOP_SPRITE0;

    // Animate the clock
    m_AnimateClock = true;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CBoard is not needed anymore, it should be destroyed.

void CBoard::Destroy (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// The update of the CBoard simply animates the clock

void CBoard::Update (void)
{
    //----------------------------
    // Clock sprite update
    //----------------------------
    
    // If the clock should animate
    if (m_AnimateClock)
    {
        // If there is not an infinite time for the battle
        if (m_pOptions->GetTimeStartMinutes() > 0 || m_pOptions->GetTimeStartSeconds() > 0)
        {
            // If the clock's current time is less than (or equal to) to the timeup's time
            if (m_pClock->GetMinutes() <  m_pOptions->GetTimeUpMinutes() ||
                (m_pClock->GetMinutes() == m_pOptions->GetTimeUpMinutes() && 
                 m_pClock->GetSeconds() <= m_pOptions->GetTimeUpSeconds()))
            {
                // Animate the bottom of the clock (fast)
                     if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_FAST)      m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE0;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_FAST * 2)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE1;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_FAST * 3)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE2;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_FAST * 4)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE3;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_FAST * 5)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE4;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_FAST * 6)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE5;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_FAST * 7)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE6;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_FAST * 8)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE7;
                else 
                { 
                    m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE0; 
                    m_ClockBottomTimer = 0.0f;
                }

                // Play clock bottom animation
                m_ClockBottomTimer += m_pTimer->GetDeltaTime();
            
                // Animate the top of the clock
                if (m_ClockTopTimer < ANIMCLOCKTOP_TIME)           m_ClockTopSprite = ANIMCLOCKTOP_SPRITE0;
                else if (m_ClockTopTimer < ANIMCLOCKTOP_TIME * 2)  m_ClockTopSprite = ANIMCLOCKTOP_SPRITE1;
                else 
                { 
                    m_ClockTopSprite = ANIMCLOCKTOP_SPRITE0; 
                    m_ClockTopTimer = 0.0f;
                }

                // Play clock top animation
                m_ClockTopTimer += m_pTimer->GetDeltaTime();
               
            }
            // Time is not up
            else
            {
                // Animate the bottom of the clock (normal speed)
                     if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_NORMAL)      m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE0;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_NORMAL * 2)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE1;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_NORMAL * 3)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE2;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_NORMAL * 4)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE3;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_NORMAL * 5)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE4;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_NORMAL * 6)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE5;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_NORMAL * 7)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE6;
                else if (m_ClockBottomTimer < ANIMCLOCKBOTTOM_NORMAL * 8)  m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE7;
                else 
                { 
                    m_ClockBottomSprite = ANIMCLOCKBOTTOM_SPRITE0; 
                    m_ClockBottomTimer = 0.0f;
                }

                // Play clock bottom animation
                m_ClockBottomTimer += m_pTimer->GetDeltaTime();
            }
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// This draws every sprite of the board : the background, the clock, 
// current time left, current player scores with the bomber heads.

void CBoard::Display (void)
{
    // Set the origin where to draw
    m_pDisplay->SetOrigin (BOARD_POSITION_X, BOARD_POSITION_Y);

    //-----------------------------------
    // Draw background
    //-----------------------------------

    m_pDisplay->DrawSprite (BOARD_BACKGROUND_POSITION_X, 
                            BOARD_BACKGROUND_POSITION_Y, 
                            NULL,                            // Draw entire background
                            NULL,                            // No need to clip
                            BMP_BOARD_BACKGROUND,
                            SPRITE_BOARD_BACKGROUND, 
                            BOARD_SPRITELAYER,
                            BOARD_BACKGROUND_PRIORITY);
    
    //-----------------------------------
    // Draw the clock
    //-----------------------------------

    // Draw the clock bottom part
    m_pDisplay->DrawSprite (CLOCKBOTTOM_POSITION_X, 
                            CLOCKBOTTOM_POSITION_Y, 
                            NULL,                            // Draw entire sprite
                            NULL,                            // No need to clip
                            BMP_BOARD_CLOCK_BOTTOM, 
                            m_ClockBottomSprite, 
                            BOARD_SPRITELAYER,
                            BOARD_OBJECTS_PRIORITY);

    // Draw the clock top part
    m_pDisplay->DrawSprite (CLOCKTOP_POSITION_X, 
                            CLOCKTOP_POSITION_Y, 
                            NULL,                            // Draw entire sprite
                            NULL,                            // No need to clip
                            BMP_BOARD_CLOCK_TOP, 
                            m_ClockTopSprite, 
                            BOARD_SPRITELAYER,
                            BOARD_OBJECTS_PRIORITY);


    //-----------------------------------
    // Draw the current clock time
    //-----------------------------------

    // If there is not an infinite time for the battle
    if (m_pOptions->GetTimeStartMinutes() > 0 || m_pOptions->GetTimeStartSeconds() > 0)
    {
        int Minutes = m_pClock->GetMinutes();
        int Seconds = m_pClock->GetSeconds();

        // Assert one character to draw only
        ASSERT (Minutes >= 0 && Minutes < 10);
    
        // Draw the number of minutes left
        m_pDisplay->DrawSprite (TIME_POSITION_X, 
                                TIME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_BOARD_TIME, 
                                Minutes, 
                                BOARD_SPRITELAYER,
                                BOARD_OBJECTS_PRIORITY);
    
        // Draw the ":" symbol
        m_pDisplay->DrawSprite (TIME_POSITION_X + TIME_DIGIT_SPACE, 
                                TIME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_BOARD_TIME, 
                                SPRITE_SEMICOLON, 
                                BOARD_SPRITELAYER,
                                BOARD_OBJECTS_PRIORITY);

        // Get each digit of the two-digit seconds number
        int Seconds10 = 0;      // Number of seconds 10 (seconds = 25 --> seconds10 = 2)
        int Seconds1 = 0;       // Number of seconds 1 (seconds = 25 --> seconds1 = 5)

        while (Seconds >= 10)
        {
            Seconds -= 10;
            Seconds10++; 
        }

        Seconds1 = Seconds;

        // Assert one character to draw only for each
        ASSERT (Seconds10 >= 0 && Seconds10 < 10);
        ASSERT (Seconds1 >= 0 && Seconds1 < 10);

        // Draw the two characters to draw the number of seconds
        m_pDisplay->DrawSprite (TIME_POSITION_X + TIME_DIGIT_SPACE * 2, 
                                TIME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_BOARD_TIME, 
                                Seconds10, 
                                BOARD_SPRITELAYER,
                                BOARD_OBJECTS_PRIORITY);

        m_pDisplay->DrawSprite (TIME_POSITION_X + TIME_DIGIT_SPACE * 3, 
                                TIME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_BOARD_TIME, 
                                Seconds1, 
                                BOARD_SPRITELAYER,
                                BOARD_OBJECTS_PRIORITY);
    }
    // If there is an infinite time for the battle
    else
    {
        // Draw the first dash "-"
        m_pDisplay->DrawSprite (TIME_POSITION_X, 
                                TIME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_BOARD_TIME, 
                                SPRITE_DASH, 
                                BOARD_SPRITELAYER,
                                BOARD_OBJECTS_PRIORITY);
    
        // Draw the ":" symbol
        m_pDisplay->DrawSprite (TIME_POSITION_X + TIME_DIGIT_SPACE, 
                                TIME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_BOARD_TIME, 
                                SPRITE_SEMICOLON, 
                                BOARD_SPRITELAYER,
                                BOARD_OBJECTS_PRIORITY);

        // Draw the second dash "-"
        m_pDisplay->DrawSprite (TIME_POSITION_X + TIME_DIGIT_SPACE * 2, 
                                TIME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_BOARD_TIME, 
                                SPRITE_DASH, 
                                BOARD_SPRITELAYER,
                                BOARD_OBJECTS_PRIORITY);

        // Draw the third dash "-"
        m_pDisplay->DrawSprite (TIME_POSITION_X + TIME_DIGIT_SPACE * 3 + 1, // +1 for the look
                                TIME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_BOARD_TIME, 
                                SPRITE_DASH, 
                                BOARD_SPRITELAYER,
                                BOARD_OBJECTS_PRIORITY);
    }

    //-----------------------------------
    // Draw scores and draw games count
    //-----------------------------------
    
    // Begin to draw at (ScoreX,ScoreY) from the 
    // top left corner of the board background
    int ScoreX = SCORE_INITIAL_POSITION_X;
    int ScoreY = SCORE_INITIAL_POSITION_Y;

    // Index in the score array
    int Player;

    // Draw the score of each player
    for (Player = 0 ; Player < MAX_PLAYERS ; Player++)
    {
        // If this player plays then draw its score
        if (m_pOptions->GetBomberType(Player) != BOMBERTYPE_OFF
            && m_pArena->GetBomber(Player).HasExisted())
        {
            int DeadHeadOffset = (m_pArena->GetBomber(Player).IsDead() ? 5 : 0);

            // Draw the player's bomber head
            m_pDisplay->DrawSprite (ScoreX, 
                                    ScoreY, 
                                    NULL,                            // Draw entire sprite
                                    NULL,                            // No need to clip
                                    BMP_BOARD_HEADS, 
                                    DeadHeadOffset + Player, 
                                    BOARD_SPRITELAYER,
                                    BOARD_OBJECTS_PRIORITY);
    
            // Draw the score
            m_pDisplay->DrawSprite (ScoreX + HEAD_TO_SCORE_X_OFFSET, 
                                    ScoreY + HEAD_TO_SCORE_Y_OFFSET, 
                                    NULL,                            // Draw entire sprite
                                    NULL,                            // No need to clip
                                    BMP_BOARD_SCORE, 
                                    m_pScores->GetPlayerScore(Player), 
                                    BOARD_SPRITELAYER,
                                    BOARD_OBJECTS_PRIORITY);
         
            // Next score to draw on the right
            ScoreX += SCORE_NEXT_X_OFFSET;
        }
    }

    // Display flag
    m_pDisplay->DrawSprite (ScoreX, 
                            ScoreY, 
                            NULL,                            // Draw entire sprite
                            NULL,                            // No need to clip
                            BMP_BOARD_DRAWGAME,
                            0, 
                            BOARD_SPRITELAYER,
                            BOARD_OBJECTS_PRIORITY);
    
    // Draw the number of draw games
    m_pDisplay->DrawSprite (ScoreX + HEAD_TO_SCORE_X_OFFSET, 
                            ScoreY + HEAD_TO_SCORE_Y_OFFSET, 
                            NULL,                            // Draw entire sprite
                            NULL,                            // No need to clip
                            BMP_BOARD_SCORE, 
                            m_pScores->GetDrawGamesCount(), 
                            BOARD_SPRITELAYER,
                            BOARD_OBJECTS_PRIORITY);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

