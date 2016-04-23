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
 *  \file CVictory.h
 *  \brief Header file of the victory screen
 */

#ifndef __CVICTORY_H__
#define __CVICTORY_H__

class CDisplay;
class CInput;
class COptions;
class CTimer;
class CScores;
class CSound;

#include "CModeScreen.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define NUM_CONFETTIS_LARGE             6       //!< How many large confettis to manage
#define NUM_CONFETTIS_MEDIUM            10      //!< How many medium confettis to manage
#define NUM_CONFETTIS_SMALL             6       //!< How many small confettis to manage

//! Total number of confettis to manage
#define NUM_CONFETTIS                   (NUM_CONFETTIS_LARGE + NUM_CONFETTIS_MEDIUM + NUM_CONFETTIS_SMALL)

//! Describes the size of a confetti
enum EConfetti
{
    CONFETTI_LARGE,         //!< Large size
    CONFETTI_MEDIUM,        //!< Medium size
    CONFETTI_SMALL          //!< Small size
};

//! Describes the state of a confetti
struct SConfetti
{
    EConfetti Type;         //!< Type of the confetti
    float PositionX;        //!< Position X (in pixels) in the game view
    float PositionY;        //!< Position Y (in pixels) in the game view
    float SpeedX;           //!< Speed (in pixels per second) on the X-axis
    float SpeedY;           //!< Speed (in pixels per second) on the Y-axis
    float AnimationTimer;   //!< Timer of the loop animation of the confetti
    int Sprite;             //!< Sprite offset number (to add to confetti color offset number)
};

//! Describes the different modes of the crowd's wave
enum ECrowdWave
{
    CROWDWAVE_NONE,         //!< A lazy crowd
    CROWDWAVE_CLASSIC,      //!< The wave as it was up to version 1.02
    CROWDWAVE_MEXICAN,      //!< The mexican wave
    NUMBER_CROWDWAVES       //!< The number of different crowd waves
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The victory screen that appears after a player has won a match (a match is composed of battles)

class CVictory : public CModeScreen
{
private:

    CScores*        m_pScores;                      //!< Link to the scores object to use
    float           m_ModeTime;                     //!< Time (in seconds) that elapsed since the mode has started
    float           m_CrowdTimer;                   //!< Crowd animation timer
    float           m_WinnerBomberTimer;            //!< Timer of the animation of the victorious bomber
    float           m_LoserBomberTimer;             //!< Timer of the animation of the bombers that lost.
    SConfetti       m_Confettis [NUM_CONFETTIS];    //!< The confettis to manage on the screen
    bool            m_HaveToExit;                   //!< Do we have to exit this mode?
    float           m_ExitModeTime;                 //!< Mode time when we realized we have to exit (used for blackscreen)
    bool            m_PlayedSound;                  //!< Did we start playing the draw game sound?
    bool            m_CrowdFlag;                    //!< This flag allows to make the crowd switch between two states. Some bombers in the crowd get up and other bombers sit down.
    int             m_WinnerBomberSprite;           //!< This is the sprite offset to use to get the current victorious bomber sprite. This sprite offset must be added to the sprite offset determining the color of the bomber.
    int             m_LoserBomberSprite;            //!< This is the sprite offset to use to get the current loser sprite. This sprite offset must be added to the sprite offset determining the color of the bomber.
    float           m_MexicanWaveTimer;             //!< Timer of the crowd's Mexican wave animation
    int             m_MexicanWavePosition;          //!< The current position of the Mexican wave (which row is standing up)
    ECrowdWave      m_CrowdWaveMode;                //!< Which wave the crowd is doing

    void            ResetConfetti (SConfetti* pConfetti); //!< Make a new confetti, make it start at the top of the game view (out of the game view)

public:

                    CVictory();                         //!< Constructor. Initialize some members.
    virtual         ~CVictory();                        //!< Destructor. Does nothing.
    inline void     SetScores (CScores *pScores);       //!< Set link to the scores object to use
    void            Create (void);                      //!< Initialize the object
    void            Destroy (void);                     //!< Uninitialize the object
    void            OpenInput (void);                   //!< Get access to the input this object needs
    void            CloseInput (void);                  //!< Release access to the input this object needs
    EGameMode       Update (void);                      //!< Update the object and return what game mode should be set
    void            Display (void);                     //!< Display the game screen
    void            StopSong (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CVictory::SetScores (CScores *pScores)
{
    m_pScores = pScores;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CVICTORY_H__
