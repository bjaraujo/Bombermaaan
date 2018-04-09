// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2010 Bernd Arnold

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
 *  \file CWinner.cpp
 *  \brief The winner screen (after a match has ended and one bomber has won the match)
 */

#include "StdAfx.h"
#include "CWinner.h"
#include "CScores.h"
#include "CMatch.h"
#include "CGame.h"
#include "COptions.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CMosaic.h"
#include "CRandomMosaic.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Duration (in seconds) of the two black screens
#define WINNER_BLACKSCREEN_DURATION     0.750f

// Minimum duration (in seconds) of the mode. Before this duration we don't check input.
// After this minimum duration has elapsed we receive input and start exiting when any control is pressed
#define WINNER_MINIMUM_DURATION         0.0f

// Display origin
#define WINNER_DISPLAY_ORIGIN_X       0       //!< Display origin of the winner screen
#define WINNER_DISPLAY_ORIGIN_Y       0

// Deltas for 16 and 32 pixel versions
#define WINNER_SPRITES_OFFSET_X         ((VIEW_WIDTH - 240) / 2)
#define WINNER_SPRITES_OFFSET_Y         ((VIEW_HEIGHT - 234) / 2)

// Sprite layer & priority
#define WINNER_SPRITE_LAYER             0       //!< Sprite layer where to draw the winner screen sprites
#define WINNER_LIGHTS_PRIORITY          1       //!< Priority-in-layer to use for light sprites
#define WINNER_BOMBER_PRIORITY          1       //!< Priority-in-layer to use for bomber sprites
#define WINNER_COIN_PRIORITY            1       //!< Priority-in-layer to use for static and animated coins sprites
#define WINNER_SCOREBOARD_PRIORITY      1       //!< Priority-in-layer to use for scoreboard title sprite
#define WINNER_CROSS_PRIORITY           1       //!< Priority-in-layer to use when drawing a cross sprite

// Scoreboard title
#define SCOREBOARD_TITLE_POSITION_X     41      //!< Position of the first letter of the scoreboard title
#define SCOREBOARD_TITLE_POSITION_Y     19
#define SCOREBOARD_SPRITE               0       //!< Scoreboard title sprite

// Lights stuff
#define LIGHTS_FULLROW1_POSITION_X      9       //!< First light position of the first full row of lights to draw
#define LIGHTS_FULLROW1_POSITION_Y      7
#define LIGHTS_FULLROW2_POSITION_X      9       //!< First light position of the second full row of lights to draw
#define LIGHTS_FULLROW2_POSITION_Y      42
#define LIGHTS_FULLROW3_POSITION_X      9       //!< First light position of the third full row of lights to draw
#define LIGHTS_FULLROW3_POSITION_Y      224
#define LIGHTS_FULLCOLUMN1_POSITION_X   9       //!< First light position of the first full column of lights to draw
#define LIGHTS_FULLCOLUMN1_POSITION_Y   7
#define LIGHTS_FULLCOLUMN2_POSITION_X   226     //!< First light position of the second full column of lights to draw
#define LIGHTS_FULLCOLUMN2_POSITION_Y   7
#define LIGHTS_SEMICOLUMN_POSITION_X    51      //!< First light position of the semi column of lights to draw
#define LIGHTS_SEMICOLUMN_POSITION_Y    42
#define LIGHTS_FULLROW_COUNT            32      //!< How many lights in a full row?
#define LIGHTS_FULLCOLUMN_COUNT         32      //!< How many lights in a full column?
#define LIGHTS_SEMICOLUMN_COUNT         27      //!< How many lights in the semi column?
#define LIGHTS_SPACE_X                  7       //!< Space (in pixels) between two lights's X positions
#define LIGHTS_SPACE_Y                  7       //!< Space (in pixels) between two lights's Y positions
#define LIGHTS_ANIMATION_TIME_0         0.150f  //!< Light animation times
#define LIGHTS_ANIMATION_TIME_1         LIGHTS_ANIMATION_TIME_0 * 2
#define LIGHTS_ANIMATION_TIME_2         LIGHTS_ANIMATION_TIME_0 * 3
#define LIGHTS_ANIMATION_TIME_3         LIGHTS_ANIMATION_TIME_0 * 4
#define LIGHTS_COLORS_COUNT             4       //!< Number of different colors a light can have

