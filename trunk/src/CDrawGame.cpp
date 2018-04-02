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
 *  \file CDrawGame.cpp
 *  \brief Draw game - no player won
 */

#include "StdAfx.h"
#include "CDrawGame.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CGame.h"
#include "CScores.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Draw game screen evolution in time
#define DRAWGAME_TITLE_DURATION         6.0f    //!< Duration (in seconds) of the draw game screen (with the big title)
#define DRAWGAME_BLACKSCREEN_DURATION   0.750f  //!< Duration (in seconds) of the two black screens
#define DRAWGAME_MINIMUM_DURATION       0.0f    //!< Minimum duration (in seconds) of the drawgame screen
                                                
// Display origin                               
#define DRAWGAME_DISPLAY_ORIGIN_X       0       //!< Display origin of the draw game screen
#define DRAWGAME_DISPLAY_ORIGIN_Y       0       
                                                
// Sprite position                              
#define DRAWGAME_MAIN_POSITION_X        0       //!< Position of the main sprite from draw game screen origin
#define DRAWGAME_MAIN_POSITION_Y        0       
#define DRAWGAME_FLAG_POSITION_X        250     //!< Position where to draw the animated flag
#define DRAWGAME_FLAG_POSITION_Y        178     
#define DRAWGAME_BIGFUME_POSITION_X     310     //!< Position of the big fume
#define DRAWGAME_BIGFUME_POSITION_Y     180     
#define DRAWGAME_SMALLFUME_POSITION_X   142     //!< Position of the small fume
#define DRAWGAME_SMALLFUME_POSITION_Y   222

// Sprite numbers
#define DRAWGAME_MAIN_SPRITE        0       //!< Main sprite with K-O bombers and title
#define FLAG_SPRITE0                0       //!< Flag animation frames
#define FLAG_SPRITE1                1       
#define BIGFUME_SPRITE0             0       //!< Sprite for a big fume
#define BIGFUME_SPRITE1             1       
#define SMALLFUME_SPRITE0           2       //!< Sprite for a small fume
#define SMALLFUME_SPRITE1           3       
                                            
// Sprite layer & priority                  
#define DRAWGAME_SPRITE_LAYER       0       //!< Sprite layer where to draw the drawgame screen sprites
#define DRAWGAME_MAIN_PRIORITY      0       //!< Priority-in-layer to use for the main sprite
#define DRAWGAME_FLAG_PRIORITY      1       //!< Priority-in-layer to use for the flag sprite
#define DRAWGAME_FUME_PRIORITY      2       //!< Priority-in-layer to use for the fume sprites
                                            
