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
 *  \file CDrawGame.h
 *  \brief Header file of a draw game
 */

#ifndef __CDRAWGAME_H__
#define __CDRAWGAME_H__

#include "CModeScreen.h"

class CDisplay;
class CInput;
class CTimer;
class CScores;
class COptions;
class CSound;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The Draw Game screen that appears after there is a draw game in a match.

class CDrawGame : public CModeScreen
{
private:

    CScores*        m_pScores;          //!< Link to the scores object to use
    float           m_FlagTimer;        //!< Timer for flag animation
    float           m_BigFumeTimer;     //!< Timer for the big fume animation
    float           m_SmallFumeTimer;   //!< Timer for the small fume animation
    int             m_FlagSprite;       //!< Flag sprite to draw
    int             m_BigFumeSprite;    //!< Big fume sprite to draw
    int             m_SmallFumeSprite;  //!< Small fume sprite to draw
    float           m_ModeTime;         //!< Time elapsed since the mode has started
    bool            m_HaveToExit;       //!< Do we have to exit this mode?
    float           m_ExitModeTime;     //!< Mode time when we realized we have to exit (used for blackscreen)
    bool            m_PlayedSound;      //!< Did we start playing the draw game sound?

public:

                    CDrawGame();                        //!< Constructor. Initialize some members.
    virtual         ~CDrawGame();                       //!< Destructor. Does nothing.
    inline void     SetScores (CScores *pScores);       //!< Set link to the scores object to use
    void            Create (void);                      //!< Initialize the object
    void            Destroy (void);                     //!< Uninitialize the object
    void            OpenInput (void);                   //!< Get access to the input this object needs
    void            CloseInput (void);                  //!< Release access to the input this object needs
    EGameMode       Update (void);                      //!< Update the object and return which game mode should be set
    void            Display (void);                     //!< Display on the screen
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CDrawGame::SetScores (CScores *pScores)
{
    m_pScores = pScores;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CDRAWGAME_H__
