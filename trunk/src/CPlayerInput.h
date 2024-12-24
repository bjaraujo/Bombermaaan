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

#ifdef DIRECTX_INPUT
#include "CInputDX.h"
#else
#include "CInputSDL.h"
#endif

#define MAX_PLAYER_INPUT_NAME_LENGTH 16 //!< Maximum length of a string containing a player input name

#define NO_ACTIVATED_CONTROL -1

#define MAX_CONTROL_NAME_LENGTH 20

class CPlayerInput
{
private:
    InputClass* m_pDirectInput;
    COptions* m_pOptions;
    int m_PlayerInput;
    char m_Name[MAX_PLAYER_INPUT_NAME_LENGTH];
    char m_ControlName[MAX_CONTROL_NAME_LENGTH];

    void CreateName();

public:
    CPlayerInput();
    ~CPlayerInput();
    InputClass* GetDirectInput();
    void SetDirectInput(InputClass* pDirectInput);
    void SetOptions(COptions* pOptions);
    void Create(int PlayerInput);
    void Destroy();
    void Open();
    bool IsOpened();
    void Close();
    const char* GetName() const;
    void Update();
    bool TestUp();
    bool TestDown();
    bool TestLeft();
    bool TestRight();
    bool TestAction1();
    bool TestAction2();
    bool TestMenuNext();
    bool TestPause();
    int GetActivatedControl();
    const char* GetControlName(int Control);
    bool TestControl(int Control);
    bool TestMenuControl(int MenuControl);
};

inline InputClass* CPlayerInput::GetDirectInput() { return m_pDirectInput; }

inline void CPlayerInput::SetDirectInput(InputClass* pDirectInput) { m_pDirectInput = pDirectInput; }

inline void CPlayerInput::SetOptions(COptions* pOptions) { m_pOptions = pOptions; }

inline const char* CPlayerInput::GetName() const { return m_Name; }

inline bool CPlayerInput::TestUp() { return TestControl(CONTROL_UP); }

inline bool CPlayerInput::TestDown() { return TestControl(CONTROL_DOWN); }

inline bool CPlayerInput::TestLeft() { return TestControl(CONTROL_LEFT); }

inline bool CPlayerInput::TestRight() { return TestControl(CONTROL_RIGHT); }

inline bool CPlayerInput::TestAction1() { return TestControl(CONTROL_ACTION1); }

inline bool CPlayerInput::TestAction2() { return TestControl(CONTROL_ACTION2); }

//! The "menu next" button was pressed?

inline bool CPlayerInput::TestMenuNext() { return TestMenuControl(JOYSTICK_BUTTON_MENU_NEXT); }

//! The "pause" button was pressed?

inline bool CPlayerInput::TestPause() { return TestMenuControl(JOYSTICK_BUTTON_BREAK); }

#endif // __CPLAYERINPUT_H__