// Bomber stuff
#define BOMBER_INITIAL_POSITION_X       20      //!< First (top) bomber position
#define BOMBER_INITIAL_POSITION_Y       55
#define BOMBER_SPACE_Y                  33      //!< Space in pixels between two bombers Y position
#define BOMBER_HAPPY_SPRITE_0           0       //!< Happy bomber sprites
#define BOMBER_HAPPY_SPRITE_1           1
#define BOMBER_SAD_SPRITE_0             2       //!< Sad bomber sprites (crying bomber)
#define BOMBER_SAD_SPRITE_1             3
#define BOMBER_SPRITES_COUNT_PER_COLOR  4       //!< How many bomber sprites for each color?
#define BOMBER_HAPPY_ANIMATION_TIME_0   0.700f  //!< Happy bomber animation times
#define BOMBER_HAPPY_ANIMATION_TIME_1   1.600f
#define BOMBER_SAD_ANIMATION_TIME_0     0.100f  //!< Sad bomber animation times
#define BOMBER_SAD_ANIMATION_TIME_1     0.250f
#define BOMBER_SAD_ANIMATION_TIME_2     0.600f
#define BOMBER_SAD_ANIMATION_TIME_3     1.200f

// Coins stuff
#define COINS_INITIAL_POSITION_X        68      //!< Position of the first (left) coin of the first (top) bomber
#define COINS_INITIAL_POSITION_Y        61
#define COINS_SPACE_X                   31      //!< Space in pixels between two coins X position
#define COINS_SPACE_Y                   33      //!< Space in pixels between two coins Y position
#define COINS_STATIC_SPRITE             0       //!< Sprite number for a static coin (non moving)
#define COINS_ANIMATION_TIME 0.2f               //!< Animation time between each sprite
#define COINS_ANIMATION_TURNS 2                 //!< Number of turns before the coin isn't animated any longer
#define COINS_SPRITE_COUNT 16                   //!< Number of different coin sprites

// Cross stuff
#define CROSS_SPACE_X           1   //!< Space in pixels between a cross sprite position for one player
#define CROSS_SPACE_Y           4   //!< and the bomber sprite position for the same player

