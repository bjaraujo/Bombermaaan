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
 *  \file CVictory.cpp
 *  \brief The victory screen (after a player has won all matches)
 */

#include "StdAfx.h"
#include "CVictory.h"
#include "CGame.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Duration (in seconds) of the two black screens
#define VICTORY_BLACKSCREEN_DURATION    0.750f

// Duration of the victory screen
#define VICTORY_MINIMUM_DURATION        0.0f
#define VICTORY_SCREEN_DURATION         10.0f

// The width of the victory view
#define VICTORY_VIEW_WIDTH              (15*32)     // The width as in the 16-pixels version
#define VICTORY_VIEW_HEIGHT             (26+13*32)  // The height as in the 16-pixels version

// Display origin                               
#define VICTORY_DISPLAY_ORIGIN_X        0       //!< Display origin of the victory screen
#define VICTORY_DISPLAY_ORIGIN_Y        0       
                                                
// Background
#define VICTORYWALL_TILE_SPRITE         0       //!< Wall tile sprite number
#define VICTORYWALL_TILES_COUNT         16       //!< How many wall tiles to draw?
#define VICTORYWALL_TILES_INITIAL_X     -9      //!< Position where to draw the first wall tile
#define VICTORYWALL_TILES_INITIAL_Y     100
#define VICTORYWALL_TILE_SPACE_X        32      //!< Space (in pixels) between two wall tile X positions

// Crowd
#define CROWD_TILES_COUNT_X         46                          //!< How many crowd tiles to draw in a row
#define CROWD_TILES_COUNT_Y         8                           //!< How many crowd tiles to draw in a column
#define CROWD_STATES_COUNT          2                           //!< How many states can the crowd have
#define CROWD_OFFSET_GETUP          -1                          //!< Y position offset to use for bombers that get up
#define CROWD_OFFSET_SITDOWN        1                           //!< Y position offset to use for bombers that sit down
#define CROWD_OFFSET_MOVING         0                           //!< Y position offset to use for bombers that change position (getting up or sitting down)
#define CROWD_INITIAL_TILE_X        -4                          //!< Position of the first crowd tile (up left corner)
#define CROWD_INITIAL_TILE_Y        0                           
#define CROWD_TILE_SIZE_X           14                          //!< Size (in pixels) of one crowd tile
#define CROWD_TILE_SIZE_Y           16                          
#define CROWD_TILES_SPACE_X         (CROWD_TILE_SIZE_X - 3)     //!< Space (in pixels) between two crowd tiles X positions
#define CROWD_TILES_SPACE_Y         (CROWD_TILE_SIZE_Y - 4)     
#define CROWD_ANIMATION_TIME_0      0.300f                      //!< Crowd animation times
#define CROWD_ANIMATION_TIME_1      CROWD_ANIMATION_TIME_0 * 2
#define CROWD_COLORS_COUNT          9                           //!< How many bomber colors can we have
#define MEXICAN_WAVE_ANIMATION_TIME 0.07f
       
