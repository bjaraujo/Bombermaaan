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
 *  \file CModeScreen.h
 *  \brief Header file of the screen mode
 */
// CModeScreen.h

#ifndef __CMODESCREEN_H__
#define __CMODESCREEN_H__

class CDisplay;
class CInput;
class COptions;
class CScores;
class CTimer;
class CSound;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CModeScreen
{   
protected:

    CDisplay*           m_pDisplay;                     //!< Link to the display object to use
    CInput*             m_pInput;                       //!< Link to the input object to use
    COptions*           m_pOptions;                     //!< Link to the options object to use
    CTimer*             m_pTimer;                       //!< Link to the timer object to use
    CSound*             m_pSound;                       //!< Link to the sound object to use
                      
public:                 
                        
                        CModeScreen();                      //!< Constructor. Initialize some members.
    virtual             ~CModeScreen();                     //!< Destructor. Does nothing.
    inline virtual void SetDisplay (CDisplay *pDisplay);    //!< Set link to the display object to use
    inline virtual void SetInput (CInput *pInput);          //!< Set link to the input object to use
    inline virtual void SetOptions (COptions *pOptions);    //!< Set link to the options object to use
    inline virtual void SetTimer (CTimer *pTimer);          //!< Set link to the timer object to use
    inline virtual void SetSound (CSound *pSound);          //!< Set link to the sound object to use
    virtual void        Create (void);                      //!< Initialize the object
    virtual void        Destroy (void);                     //!< Uninitialize the object
    virtual void        OpenInput (void) = 0;               //!< Get access to the input this object needs
    virtual void        CloseInput (void) = 0;              //!< Release access to the input this object needs
    virtual EGameMode   Update (void) = 0;                  //!< Update the object and return what game mode should be set
    virtual void        Display (void) = 0;                 //!< Display on the screen
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CModeScreen::SetDisplay (CDisplay *pDisplay)
{
    m_pDisplay = pDisplay;
}

inline void CModeScreen::SetInput (CInput *pInput)
{
    m_pInput = pInput;
}

inline void CModeScreen::SetOptions (COptions *pOptions)
{
    m_pOptions = pOptions;
}

inline void CModeScreen::SetTimer (CTimer *pTimer)
{
    m_pTimer = pTimer;
}

inline void CModeScreen::SetSound (CSound *pSound)
{
    m_pSound = pSound;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CMODESCREEN_H__
