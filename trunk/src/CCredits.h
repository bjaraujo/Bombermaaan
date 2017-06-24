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
 *  \file CControls.h
 *  \brief Header file of the controls
 */

#ifndef __CCREDITS_H__
#define __CCREDITS_H__

#include "CFont.h"
#include "CModeScreen.h"

class CDisplay;
class CInput;
class COptions;
class CTimer;
class CSound;
class CMosaic;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The credits screen.

class CCredits : public CModeScreen
{
private:

    float           m_ModeTime;                 //!< Time (in seconds) that elapsed since the mode has started
    bool            m_HaveToExit;               //!< Do we have to exit this mode?
    float           m_ExitModeTime;             //!< Mode time when we realized we have to exit (used for blackscreen)
    CFont           m_Font;                     //!< Font object used to draw strings
    int             m_Cursor;                   //!< Number of the menu item the cursor hand is pointing to
    bool            m_SongStarted;              //!< Did we start playing the song after the black screen?
    CMosaic*        m_pMosaic;

public:

                    CCredits();                         //!< Constructor. Initialize some members.
    virtual         ~CCredits();                        //!< Destructor. Does nothing.
    inline void     SetDisplay (CDisplay *pDisplay);    //!< Set link to the display object to use
    void            Create (void);                      //!< Initialize the object
    void            Destroy (void);                     //!< Uninitialize the object
    void            OpenInput (void);                   //!< Get access to the input this object needs
    void            CloseInput (void);                  //!< Release access to the input this object needs
    EGameMode       Update (void);                      //!< Update the object and return what game mode should be set
    void            Display (void);                     //!< Display the game screen
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CCredits::SetDisplay (CDisplay *pDisplay)
{
    CModeScreen::SetDisplay (pDisplay);
    m_Font.SetDisplay (m_pDisplay);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CCREDITS_H__
