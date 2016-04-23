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
 *  \file CBoard.h
 *  \brief Header file of the board
 */

#ifndef __CBOARD_H__
#define __CBOARD_H__

class CClock;
class CDisplay;
class COptions;
class CTimer;
class CScores;
class CArena;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The board that is above the arena in the match screen. It shows the scores and the time left.

class CBoard
{
private:

    CDisplay*       m_pDisplay;                 //!< Link to the display object to use
    COptions*       m_pOptions;                 //!< Link to the options object to use
    CScores*        m_pScores;                  //!< Link to the scores object to use
    CClock*         m_pClock;                   //!< Link to the clock object to use
    CTimer*         m_pTimer;                   //!< Link to the timer object to use
    CArena*         m_pArena;                   //!< Link to the arena object to use
    float           m_ClockBottomTimer;         //!< Timer for clockbottom animation
    float           m_ClockTopTimer;            //!< Timer for clocktop animation
    int             m_ClockBottomSprite;        //!< Current clockbottom sprite to draw
    int             m_ClockTopSprite;           //!< Current clocktop sprite to draw
    bool            m_AnimateClock;             //!< Should the clock animate?

public:

                    CBoard (void);                          //!< Constructor. Initialize some members.
                    ~CBoard (void);                         //!< Destructor. Does nothing.
    inline void     SetDisplay (CDisplay *pDisplay);        //!< Set link to the display object to use
    inline void     SetOptions (COptions *pOptions);        //!< Set link to the options object to use
    inline void     SetScores (CScores *pScores);           //!< Set link to the scores object to use
    inline void     SetClock (CClock *pClock);              //!< Set link to the clock object to use
    inline void     SetTimer (CTimer *pTimer);              //!< Set link to the timer object to use
    inline void     SetArena (CArena *pArena);              //!< Set link to the arena object to use
    void            Create (void);                          //!< Initialize the object
    void            Destroy (void);                         //!< Uninitialize the object
    void            Update (void);                          //!< Update the board
    void            Display (void);                         //!< Display the board
    inline void     SetClockAnimation (bool AnimateClock);  //!< Make the board's clock animation active or inactive
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CBoard::SetDisplay (CDisplay *pDisplay)
{
    m_pDisplay = pDisplay;
}

inline void CBoard::SetOptions (COptions *pOptions)
{
    m_pOptions = pOptions;
}

inline void CBoard::SetScores (CScores *pScores)
{
    m_pScores = pScores;
}

inline void CBoard::SetClock (CClock *pClock)
{
    m_pClock = pClock;
}

inline void CBoard::SetTimer (CTimer *pTimer)
{
    m_pTimer = pTimer;
}

inline void CBoard::SetClockAnimation (bool AnimateClock)
{
    m_AnimateClock = AnimateClock;
}

inline void CBoard::SetArena (CArena *pArena)
{
    m_pArena = pArena;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


#endif
