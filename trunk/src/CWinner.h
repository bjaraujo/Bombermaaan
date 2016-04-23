/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007 Bernd Arnold

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
 *  \file CWinner.h
 *  \brief Header file of the winner screen
 */

#ifndef __CWINNER_H__
#define __CWINNER_H__

class CDisplay;
class CInput;
class COptions;
class CTimer;
class CScores;
class CMatch;
class CSound;
class CMosaic;

#include "CModeScreen.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The winner (scoreboard) screen that appears after a player has won one battle.

class CWinner : public CModeScreen
{
private:

    CScores*        m_pScores;                  //!< Link to the scores object to use
    CMatch*         m_pMatch;                   //!< Link to the match object to use
    float           m_LightsTimer;              //!< Lights animation timer
    float           m_HappyBomberTimer;         //!< Happy bomber animation timer
    float           m_SadBomberTimer;           //!< Sad bomber animation timer
    int             m_LightSpriteOffset;        //!< Sprite offset to use to switch light colors
    int             m_HappyBomberSpriteOffset;  //!< Sprite offset to use to have a happy bomber
    int             m_SadBomberSpriteOffset;    //!< Sprite offset to use to have a sad bomber
    bool            m_PlayedSound;              //!< Did we start playing the victory sound?
    CMosaic*        m_pMosaic;
    float           m_ModeTime;                 //!< Time (in seconds) that elapsed since the mode has started
    float           m_ExitModeTime;             //!< Mode time when we have to start the last black screen
    float           m_CoinTime;                 //!< Time that elapsed for the current coin sprite
    int             m_CoinSpriteOffset;         //!< Sprite offset of the coin
    int             m_ExitGameMode;             //!< Game mode to ask for when exiting
    bool            m_HaveToExit;               //!< Do we have to exit this mode?
    
public:

                    CWinner();                          //!< Constructor. Initialize some members.
    virtual         ~CWinner();                         //!< Destructor. Does nothing.
    inline void     SetScores (CScores *pScores);       //!< Set link to the scores object to use
    inline void     SetMatch (CMatch *pMatch);          //!< Set link to the match object to use
    void            Create (void);                      //!< Initialize the object
    void            Destroy (void);                     //!< Uninitialize the object
    void            OpenInput (void);                   //!< Get access to the input this object needs
    void            CloseInput (void);                  //!< Release access to the input this object needs
    EGameMode       Update (void);                      //!< Update the object and return what game mode should be set
    void            Display (void);                     //!< Display on the screen
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CWinner::SetScores (CScores *pScores)
{
    m_pScores = pScores;
}

inline void CWinner::SetMatch (CMatch *pMatch)
{
    m_pMatch = pMatch;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CWINNER_H__