#define MOSAIC_SPRITE_LAYER                0           //!< Sprite layer where to draw the mosaic tiles
#define MOSAIC_SPRITE_PRIORITY_IN_LAYER    0           //!< Priority to use in the sprite layer where to draw the mosaic tiles
#define MOSAIC_SPEED_X                     0.0f       //!< Speed of the scrolling background horizontally
#define MOSAIC_SPEED_Y                     0.0f      //!< Speed of the scrolling background vertically

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CWinner::CWinner (void) : CModeScreen()
{
    // Initialize the pointers to NULL so that we
    // can easily detect the ones we forgot to set.
    m_pScores = NULL;
    m_pMatch = NULL;

    m_LightSpriteOffset = 0;
    m_HappyBomberSpriteOffset = 0;
    m_SadBomberSpriteOffset = 0;

    m_LightsTimer = 0;
    m_HappyBomberTimer = 0;
    m_SadBomberTimer = 0;

    m_PlayedSound = false;
    m_pMosaic = NULL;

    m_ModeTime = 0.0f;
    m_ExitModeTime = 0.0f;

    m_CoinTime = 0.0f;
    m_CoinSpriteOffset = 0;

    m_HaveToExit = false;
    m_ExitGameMode = GAMEMODE_NONE;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CWinner::~CWinner (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CWinner, you must create it.

void CWinner::Create (void)
{
    CModeScreen::Create();

    // Check if all the objects to communicate with are set
    ASSERT (m_pScores != NULL);
    ASSERT (m_pMatch != NULL);

    // Reset animation timers
    m_LightsTimer = 0.0f;
    m_HappyBomberTimer = 0.0f;
    m_SadBomberTimer = 0.0f;

    // Reset mode time (no time has been elapsed in this mode yet)
    m_ModeTime = 0.0f;

    // Reset coin animations
    m_CoinTime = 0.0f;
    m_CoinSpriteOffset = 0;

    // Don't have to exit this mode yet
    m_HaveToExit = false;

    m_LightSpriteOffset = 0;
    m_HappyBomberSpriteOffset = BOMBER_HAPPY_SPRITE_0;
    m_SadBomberSpriteOffset = BOMBER_SAD_SPRITE_0;

    // Update the winner player's score in the scores object
    for (int Player = 0; Player < MAX_PLAYERS; Player++)
    {
        if (m_pMatch->IsPlayerWinner(Player))
            m_pScores->RaisePlayerScore(Player);
    }

    m_PlayedSound = false;

    // Make a random green mosaic object
    m_pMosaic = CRandomMosaic::CreateRandomMosaic (m_pDisplay,
                                                   MOSAIC_SPRITE_LAYER,
                                                   MOSAIC_SPRITE_PRIORITY_IN_LAYER,
                                                   MOSAIC_SPEED_X,
                                                   MOSAIC_SPEED_Y,
                                                   MOSAICCOLOR_GREEN,
                                                   true);

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CWinner is not needed anymore, you should destroy it

void CWinner::Destroy (void)
{
    CModeScreen::Destroy();

    // Delete the scrolling mosaic background
    m_pMosaic->Destroy();
    delete m_pMosaic;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CWinner::OpenInput (void)
{
    m_pInput->GetMainInput().Open();

    // Open everyone's input device so the screen can be left with specific joystick button
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_pInput->GetPlayerInput(m_pOptions->GetPlayerInput(i)).Open();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CWinner::CloseInput (void)
{
    m_pInput->GetMainInput().Close();

    // Close everyone's input device
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        m_pInput->GetPlayerInput(m_pOptions->GetPlayerInput(i)).Close();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EGameMode CWinner::Update (void)
{
    // Increase elapsed time since mode has started
    m_ModeTime += m_pTimer->GetDeltaTime();

    // If we have to make the first black screen
    if (m_ModeTime <= WINNER_BLACKSCREEN_DURATION)
    {

    }
    // If minimum duration of the mode has not elapsed OR we don't have to exit yet
    else if (m_ModeTime <= WINNER_MINIMUM_DURATION || !m_HaveToExit)
    {
        // Update the scrolling mosaic background
        m_pMosaic->Update (m_pTimer->GetDeltaTime());

        //---------------------------------------
        // Play a sound if it hasn't started yet
        //---------------------------------------

        // If the winner sound hasn't started yet
        if (!m_PlayedSound)
        {
            // Start playing the winner sound
            m_pSound->PlaySample (SAMPLE_WINNER);

            // Don't do this more than once
            m_PlayedSound = true;
        }

        //---------------------------
        // Check if we have to exit
        //---------------------------

        // If minimum duration of the mode has elapsed
        if (m_ModeTime >= WINNER_MINIMUM_DURATION)
        {
            bool LeaveScreen = false;

            // Check everyone's joystick button if the 'next' button was pressed
            for ( int Player = 0; Player < MAX_PLAYERS; Player++ ) {

                // If this player plays and is a human
                if (m_pOptions->GetBomberType (Player) == BOMBERTYPE_MAN)
                {
                    // Get his player input using the options object
                    int PlayerInputNr = m_pOptions->GetPlayerInput (Player);

                    // If this player input is opened
                    if (m_pInput->GetPlayerInput(PlayerInputNr).IsOpened()) {
                        m_pInput->GetPlayerInput(PlayerInputNr).Update();
                        // LeaveScreen ||= resulted in error C2059
                        LeaveScreen |= m_pInput->GetPlayerInput(PlayerInputNr).TestMenuNext();
                    }
                }

            }

            // Check the keyboard as well
            LeaveScreen |= m_pInput->GetMainInput().TestNext();

            // If the NEXT control is active
            if (LeaveScreen)
            {
                // Remember we have to exit this mode
                m_HaveToExit = true;

                // Remember the mode time
                m_ExitModeTime = m_ModeTime;
            }
        }

        //---------------------------
        // Animate the lights
        //---------------------------

        // Set the sprite offset
             if (m_LightsTimer < LIGHTS_ANIMATION_TIME_0) m_LightSpriteOffset = 0;    // First light color
        else if (m_LightsTimer < LIGHTS_ANIMATION_TIME_1) m_LightSpriteOffset = 1;    // Second light color
        else if (m_LightsTimer < LIGHTS_ANIMATION_TIME_2) m_LightSpriteOffset = 2;    // Third light color
        else if (m_LightsTimer < LIGHTS_ANIMATION_TIME_3) m_LightSpriteOffset = 3;    // Fourth light color
        else
        {
            // Make a loop
            m_LightSpriteOffset = 0; // First light color again
            m_LightsTimer = 0.0f;
        }

        // Play animation
        m_LightsTimer += m_pTimer->GetDeltaTime();

        //-----------------------------
        // Animate happy bomber
        //-----------------------------

        // Set the sprite offset
             if (m_HappyBomberTimer < BOMBER_HAPPY_ANIMATION_TIME_0) m_HappyBomberSpriteOffset = BOMBER_HAPPY_SPRITE_0;
        else if (m_HappyBomberTimer < BOMBER_HAPPY_ANIMATION_TIME_1) m_HappyBomberSpriteOffset = BOMBER_HAPPY_SPRITE_1;
        else
        {
            // Make a loop
            m_HappyBomberSpriteOffset = BOMBER_HAPPY_SPRITE_0;
            m_HappyBomberTimer = 0.0f;
        }

        // Play animation
        m_HappyBomberTimer += m_pTimer->GetDeltaTime();

        //-----------------------------
        // Animate sad bomber
        //-----------------------------

        // Set the sprite offset
             if (m_SadBomberTimer < BOMBER_SAD_ANIMATION_TIME_0) m_SadBomberSpriteOffset = BOMBER_SAD_SPRITE_1;
        else if (m_SadBomberTimer < BOMBER_SAD_ANIMATION_TIME_1) m_SadBomberSpriteOffset = BOMBER_SAD_SPRITE_0;
        else if (m_SadBomberTimer < BOMBER_SAD_ANIMATION_TIME_2) m_SadBomberSpriteOffset = BOMBER_SAD_SPRITE_1;
        else if (m_SadBomberTimer < BOMBER_SAD_ANIMATION_TIME_3) m_SadBomberSpriteOffset = BOMBER_SAD_SPRITE_0;
        else
        {
            // Make a loop
            m_SadBomberSpriteOffset = BOMBER_SAD_SPRITE_1;
            m_SadBomberTimer = 0.0f;
        }

        // Play animation
        m_SadBomberTimer += m_pTimer->GetDeltaTime();

        //-----------------------------
        // Animate coin
        //-----------------------------

        if ( m_CoinSpriteOffset % COINS_SPRITE_COUNT == COINS_STATIC_SPRITE &&
            m_CoinSpriteOffset >= COINS_ANIMATION_TURNS * COINS_SPRITE_COUNT ) {
            // Don't animate coin any longer
        } else {
            // Animate
            m_CoinTime += m_pTimer->GetDeltaTime();
            // Increase the sprite index
            while ( m_CoinTime >= COINS_ANIMATION_TIME ) {
                m_CoinTime -= COINS_ANIMATION_TIME;
                // Don't increase the sprite index if the coin turns reached the limit and we
                // already arrived at the static sprite's index
                if ( m_CoinSpriteOffset % COINS_SPRITE_COUNT != COINS_STATIC_SPRITE ||
                    m_CoinSpriteOffset < COINS_ANIMATION_TURNS * COINS_SPRITE_COUNT ) {
                    m_CoinSpriteOffset ++;
                }
            }
        }
    }
    // The minimum mode duration has elapsed AND we have to exit,
    // so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= WINNER_BLACKSCREEN_DURATION)
    {

    }
    // Last black screen is complete! Get out of here!
    else
    {
        // If the score of the winner player is what's needed to be victorious
        for (int Player = 0; Player < MAX_PLAYERS; Player++)
        {
            if (m_pScores->GetPlayerScore(Player) == m_pOptions->GetBattleCount())
            {
                // Ask for a game mode switch to victory screen to congratulate the victorious player
                return GAMEMODE_VICTORY;
            }
        }

        // Ask for a game mode switch to match screen to continue the battle!
        return GAMEMODE_MATCH;

    }

    // Stay in this game mode
    return GAMEMODE_WINNER;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CWinner::Display (void)
{
    // If we have to make the first black screen
    if (m_ModeTime <= WINNER_BLACKSCREEN_DURATION)
    {
    }
    // If minimum duration of the mode has not elapsed OR we don't have to exit yet
    else if (m_ModeTime <= WINNER_MINIMUM_DURATION || !m_HaveToExit)
    {
        //---------------------------
        // Prepare displaying for the background
        //---------------------------

        // Set the position from which to display sprites
        m_pDisplay->SetOrigin (WINNER_DISPLAY_ORIGIN_X, WINNER_DISPLAY_ORIGIN_Y);

        //---------------------------
        // Display background
        //---------------------------

        // Display the scrolling mosaic background
        m_pMosaic->Display ();

        int Column;     // Used more than once in For loops
        int Row;        // Used more than once in For loops
        int Light;      // Light number used to browse through the light colors

        //---------------------------
        // Prepare displaying for all other elements
        //---------------------------

        // Set the position from which to display sprites
        m_pDisplay->SetOrigin (WINNER_DISPLAY_ORIGIN_X + WINNER_SPRITES_OFFSET_X, WINNER_DISPLAY_ORIGIN_Y + WINNER_SPRITES_OFFSET_Y);

        //--------------------------
        // Display SCOREBOARD title
        //--------------------------

        // Draw the big title
        m_pDisplay->DrawSprite (SCOREBOARD_TITLE_POSITION_X,
                                SCOREBOARD_TITLE_POSITION_Y,
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_WINNER_TITLE,
                                SCOREBOARD_SPRITE,
                                WINNER_SPRITE_LAYER,
                                WINNER_SCOREBOARD_PRIORITY);

        //----------------------------------
        // Display the full rows of lights
        //----------------------------------

        Light = 0;

        // For each light in a full row
        for (Column = 0 ; Column < LIGHTS_FULLROW_COUNT ; Column++)
        {
            // Draw one light for the first full row. The sprite number determines the light color.
            m_pDisplay->DrawSprite (LIGHTS_FULLROW1_POSITION_X + Column * LIGHTS_SPACE_X,
                                    LIGHTS_FULLROW1_POSITION_Y,
                                    NULL,                            // Draw entire sprite
                                    NULL,                            // No need to clip
                                    BMP_WINNER_LIGHTS,
                                    (m_LightSpriteOffset + Light) % LIGHTS_COLORS_COUNT,
                                    WINNER_SPRITE_LAYER,
                                    WINNER_LIGHTS_PRIORITY);

            // Draw one light for the second full row. The sprite number determines the light color.
            m_pDisplay->DrawSprite (LIGHTS_FULLROW2_POSITION_X + Column * LIGHTS_SPACE_X,
                                    LIGHTS_FULLROW2_POSITION_Y,
                                    NULL,                            // Draw entire sprite
                                    NULL,                            // No need to clip
                                    BMP_WINNER_LIGHTS,
                                    (m_LightSpriteOffset + Light) % LIGHTS_COLORS_COUNT,
                                    WINNER_SPRITE_LAYER,
                                    WINNER_LIGHTS_PRIORITY);

            // Draw one light for the third full row. The sprite number determines the light color.
            m_pDisplay->DrawSprite (LIGHTS_FULLROW3_POSITION_X + Column * LIGHTS_SPACE_X,
                                    LIGHTS_FULLROW3_POSITION_Y,
                                    NULL,                            // Draw entire sprite
                                    NULL,                            // No need to clip
                                    BMP_WINNER_LIGHTS,
                                    (m_LightSpriteOffset + Light) % LIGHTS_COLORS_COUNT,
                                    WINNER_SPRITE_LAYER,
                                    WINNER_LIGHTS_PRIORITY);

            // One more light! This allows to scan the colors.
            Light++;
        }

        //-------------------------------------
        // Display the full columns of lights
        //-------------------------------------

        Light = 0;

        // For each light in a full column
        for (Row = 0 ; Row < LIGHTS_FULLCOLUMN_COUNT ; Row++)
        {
            // Draw one light for the first full column. The sprite number determines the light color.
            m_pDisplay->DrawSprite (LIGHTS_FULLCOLUMN1_POSITION_X,
                                    LIGHTS_FULLCOLUMN1_POSITION_Y + Row * LIGHTS_SPACE_Y,
                                    NULL,                            // Draw entire sprite
                                    NULL,                            // No need to clip
                                    BMP_WINNER_LIGHTS,
                                    (m_LightSpriteOffset + Light) % LIGHTS_COLORS_COUNT,
                                    WINNER_SPRITE_LAYER,
                                    WINNER_LIGHTS_PRIORITY);

            // Draw one light for the second full column. The sprite number determines the light color.
            m_pDisplay->DrawSprite (LIGHTS_FULLCOLUMN2_POSITION_X,
                                    LIGHTS_FULLCOLUMN2_POSITION_Y + Row * LIGHTS_SPACE_Y,
                                    NULL,                            // Draw entire sprite
                                    NULL,                            // No need to clip
                                    BMP_WINNER_LIGHTS,
                                    (m_LightSpriteOffset + Light) % LIGHTS_COLORS_COUNT,
                                    WINNER_SPRITE_LAYER,
                                    WINNER_LIGHTS_PRIORITY);

            // One more light! This allows to scan the colors.
            Light++;
        }

        //------------------------------------
        // Display the semi column of lights
        //------------------------------------

        Light = 0;

        // For each light in the semi column
        for (Row = 0 ; Row < LIGHTS_SEMICOLUMN_COUNT ; Row++)
        {
            // Draw one light for the semi column. The sprite number determines the light color.
            m_pDisplay->DrawSprite (LIGHTS_SEMICOLUMN_POSITION_X,
                                    LIGHTS_SEMICOLUMN_POSITION_Y + Row * LIGHTS_SPACE_Y,
                                    NULL,                            // Draw entire sprite
                                    NULL,                            // No need to clip
                                    BMP_WINNER_LIGHTS,
                                    (m_LightSpriteOffset + Light) % LIGHTS_COLORS_COUNT,
                                    WINNER_SPRITE_LAYER,
                                    WINNER_LIGHTS_PRIORITY);

            // One more light! This allows to scan the colors.
            Light++;
        }

        //--------------------------------------------------------------------------
        // Display the bombers (or a cross if bomber does not play) and their coins
        //--------------------------------------------------------------------------

        // Scan the players
        for (int Player = 0 ; Player < MAX_PLAYERS ; Player++)
        {
            // If the bomber plays
            if (m_pOptions->GetBomberType(Player) != BOMBERTYPE_OFF)
            {
                // Determine the bomber sprite. It depends on the bomber color and on its happiness.
                // The bomber is happy if he just won the match.
                int BomberSprite = Player * BOMBER_SPRITES_COUNT_PER_COLOR      // Determines the color
                                 + (m_pMatch->IsPlayerWinner(Player) ?     // Determines happiness
                                       m_HappyBomberSpriteOffset : m_SadBomberSpriteOffset);

                // Draw the bomber head
                m_pDisplay->DrawSprite (BOMBER_INITIAL_POSITION_X,
                                        BOMBER_INITIAL_POSITION_Y + Player * BOMBER_SPACE_Y,
                                        NULL,                            // Draw entire sprite
                                        NULL,                            // No need to clip
                                        BMP_WINNER_BOMBER,
                                        BomberSprite,       // Appropriate bomber sprite (color + happiness)
                                        WINNER_SPRITE_LAYER,
                                        WINNER_BOMBER_PRIORITY);

                // Draw as many coins as the player score
                for (int Coin = 0 ; Coin < m_pScores->GetPlayerScore(Player) ; Coin++)
                {
                    // Don't animate coin by default (use the static sprite's index)
                    int currentCoinSprite = COINS_STATIC_SPRITE;
                    // Animate coin only if it is the last coin and this is the winning player
                    if (Coin + 1 == m_pScores->GetPlayerScore(Player) && m_pMatch->IsPlayerWinner(Player)) {
                        // m_CoinSpriteOffset is always increased, so we use the modulo operator
                        currentCoinSprite = m_CoinSpriteOffset % COINS_SPRITE_COUNT;
                    }
                    // Draw the coin
                    m_pDisplay->DrawSprite (COINS_INITIAL_POSITION_X + Coin * COINS_SPACE_X,
                                            COINS_INITIAL_POSITION_Y + Player * COINS_SPACE_Y,
                                            NULL,                            // Draw entire sprite
                                            NULL,                            // No need to clip
                                            BMP_WINNER_COIN,
                                            currentCoinSprite,
                                            WINNER_SPRITE_LAYER,
                                            WINNER_COIN_PRIORITY);
                }
            }
            // If the bomber does not play
            else
            {
                // Draw a cross sprite of the color of the bomber
                m_pDisplay->DrawSprite (BOMBER_INITIAL_POSITION_X + CROSS_SPACE_X,
                                        BOMBER_INITIAL_POSITION_Y + CROSS_SPACE_Y + Player * BOMBER_SPACE_Y,
                                        NULL,                            // Draw entire sprite
                                        NULL,                            // No need to clip
                                        BMP_WINNER_CROSS,
                                        Player,       // Appropriate color
                                        WINNER_SPRITE_LAYER,
                                        WINNER_CROSS_PRIORITY);
            }
        }
    }
    // The minimum mode duration has elapsed AND we have to exit,
    // so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= WINNER_BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
