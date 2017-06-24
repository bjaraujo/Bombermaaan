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
 *  \file CTitle.h
 *  \brief Header file of the title screen
 */

#ifndef __CTITLE_H__
#define __CTITLE_H__

#include "CModeScreen.h"
#include "CCloudManager.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The title screen

class CTitle : public CModeScreen
{
private:

    int             m_Cursor;                       //!< Number of the menu item pointed by the cursor hand
    bool            m_SongStarted;                  //!< Did we start playing the song after the black screen?
    CCloudManager   m_CloudManager;                 //!< Manages the clouds in the sky
    float           m_ModeTime;                     //!< Time (in seconds) that elapsed since the mode has started
    float           m_ExitModeTime;                 //!< Mode time when we have to start the last black screen
    EGameMode       m_ExitGameMode;                 //!< Game mode to ask for when exiting
    bool            m_HaveToExit;                   //!< Do we have to exit this mode?
    int             m_MovingHandDistance;           //!< Extra distance between hand and menu
    float           m_MovingHandTimer;              //!< Timer used for the moving hand
    float            m_IdleTime;                        //!< Time this screen is idle

public:

                    CTitle();                           //!< Constructor. Initialize some members.
    virtual         ~CTitle();                          //!< Destructor. Does nothing.
    inline void     SetDisplay (CDisplay *pDisplay);    //!< Set link to the display object to use
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

inline void CTitle::SetDisplay (CDisplay *pDisplay)
{
    CModeScreen::SetDisplay (pDisplay);
    m_CloudManager.SetDisplay (pDisplay);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CTITLE_H__