// Bombers
#define WINNER_BOMBER_ANIMATION_TIME_0      2.000f      //!< Victorious bomber animation times
#define WINNER_BOMBER_ANIMATION_TIME_1      WINNER_BOMBER_ANIMATION_TIME_0 + 0.250f
#define WINNER_BOMBER_ANIMATION_TIME_2      WINNER_BOMBER_ANIMATION_TIME_1 + 0.040f
#define WINNER_BOMBER_ANIMATION_TIME_3      WINNER_BOMBER_ANIMATION_TIME_2 + 0.030f
#define WINNER_BOMBER_ANIMATION_TIME_4      WINNER_BOMBER_ANIMATION_TIME_3 + 0.030f
#define WINNER_BOMBER_ANIMATION_TIME_5      WINNER_BOMBER_ANIMATION_TIME_4 + 0.030f
#define WINNER_BOMBER_ANIMATION_TIME_6      WINNER_BOMBER_ANIMATION_TIME_5 + 0.030f
#define WINNER_BOMBER_ANIMATION_TIME_7      WINNER_BOMBER_ANIMATION_TIME_6 + 0.030f
#define WINNER_BOMBER_ANIMATION_TIME_8      WINNER_BOMBER_ANIMATION_TIME_7 + 0.040f
#define WINNER_BOMBER_ANIMATION_TIME_9      WINNER_BOMBER_ANIMATION_TIME_8 + 0.050f
#define WINNER_BOMBER_ANIMATION_TIME_10     WINNER_BOMBER_ANIMATION_TIME_9 + 0.250f
#define WINNER_BOMBER_ANIMATION_TIME_11     WINNER_BOMBER_ANIMATION_TIME_10 + 0.300f  //!< Sprite before the last sprite (last sprite is infinite)
#define WINNER_BOMBER_SPRITE_0              0           //!< Sprite number for the victorious bomber
#define WINNER_BOMBER_SPRITE_1              1           //!< (you have to add the color offset to get the
#define WINNER_BOMBER_SPRITE_2              2           //!<  desired bomber color)
#define WINNER_BOMBER_SPRITE_3              3
#define WINNER_BOMBER_SPRITE_4              4
#define WINNER_BOMBER_SPRITE_5              5
#define WINNER_BOMBER_SPRITE_6              6
#define WINNER_BOMBER_SPRITE_7              7
#define WINNER_BOMBER_SPRITE_8              8
#define WINNER_BOMBER_SPRITE_9              9
#define WINNER_BOMBER_SPRITE_10             10
#define LOSER_BOMBER_ANIMATION_TIME_0       4.000f
#define LOSER_BOMBER_ANIMATION_TIME_1       4.100f      //!< Sprite before the last sprite (last sprite is infinite)
#define LOSER_BOMBER_SPRITE_0               11          //!< Sprite number for the bombers that lost
#define LOSER_BOMBER_SPRITE_1               12          //!< (you have to add the color offset to get the
#define LOSER_BOMBER_SPRITE_2               13          //!<  desired bomber color)
#define LOSER_BOMBER_SPACE_X                30          //!< Space (in pixels) between two losers X positions
#define WINNER_BOMBER_SPACE_X               30          //!< Space (in pixels) between two winners X positions
#define LOSER_BOMBER_SPACE_EDGE             10          //!< Space (in pixels) between the right edge and the loser to the right
#define WINNER_BOMBER_POSITION_Y            220         //!< Y Position where to draw winner sprites
#define LOSER_BOMBER_POSITION_Y             120         //!< Y Position where to draw loser sprites
#define BOMBER_SPRITES_COUNT_PER_COLOR      14          //!< How many bomber sprites per color?

// Confettis
#define CONFETTI_LIMIT_LEFT     -20.0f
#define CONFETTI_LIMIT_RIGHT    float (VIEW_WIDTH + 10)
#define CONFETTI_LIMIT_BOTTOM   float (VIEW_HEIGHT + 10)
#define CONFETTI_ANIMATION_TIME_0       1 * 0.200f      //!< Confetti animation times
#define CONFETTI_ANIMATION_TIME_1       2 * 0.200f
#define CONFETTI_ANIMATION_TIME_2       3 * 0.200f
#define CONFETTI_ANIMATION_TIME_3       4 * 0.200f
#define CONFETTI_ANIMATION_TIME_4       5 * 0.200f
#define CONFETTI_ANIMATION_SPRITE_0     0               //!< Confetti animation frames sprites numbers
#define CONFETTI_ANIMATION_SPRITE_1     1
#define CONFETTI_ANIMATION_SPRITE_2     2
#define CONFETTI_ANIMATION_SPRITE_3     3
#define CONFETTI_ANIMATION_SPRITE_4     4
#define CONFETTI_RESET_POSITION_X       float (RANDOM(VIEW_WIDTH))  //!< Position to set when resetting the confetti
#define CONFETTI_RESET_POSITION_Y       -20.0f
#define CONFETTI_RESET_SPEED_X          float (RANDOM(110)-70)      //!< Speed to set when resetting the confetti
#define CONFETTI_RESET_SPEED_Y          float (RANDOM(100)+40)
#define CONFETTIS_COUNT_PER_COLOR       5               //!< How many confettis per color?

