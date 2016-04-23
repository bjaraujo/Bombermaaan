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
 *  \file CMenuBase.h
 *  \brief Header file of the menu base
 */

#ifndef __CMENUBASE_H__
#define __CMENUBASE_H__

class CDisplay;
class CInput;
class CSound;
class COptions;
class CTimer;
class CFont;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CMenuBase
{
private:

    float           m_MenuModeTime;         //!< Time (in seconds) that elapsed since this menu mode has started
    bool            m_HaveToExit;           //!< Do we have to exit this menu mode?
    EMenuAction     m_ExitMenuAction;       //!< Menu action to ask for when exiting (after transition)
    float           m_ExitMenuModeTime;     //!< Menu mode time when we realized we have to exit (used for transition)

protected:

    CDisplay*       m_pDisplay;
    CSound*         m_pSound;
    CInput*         m_pInput;
    COptions*       m_pOptions;
    CTimer*         m_pTimer;
    CFont*          m_pFont;

    virtual void    OnCreate (void) = 0;
    virtual void    OnDestroy (void) = 0;
    virtual void    OnUpdate (void) = 0;
    virtual void    OnDisplay (void) = 0;
    virtual void    OnUp (void) = 0;
    virtual void    OnDown (void) = 0;
    virtual void    OnLeft (void) = 0;
    virtual void    OnRight (void) = 0;
    virtual void    OnPrevious (void) = 0;
    virtual void    OnNext (void) = 0;

    void            Exit (EMenuAction ExitMenuAction);
    
public:

    CMenuBase (void);
    virtual ~CMenuBase (void);

    inline void     SetDisplay (CDisplay *pDisplay);
    inline void     SetSound (CSound *pSound);
    inline void     SetInput (CInput *pInput);
    inline void     SetOptions (COptions *pOptions);
    inline void     SetTimer (CTimer *pTimer);
    inline void     SetFont (CFont *pFont);

    void            Create (void);
    void            Destroy (void);

    EMenuAction     Update (void);
    void            Display (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CMenuBase::SetDisplay (CDisplay *pDisplay)
{
    m_pDisplay = pDisplay;
}

inline void CMenuBase::SetSound (CSound *pSound)
{
    m_pSound = pSound;
}

inline void CMenuBase::SetInput (CInput *pInput)
{
    m_pInput = pInput;
}

inline void CMenuBase::SetOptions (COptions *pOptions)
{
    m_pOptions = pOptions;
}

inline void CMenuBase::SetTimer (CTimer *pTimer)
{
    m_pTimer = pTimer;
}

inline void CMenuBase::SetFont (CFont *pFont) 
{ 
    m_pFont = pFont; 
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CMENUBASE_H__
