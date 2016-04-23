/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2010 Bernd Arnold

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
 *  \file CPlayerInput.h
 *  \brief Header file of a player's input
 */

#ifndef __CPLAYERINPUT_H__
#define __CPLAYERINPUT_H__

#include "COptions.h"
#ifdef WIN32
#include "CDirectInput.h"
#else
#include "CSDLInput.h"
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MAX_PLAYER_INPUT_NAME_LENGTH    16      //!< Maximum length of a string containing a player input name

#define JOYSTICK_UP         0
#define JOYSTICK_DOWN       1
#define JOYSTICK_LEFT       2
#define JOYSTICK_RIGHT      3
#define JOYSTICK_BUTTON(x)  (4 + x)

#define NUMBER_OF_JOYSTICK_DIRECTIONS     4     //!< Number of joystick directions (up down left right)

//! The third joystick button can be used for leaving the winner screen
#define JOYSTICK_BUTTON_MENU_NEXT   JOYSTICK_BUTTON(2)

//! The 9th joystick button (this usually is the "start" button) can be used for leaving the winner screen
//! On xbox controller, the start button could be the 7th button (see tracker item #2907122)
#define JOYSTICK_BUTTON_PAUSE       JOYSTICK_BUTTON(9)

#define NO_ACTIVATED_CONTROL    -1

#define MAX_CONTROL_NAME_LENGTH     20

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CPlayerInput
{
private:

	InputClass*         m_pDirectInput;
    COptions*           m_pOptions;
    int                 m_PlayerInput;
    char                m_Name [MAX_PLAYER_INPUT_NAME_LENGTH];
    char                m_ControlName [MAX_CONTROL_NAME_LENGTH];
                        
    void                CreateName (void);
                        
public:                 
                        
                        CPlayerInput();
                        ~CPlayerInput();
    inline InputClass*  GetDirectInput (void);
    inline void         SetDirectInput (InputClass* pDirectInput);
    inline void         SetOptions (COptions* pOptions);
    void                Create (int PlayerInput);
    void                Destroy (void);
    void                Open (void);
    bool                IsOpened (void);
    void                Close (void);
    inline const char*  GetName (void);
    void                Update (void);
    inline bool         TestUp (void);
    inline bool         TestDown (void);
    inline bool         TestLeft (void);
    inline bool         TestRight (void);
    inline bool         TestAction1 (void);
    inline bool         TestAction2 (void);
    inline bool         TestMenuNext (void);
    inline bool         TestPause (void);
    int                 GetActivatedControl (void);
    const char*         GetControlName (int Control);
    bool                TestControl (int Control);
    bool                TestMenuControl (int MenuControl);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline InputClass* CPlayerInput::GetDirectInput (void)
{
    return m_pDirectInput;
}

inline void CPlayerInput::SetDirectInput (InputClass* pDirectInput)
{
    m_pDirectInput = pDirectInput;
}

inline void CPlayerInput::SetOptions (COptions* pOptions)
{
    m_pOptions = pOptions;
}

inline const char* CPlayerInput::GetName (void)
{
    return m_Name;
}

inline bool CPlayerInput::TestUp (void)
{
    return TestControl (CONTROL_UP);
}

inline bool CPlayerInput::TestDown (void)
{
    return TestControl (CONTROL_DOWN);
}

inline bool CPlayerInput::TestLeft (void)
{
    return TestControl (CONTROL_LEFT);
}

inline bool CPlayerInput::TestRight (void)
{
    return TestControl (CONTROL_RIGHT);
}

inline bool CPlayerInput::TestAction1 (void)
{
    return TestControl (CONTROL_ACTION1);
}

inline bool CPlayerInput::TestAction2 (void)
{
    return TestControl (CONTROL_ACTION2);
}

//! The "menu next" button was pressed?

inline bool CPlayerInput::TestMenuNext (void)
{
    return TestMenuControl (JOYSTICK_BUTTON_MENU_NEXT);
}

//! The "pause" button was pressed?

inline bool CPlayerInput::TestPause (void)
{
    return TestMenuControl (JOYSTICK_BUTTON_PAUSE);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CPLAYERINPUT_H__