// Animation                                
#define ANIMFLAG_TIME_0             0.300f                      //!< Flag animation times
#define ANIMFLAG_TIME_1             (ANIMFLAG_TIME_0 * 2)
#define ANIMBIGFUME_TIME_0          0.250f                      //!< Big fume animation times
#define ANIMBIGFUME_TIME_1          (ANIMBIGFUME_TIME_0 * 2)
#define ANIMSMALLFUME_TIME_0        0.220f                      //!< Small fume animation times
#define ANIMSMALLFUME_TIME_1        (ANIMSMALLFUME_TIME_0 * 2)

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDrawGame::CDrawGame (void) : CModeScreen()
{
    // Initialize the pointers to NULL so that we 
    // can easily detect the ones we forgot to set.
    m_pScores = NULL;
    
    m_ModeTime = 0.0f;
    m_FlagTimer = 0.0f;
    m_BigFumeTimer = 0.0f;
    m_SmallFumeTimer = 0.0f;
    m_HaveToExit = false;
    m_ExitModeTime = 0.0f;
    m_PlayedSound = false;

    m_FlagSprite = FLAG_SPRITE0;
    m_BigFumeSprite = BIGFUME_SPRITE0;
    m_SmallFumeSprite = SMALLFUME_SPRITE0;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDrawGame::~CDrawGame (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CDrawGame, you must create it.

void CDrawGame::Create (void)
{
    CModeScreen::Create();
    
    // Check if all the objects to communicate with are set
    ASSERT (m_pScores != NULL);
    
    // Reset mode time
    m_ModeTime = 0.0f;

    // Reset animation timers
    m_FlagTimer = 0.0f;
    m_BigFumeTimer = 0.0f;
    m_SmallFumeTimer = 0.0f;

    // Update the draw games count in the scores object
    m_pScores->RaiseDrawGamesCount ();

    // Don't have to exit this mode yet
    m_HaveToExit = false;
    m_ExitModeTime = 0.0f;

    m_PlayedSound = false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CDrawGame is not needed anymore, you should destroy it

void CDrawGame::Destroy (void)
{
    CModeScreen::Destroy();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDrawGame::OpenInput (void)
{
    m_pInput->GetMainInput().Open();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDrawGame::CloseInput (void)
{
    m_pInput->GetMainInput().Close();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EGameMode CDrawGame::Update (void)
{
    // Increase elapsed time since mode has started
    m_ModeTime += m_pTimer->GetDeltaTime();
        
    // If it's time to make the first black screen
    if (m_ModeTime <= DRAWGAME_BLACKSCREEN_DURATION)
    {
    
    }
    // If the maximum duration of the drawgame screen has not elapsed and we don't have to exit this mode yet    
    else if (m_ModeTime <= DRAWGAME_BLACKSCREEN_DURATION + DRAWGAME_TITLE_DURATION && !m_HaveToExit)
    {
        //---------------------------------------
        // Play a sound if it hasn't started yet
        //---------------------------------------

        // If the draw game sound hasn't started yet
        if (!m_PlayedSound)
        {
            // Start playing the draw game sound
            m_pSound->PlaySample (SAMPLE_DRAW_GAME);
            m_pSound->PlaySample (SAMPLE_DRAW_GAME_VOICE);

            // Don't do this more than once
            m_PlayedSound = true;
        }

        //------------------------------------------------------------------------------------
        // React to controls from human players if minimum duration of the screen has elapsed
        //------------------------------------------------------------------------------------

        // If the minimum duration of the drawgame screen has elapsed
        if (m_ModeTime >= DRAWGAME_BLACKSCREEN_DURATION + DRAWGAME_MINIMUM_DURATION)
        {
            // If the NEXT control is active
            if (m_pInput->GetMainInput().TestNext())
            {
                // Remember we have to exit this mode
                m_HaveToExit = true;

                // Remember the mode time
                m_ExitModeTime = m_ModeTime;
            }
        }

        //-----------------------
        // Animate flag
        //-----------------------

        // Animate the flag
             if (m_FlagTimer < ANIMFLAG_TIME_0)  m_FlagSprite = FLAG_SPRITE0;
        else if (m_FlagTimer < ANIMFLAG_TIME_1)  m_FlagSprite = FLAG_SPRITE1;
        else 
        { 
            // Set next sprite
            m_FlagSprite = FLAG_SPRITE0; 
    
            // Reset animation timer
            m_FlagTimer = 0.0f;
        }

        // Play flag animation
        m_FlagTimer += m_pTimer->GetDeltaTime();

        //-----------------------
        // Animate big fume
        //-----------------------

        // Animate the big fume
             if (m_BigFumeTimer < ANIMBIGFUME_TIME_0)  m_BigFumeSprite = BIGFUME_SPRITE0;
        else if (m_BigFumeTimer < ANIMBIGFUME_TIME_1)  m_BigFumeSprite = BIGFUME_SPRITE1;
        else 
        { 
            // Set next sprite
            m_BigFumeSprite = BIGFUME_SPRITE0; 
    
            // Reset animation timer
            m_BigFumeTimer = 0.0f;
        }

        // Play big fume animation
        m_BigFumeTimer += m_pTimer->GetDeltaTime();

        //-----------------------
        // Animate small fume
        //-----------------------

        // Animate the small fume
             if (m_SmallFumeTimer < ANIMSMALLFUME_TIME_0)  m_SmallFumeSprite = SMALLFUME_SPRITE0;
        else if (m_SmallFumeTimer < ANIMSMALLFUME_TIME_1)  m_SmallFumeSprite = SMALLFUME_SPRITE1;
        else 
        { 
            // Set next sprite
            m_SmallFumeSprite = SMALLFUME_SPRITE0; 
    
            // Reset animation timer
            m_SmallFumeTimer = 0.0f;
        }

        // Play small fume animation
        m_SmallFumeTimer += m_pTimer->GetDeltaTime();
    }
    // If it's time to make the last black screen
    else if (m_ModeTime <= m_ExitModeTime + DRAWGAME_BLACKSCREEN_DURATION)
    {
    
    }
    // Last black screen is completed! Get out of here
    else
    {
        // Ask for match game mode in order to start a new match
        return GAMEMODE_MATCH;
    }   
    
    // Stay in this game mode
    return GAMEMODE_DRAWGAME;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDrawGame::Display (void)
{
    // If it's time to make the first black screen
    if (m_ModeTime <= DRAWGAME_BLACKSCREEN_DURATION)
    {
    }
    // If the maximum duration of the drawgame screen has not elapsed and we don't have to exit this mode yet    
    else if (m_ModeTime <= DRAWGAME_BLACKSCREEN_DURATION + DRAWGAME_TITLE_DURATION && !m_HaveToExit)
    {
        // Set the position from which to display sprites
        m_pDisplay->SetOrigin (DRAWGAME_DISPLAY_ORIGIN_X, DRAWGAME_DISPLAY_ORIGIN_Y);

        // Draw the main sprite
        m_pDisplay->DrawSprite (DRAWGAME_MAIN_POSITION_X, 
                                DRAWGAME_MAIN_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_DRAWGAME_MAIN, 
                                DRAWGAME_MAIN_SPRITE, 
                                DRAWGAME_SPRITE_LAYER, 
                                DRAWGAME_MAIN_PRIORITY);

        // Draw the flag
        m_pDisplay->DrawSprite (DRAWGAME_FLAG_POSITION_X, 
                                DRAWGAME_FLAG_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_DRAWGAME_FLAG, 
                                m_FlagSprite, 
                                DRAWGAME_SPRITE_LAYER, 
                                DRAWGAME_FLAG_PRIORITY);

        // Draw the big fume
        m_pDisplay->DrawSprite (DRAWGAME_BIGFUME_POSITION_X, 
                                DRAWGAME_BIGFUME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_DRAWGAME_FUMES,
                                m_BigFumeSprite, 
                                DRAWGAME_SPRITE_LAYER, 
                                DRAWGAME_FUME_PRIORITY);

        // Draw the small fume
        m_pDisplay->DrawSprite (DRAWGAME_SMALLFUME_POSITION_X, 
                                DRAWGAME_SMALLFUME_POSITION_Y, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_DRAWGAME_FUMES,
                                m_SmallFumeSprite, 
                                DRAWGAME_SPRITE_LAYER, 
                                DRAWGAME_FUME_PRIORITY);
    }
    // If it's time to make the last black screen
    else if (m_ModeTime <= m_ExitModeTime + DRAWGAME_BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
