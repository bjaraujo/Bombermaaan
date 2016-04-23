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
 *  \file CMenuYesNo.h
 *  \brief Header file of the yes/no menu
 */

#ifndef __CMENUYESNO_H__
#define __CMENUYESNO_H__

#include "CFont.h"

class CDisplay;
class CInput;
class CSound;
class CTimer;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! A mini menu with two options: yes or no
class CMenuYesNo
{
private:

    CDisplay*           m_pDisplay;             //!< Link to display object to use
    CInput*             m_pInput;               //!< Link to input object to use
    CSound*             m_pSound;               //!< Link to sound object to use
    CTimer*             m_pTimer;
    CFont               m_Font;                 //!< Font object used to draw text
    bool                m_YesNo;
    char                m_Title[25];
    bool                m_Active;
    bool                m_WasSoundPaused;       //!< Was the sound paused when the Yes/No message box had to appear?
    
public:

                        CMenuYesNo (void);
                        ~CMenuYesNo (void);
    inline void         SetDisplay (CDisplay *pDisplay);    //!< Set link to the display object to use
    inline void         SetInput (CInput *pInput);          //!< Set link to the input object to use
    inline void         SetTimer (CTimer *pTimer);          //!< Set link to the timer object to use
    inline void         SetSound (CSound *pSound);          //!< Set link to the sound object to use
    void                Create (void);
    void                Destroy (void);
    EGameMode           Update (EGameMode CurrentGameMode); //!< Update the object and return what was chosen
    void                Display (void);
    inline bool         IsActive (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CMenuYesNo::SetDisplay (CDisplay *pDisplay)
{
    m_pDisplay = pDisplay;
    m_Font.SetDisplay (pDisplay);
}

inline void CMenuYesNo::SetInput (CInput *pInput)
{
    m_pInput = pInput;
}

inline void CMenuYesNo::SetTimer (CTimer *pTimer)
{
    m_pTimer = pTimer;
}

inline void CMenuYesNo::SetSound (CSound *pSound)
{
    m_pSound = pSound;
}

inline bool CMenuYesNo::IsActive (void)
{
    return m_Active;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CMENUYESNO_H__