// VICTORY! Title
#define VICTORY_TITLE_SPRITE            0       //!< Sprite number of the title
#define VICTORY_TITLE_POSITION_X        25      //!< Position where to draw the title sprite
#define VICTORY_TITLE_POSITION_Y        11

// Sprite layer & priority                  
#define VICTORY_CROWD_LAYER             0       //!< Sprite layer where to draw the crowd sprites
#define VICTORY_WALL_LAYER              1       //!< Sprite layer where to draw the victory wall tiles sprites
#define VICTORY_BOMBER_LAYER            2       //!< Sprite layer where to draw the bomber sprites
#define VICTORY_CONFETTIS_LAYER         3       //!< Sprite layer where to draw the confettis sprites
#define VICTORY_TITLE_LAYER             4       //!< Sprite layer where to draw the VICTORY! title sprite

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CVictory::CVictory (void) : CModeScreen()
{
    // Initialize the pointers to NULL so that we 
    // can easily detect the ones we forgot to set.
    m_pScores = NULL;
    
    m_ModeTime = 0.0f;

    m_CrowdTimer = 0.0f;
    m_WinnerBomberTimer = 0.0f;
    m_LoserBomberTimer = 0.0f;
    m_MexicanWaveTimer = 0.0f;
    m_MexicanWavePosition = -1;

    m_CrowdWaveMode = ECrowdWave( RANDOM( NUMBER_CROWDWAVES ) );

    m_HaveToExit = false;
    m_ExitModeTime = 0.0f;

    m_PlayedSound = false;

    m_CrowdFlag = false;

    m_WinnerBomberSprite = 0;
    m_LoserBomberSprite = 0;

    for (int i = 0; i < NUM_CONFETTIS; i++)
    {
        m_Confettis[i] = SConfetti();
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CVictory::~CVictory (void)
{
    // Nothing to do!
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Before using a CVictory, you must create it.

void CVictory::Create (void)
{
    CModeScreen::Create();

    // Check if all the objects to communicate with are set
    ASSERT (m_pScores != NULL);

    // No time has been spent in this mode yet
    m_ModeTime = 0.0f;

    // Reset animation timers
    m_CrowdTimer = 0.0f;
    m_WinnerBomberTimer = 0.0f;
    m_LoserBomberTimer = 0.0f;
    m_MexicanWaveTimer = 0.0f;
    m_MexicanWavePosition = -1;

    // Choose a random wave for the crowd
    m_CrowdWaveMode = ECrowdWave( RANDOM( NUMBER_CROWDWAVES ) );

    // Don't have to exit this mode yet
    m_HaveToExit = false;
    m_ExitModeTime = 0.0f;

    m_PlayedSound = false;

    // Index in the confettis array
    int Confetti = 0;

    // Initialize the confetti type for large confettis
    while (Confetti < NUM_CONFETTIS_LARGE)
    {
        m_Confettis[Confetti].Type = CONFETTI_LARGE;
        Confetti++;
    }

    // Initialize the confetti type for medium confettis
    while (Confetti < NUM_CONFETTIS_LARGE + NUM_CONFETTIS_MEDIUM)
    {
        m_Confettis[Confetti].Type = CONFETTI_MEDIUM;
        Confetti++;
    }

    // Initialize the confetti type for small confettis
    while (Confetti < NUM_CONFETTIS_LARGE + NUM_CONFETTIS_MEDIUM + NUM_CONFETTIS_SMALL)
    {
        m_Confettis[Confetti].Type = CONFETTI_MEDIUM;
        Confetti++;
    }

    // Reset all confettis
    for (Confetti = 0 ; Confetti < NUM_CONFETTIS ; Confetti++)
    {
        ResetConfetti (&m_Confettis[Confetti]);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// When a CVictory is not needed anymore, you should destroy it

void CVictory::Destroy (void)
{
    CModeScreen::Destroy();
    StopSong();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVictory::OpenInput (void)
{
    m_pInput->GetMainInput().Open();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVictory::CloseInput (void)
{
    m_pInput->GetMainInput().Close();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVictory::ResetConfetti (SConfetti* pConfetti)
{
    // Reset confetti animation
    pConfetti->AnimationTimer = 0.0f;
    
    // Set the confetti on the top of the game view (and OUT of the game view)
    // and on a random place on the X axis
    pConfetti->PositionX = CONFETTI_RESET_POSITION_X;
    pConfetti->PositionY = CONFETTI_RESET_POSITION_Y;
    
    // Set a random speed for the confetti. The confetti must fall down of course...
    pConfetti->SpeedX = CONFETTI_RESET_SPEED_X;
    pConfetti->SpeedY = CONFETTI_RESET_SPEED_Y;

    // Fill sprite number
    pConfetti->Sprite = CONFETTI_ANIMATION_SPRITE_0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

EGameMode CVictory::Update (void)
{
    // Increase elapsed time since mode has started
    m_ModeTime += m_pTimer->GetDeltaTime();

    // If we have to make the first black screen
    if (m_ModeTime <= VICTORY_BLACKSCREEN_DURATION)
    {
        
    }
    // If we don't have to exit yet
    else if (!m_HaveToExit)
    {
        //---------------------------------------
        // Play a sound if it hasn't started yet
        //---------------------------------------

        // If the victory sound hasn't started yet
        if (!m_PlayedSound)
        {
            // Start playing the victory sound
            m_pSound->PlaySample (SAMPLE_VICTORY);
            m_pSound->PlaySample (SAMPLE_VICTORY_VOICE);

            // Don't do this more than once
            m_PlayedSound = true;
        }

        // If minimum duration of the mode has elapsed
        if (m_ModeTime >= VICTORY_MINIMUM_DURATION)
        {
            bool LeaveScreen = false;

            // Check everyone's joystick button if the 'next' button was pressed
            for (int Player = 0; Player < MAX_PLAYERS; Player++) {

                // If this player plays and is a human
                if (m_pOptions->GetBomberType(Player) == BOMBERTYPE_MAN)
                {
                    // Get his player input using the options object
                    int PlayerInputNr = m_pOptions->GetPlayerInput(Player);

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
        // Check if we have to exit
        //---------------------------
        
        if (m_ModeTime >= VICTORY_BLACKSCREEN_DURATION + VICTORY_SCREEN_DURATION)
        {
            // Remember we have to exit this mode
            m_HaveToExit = true;
            // Remember the mode time
            m_ExitModeTime = m_ModeTime;
        }
        
        //---------------------------
        // Animate the crowd (classic style)
        //---------------------------

        if ( m_CrowdWaveMode == CROWDWAVE_CLASSIC ) {

            // Set the crowd state
                 if (m_CrowdTimer < CROWD_ANIMATION_TIME_0)   m_CrowdFlag = true;
            else if (m_CrowdTimer < CROWD_ANIMATION_TIME_1)   m_CrowdFlag = false;
            else
            {
                // Make a loop
                m_CrowdTimer = 0.0f;
                m_CrowdFlag = true;
            }

            // Play animation
            m_CrowdTimer += m_pTimer->GetDeltaTime();

        }

        //---------------------------
        // Animate the crowd (Mexican wave)
        //---------------------------

        else if ( m_CrowdWaveMode == CROWDWAVE_MEXICAN ) {

            if ( m_MexicanWaveTimer > MEXICAN_WAVE_ANIMATION_TIME ) {
                // The next row should be the center of the wave
                m_MexicanWavePosition ++;
                // The end reached?
                if ( m_MexicanWavePosition > CROWD_TILES_COUNT_X ) {
                    // Start the wave again from the left
                    // The lower set (negative values allowed) the later starts the wave again
                    m_MexicanWavePosition = -5;
                }
                m_MexicanWaveTimer = 0.0f;
            }

            // Play animation
            m_MexicanWaveTimer += m_pTimer->GetDeltaTime();

        }

        //-----------------------------------
        // Animate the victorious bomber
        //-----------------------------------

        // Set the sprite offset to save the current state of the victorious bomber
             if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_0)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_0;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_1)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_1;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_2)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_2;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_3)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_3;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_4)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_4;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_5)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_5;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_6)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_6;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_7)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_7;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_8)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_8;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_9)   m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_9;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_10)  m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_1;
        else if (m_WinnerBomberTimer < WINNER_BOMBER_ANIMATION_TIME_11)  m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_0;
        else
        {
            // Don't loop the animation! Set the last state
            m_WinnerBomberSprite = WINNER_BOMBER_SPRITE_10;
        }

        // Play animation
        m_WinnerBomberTimer += m_pTimer->GetDeltaTime();

        //-----------------------------------
        // Animate the bombers that lost
        //-----------------------------------

        // Set the sprite offset to save the current state of the loser
             if (m_LoserBomberTimer < LOSER_BOMBER_ANIMATION_TIME_0)   m_LoserBomberSprite = LOSER_BOMBER_SPRITE_0;
        else if (m_LoserBomberTimer < LOSER_BOMBER_ANIMATION_TIME_1)   m_LoserBomberSprite = LOSER_BOMBER_SPRITE_1;
        else
        {
            // Don't loop the animation! Set the last state
            m_LoserBomberSprite = LOSER_BOMBER_SPRITE_2;
        }

        // Play animation
        m_LoserBomberTimer += m_pTimer->GetDeltaTime();

        //-----------------------------------------
        // Update the confettis (move and animate)
        //-----------------------------------------

        // Scan the confettis to manage
        for (int Confetti = 0 ; Confetti < NUM_CONFETTIS ; Confetti++)
        {
            // Make the confetti move according to its speed
            m_Confettis[Confetti].PositionX += m_Confettis[Confetti].SpeedX * m_pTimer->GetDeltaTime ();
            m_Confettis[Confetti].PositionY += m_Confettis[Confetti].SpeedY * m_pTimer->GetDeltaTime ();
        
            // If the confetti is now out of the game view
            if (m_Confettis[Confetti].PositionX < CONFETTI_LIMIT_LEFT ||
                m_Confettis[Confetti].PositionX > CONFETTI_LIMIT_RIGHT ||
                m_Confettis[Confetti].PositionY > CONFETTI_LIMIT_BOTTOM)
            {
                // Reset the confetti so that it starts again at the top, as a new and different confetti
                ResetConfetti (&m_Confettis[Confetti]);
            }
            // If the confetti is not out of the game view
            else
            {
                // Set the current animation sprite
                     if (m_Confettis[Confetti].AnimationTimer < CONFETTI_ANIMATION_TIME_0)  m_Confettis[Confetti].Sprite = CONFETTI_ANIMATION_SPRITE_0;
                else if (m_Confettis[Confetti].AnimationTimer < CONFETTI_ANIMATION_TIME_1)  m_Confettis[Confetti].Sprite = CONFETTI_ANIMATION_SPRITE_1;
                else if (m_Confettis[Confetti].AnimationTimer < CONFETTI_ANIMATION_TIME_2)  m_Confettis[Confetti].Sprite = CONFETTI_ANIMATION_SPRITE_2;
                else if (m_Confettis[Confetti].AnimationTimer < CONFETTI_ANIMATION_TIME_3)  m_Confettis[Confetti].Sprite = CONFETTI_ANIMATION_SPRITE_3;
                else if (m_Confettis[Confetti].AnimationTimer < CONFETTI_ANIMATION_TIME_4)  m_Confettis[Confetti].Sprite = CONFETTI_ANIMATION_SPRITE_4;
                else
                {
                    // Loop animation
                    m_Confettis[Confetti].AnimationTimer = 0.0f;
                    m_Confettis[Confetti].Sprite = CONFETTI_ANIMATION_SPRITE_0;
                }

                // Play the confetti animation
                m_Confettis[Confetti].AnimationTimer += m_pTimer->GetDeltaTime ();
            }
        }
    }
    // The minimum mode duration has elapsed AND we have to exit, 
    // so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= VICTORY_BLACKSCREEN_DURATION)
    {
        
    }
    // Last black screen is complete! Get out of here!
    else
    {
        // All scores to zero
        m_pScores->Reset ();

        // Ask for a game mode switch to menu screen
        return GAMEMODE_MENU;
    }

    // Stay in this game mode
    return GAMEMODE_VICTORY;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVictory::Display (void)
{
    // If we have to make the first black screen
    if (m_ModeTime <= VICTORY_BLACKSCREEN_DURATION)
    {
    }
    // If we don't have to exit yet
    else if (!m_HaveToExit)
    {
        //---------------------------
        // Prepare displaying
        //---------------------------

        // Set the position from which to display sprites
        m_pDisplay->SetOrigin (VICTORY_DISPLAY_ORIGIN_X, VICTORY_DISPLAY_ORIGIN_Y);

        // We use sprite clipping in this mode for the crowd, the victory wall and confettis.
        // Therefore prepare a clip rect of the dimensions of the game view.
        RECT Clip;
        Clip.left = 0;
        Clip.top = 0;
        Clip.right = VICTORY_VIEW_WIDTH;
        Clip.bottom = VICTORY_VIEW_HEIGHT;

        //---------------------------
        //  Display the crowd
        //---------------------------

        int TileX;
        int TileY;

        // Sprite number determining the color of the bomber in the crow tile.
        int Color = 0;

        // For each crowd tiles row to draw
        for (TileX = 0 ; TileX < CROWD_TILES_COUNT_X ; TileX++)
        {
            // For each crowd tiles column to draw
            for (TileY = 0 ; TileY < CROWD_TILES_COUNT_Y ; TileY++)
            {
                // Y offset to apply to crow tile position in order to 
                // get a bomber that gets up or sits down.
                int OffsetY;
            
                if ( m_CrowdWaveMode == CROWDWAVE_CLASSIC ) {

                    // The trick to determine the state is to make the modulo
                    // of the sum of column number + row number. We then get
                    // something like this :
                    // 0101010
                    // 1010101
                    // 0101010
                
                    // If this bomber is in the first crowd state
                    if (((TileX + TileY) % CROWD_STATES_COUNT) == 0)
                    {
                        // Set the Y offset
                        OffsetY = (m_CrowdFlag ? CROWD_OFFSET_GETUP : CROWD_OFFSET_SITDOWN);
                    }
                    // If this bomber is in the second crowd state
                    else
                    {
                        // Set the Y offset (inversed meaning of the flag value)
                        OffsetY = (m_CrowdFlag ? CROWD_OFFSET_SITDOWN : CROWD_OFFSET_GETUP);
                    }

                } else if ( m_CrowdWaveMode == CROWDWAVE_MEXICAN ) {

                    // The Mexican wave:
                    // The bombers of the current position are standing (CROWD_OFFSET_GETUP), while the next
                    // and previous bombers are changing their position (CROWD_OFFSET_MOVING).
                    // The other bombers are sitting (CROWD_OFFSET_SITDOWN)

                    if ( m_MexicanWavePosition == TileX ) {
                        OffsetY = CROWD_OFFSET_GETUP;
                    } else if ( TileX == m_MexicanWavePosition - 1 || TileX == m_MexicanWavePosition + 1 ) {
                        OffsetY = CROWD_OFFSET_MOVING;
                    } else {
                        OffsetY = CROWD_OFFSET_SITDOWN;
                    }

                } else if ( m_CrowdWaveMode == CROWDWAVE_NONE ) {

                    // The crowd doesn't move at all
                    OffsetY = CROWD_OFFSET_SITDOWN;

                } else {

                    // Should never happen
                    OffsetY = CROWD_OFFSET_SITDOWN;
                    ASSERT( false );

                }

                // Draw the crowd tile
                m_pDisplay->DrawSprite (CROWD_INITIAL_TILE_X + TileX * CROWD_TILES_SPACE_X, 
                                        CROWD_INITIAL_TILE_Y + TileY * CROWD_TILES_SPACE_Y + OffsetY, 
                                        NULL,                            // Draw entire sprite
                                        &Clip,                           // Clip to game view
                                        BMP_VICTORY_CROWD, 
                                        Color % CROWD_COLORS_COUNT, 
                                        VICTORY_CROWD_LAYER,
                                        TileY);
            }

            // Scan the colors every two tiles column
            if ((TileX+1) % 2 == 0) Color++;
        }

        //--------------------------
        // Display the victory wall
        //--------------------------

        for (TileX = 0 ; TileX < VICTORYWALL_TILES_COUNT ; TileX++)
        {
            // Draw one background sprite tile
            m_pDisplay->DrawSprite (VICTORYWALL_TILES_INITIAL_X + TileX * VICTORYWALL_TILE_SPACE_X, 
                                    VICTORYWALL_TILES_INITIAL_Y, 
                                    NULL,                            // Draw entire sprite
                                    &Clip,                           // Clip to game view
                                    BMP_VICTORY_WALL, 
                                    VICTORYWALL_TILE_SPRITE, 
                                    VICTORY_WALL_LAYER,
                                    PRIORITY_UNUSED);
         }

        //---------------------------------------------
        // Display the bombers (winners and losers) *** COULD BE COMPUTED ONLY ONCE
        //---------------------------------------------

        int Player;
        int WinnerBombersCount = 0;     // How many bombers are victorious?
        int LoserBombersCount = 0;      // How many bombers have lost?

        // Scan the players
        for (Player = 0 ; Player < MAX_PLAYERS ; Player++)
        {
            // If this bomber plays then it has to be drawn
            if (m_pOptions->GetBomberType(Player) != BOMBERTYPE_OFF)
            {
                // If this bomber is a victorious bomber
                if (m_pScores->GetPlayerScore(Player) == m_pOptions->GetBattleCount())
                {
                    // One more winner
                    WinnerBombersCount++;
                }
                // If this bomber has lost the battle
                else
                {
                    // One more loser
                    LoserBombersCount++;
                }
            }
        }

        // Position of the first (left) loser sprite to draw and 
        // position of the first (left) winner sprite to draw.
        // Here is how this is computed :
        // Loser bombers are all aligned to the right of the view. They take some place on the view
        // (and there has to be a space between the bomber to the the right and the right border of 
        // the view), and the winners (which take some place on the view too) must center in 
        // the remaining place of the view.
        int LoserInitialX = VICTORY_VIEW_WIDTH - (LoserBombersCount * LOSER_BOMBER_SPACE_X + LOSER_BOMBER_SPACE_EDGE);
        int WinnerInitialX = (LoserInitialX - WinnerBombersCount * WINNER_BOMBER_SPACE_X) / 2;       
    
        // We have to count again while scanning the players in order 
        // to compute the position of the bomber sprite.
        WinnerBombersCount = 0;     // How many bombers are victorious?
        LoserBombersCount = 0;      // How many bombers have lost?

        // Scan the players
        for (Player = 0 ; Player < MAX_PLAYERS ; Player++)
        {
            // If this bomber plays then it has to be drawn
            if (m_pOptions->GetBomberType(Player) != BOMBERTYPE_OFF)
            {
                // If this bomber is a victorious bomber
                if (m_pScores->GetPlayerScore(Player) == m_pOptions->GetBattleCount())
                {
                    // Draw the victorious bomber sprite
                    // Sprite number is computed using winner sprite offset, which allows to select
                    // the current frame in the winner animation ; and using the color offset, which
                    // allows to select the bomber color.
                    m_pDisplay->DrawSprite (WinnerInitialX + WinnerBombersCount * WINNER_BOMBER_SPACE_X, 
                                            WINNER_BOMBER_POSITION_Y, 
                                            NULL,                            // Draw entire sprite
                                            NULL,                            // No need to clip
                                            BMP_VICTORY_BOMBER, 
                                            m_WinnerBomberSprite + Player * BOMBER_SPRITES_COUNT_PER_COLOR, 
                                            VICTORY_BOMBER_LAYER,
                                            PRIORITY_UNUSED);
                 
                    // One more winner
                    WinnerBombersCount++;
                }
                // If this bomber has lost the battle
                else
                {
                    // Draw the loser sprite
                    // Sprite number is computed using loser sprite offset, which allows to select
                    // the current frame in the loser animation ; and using the color offset, which
                    // allows to select the bomber color.
                    m_pDisplay->DrawSprite (LoserInitialX + LoserBombersCount * LOSER_BOMBER_SPACE_X, 
                                            LOSER_BOMBER_POSITION_Y, 
                                            NULL,                            // Draw entire sprite
                                            NULL,                            // No need to clip
                                            BMP_VICTORY_BOMBER, 
                                            m_LoserBomberSprite + Player * BOMBER_SPRITES_COUNT_PER_COLOR, 
                                            VICTORY_BOMBER_LAYER,
                                            PRIORITY_UNUSED);

                    // One more loser
                    LoserBombersCount++;
                }
            }
        }

        //------------------------------------------------------------
        // Determine the winner player to know the color of confettis *** COULD BE COMPUTED ONLY ONCE
        //------------------------------------------------------------

        int WinnerPlayer = 0;

        // Scan the players
        for (Player = 0 ; Player < MAX_PLAYERS ; Player++)
        {
            // If this bomber plays then it has to be drawn
            if (m_pOptions->GetBomberType(Player) != BOMBERTYPE_OFF)
            {
                // If this bomber is a victorious bomber
                if (m_pScores->GetPlayerScore(Player) == m_pOptions->GetBattleCount())
                {
                    WinnerPlayer = Player;
                    break;
                }
            }
        }

        //-----------------------
        // Display the confettis
        //-----------------------

        // Scan the confettis to manage
        for (int Confetti = 0 ; Confetti < NUM_CONFETTIS ; Confetti++)
        {
            // Make the confetti move according to its speed
            m_Confettis[Confetti].PositionX += m_Confettis[Confetti].SpeedX * m_pTimer->GetDeltaTime ();
            m_Confettis[Confetti].PositionY += m_Confettis[Confetti].SpeedY * m_pTimer->GetDeltaTime ();
        
            int ConfettiSpriteTable = -1;
                
            // Save the sprite table to use according to the type of the confetti
            switch (m_Confettis[Confetti].Type)
            {
                case CONFETTI_LARGE  : ConfettiSpriteTable = BMP_VICTORY_CONFETTIS_LARGE;  break;
                case CONFETTI_MEDIUM : ConfettiSpriteTable = BMP_VICTORY_CONFETTIS_MEDIUM; break;
                case CONFETTI_SMALL  : ConfettiSpriteTable = BMP_VICTORY_CONFETTIS_SMALL;  break;
            }

            ASSERT (ConfettiSpriteTable != -1);
            
            // Draw the confetti sprite
            m_pDisplay->DrawSprite ((int) m_Confettis[Confetti].PositionX,
                                    (int) m_Confettis[Confetti].PositionY,
                                    NULL,                            // Draw entire sprite
                                    &Clip,                           // Clip to game view
                                    ConfettiSpriteTable,
                                    WinnerPlayer * CONFETTIS_COUNT_PER_COLOR + m_Confettis[Confetti].Sprite,  // Get the confetti sprite of the color of the winner
                                    VICTORY_CONFETTIS_LAYER,
                                    PRIORITY_UNUSED);
        }

        //-------------------------------
        // Display the VICTORY! title
        //-------------------------------

        m_pDisplay->DrawSprite (VICTORY_TITLE_POSITION_X,
                                VICTORY_TITLE_POSITION_Y,
                                NULL,                            // Draw entire sprite
                                &Clip,                           // Clip to game view
                                BMP_VICTORY_TITLE,
                                VICTORY_TITLE_SPRITE,
                                VICTORY_TITLE_LAYER,
                                PRIORITY_UNUSED);
    }
    // The minimum mode duration has elapsed AND we have to exit, 
    // so we have to make the last black screen
    else if (m_ModeTime - m_ExitModeTime <= VICTORY_BLACKSCREEN_DURATION)
    {
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CVictory::StopSong (void)
{
    // Stop playing the victory song
    m_pSound->StopAllSamples();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
