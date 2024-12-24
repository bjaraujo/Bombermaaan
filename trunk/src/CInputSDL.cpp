// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2008-2010 Markus Drescher

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
 *  \file CInputSDL.cpp
 *  \brief SDL input devices on Linux
 */

#include "StdAfx.h"

#include "CInputSDL.h"

#define JOYSTICK_DEAD_ZONE 10 //!< Percentage of the dead zone to set for each joystick
#define JOYSTICK_MINIMUM_AXIS -32768 //!< Minimum axis value to set for each joystick
#define JOYSTICK_MAXIMUM_AXIS +32767 //!< Maximum axis value to set for each joystick

CInputSDL::CInputSDL()
{
    m_hWnd = nullptr;
    m_hInstance = nullptr;
    m_Ready = false;

    m_KeyboardOpened = false;
    memset(m_KeyState, 0, MAX_KEYS);
    memset(m_KeyFriendlyName, 0, MAX_KEYS * MAX_PATH);

    m_joystickCount = 0;
}

CInputSDL::~CInputSDL() = default;

struct SEnumParam
{
    std::vector<SDL_Joystick*> pDevices;
};

// static bool CreateInputDevice (int Joystick, LPVOID pParameter)
// has been removed.

bool CInputSDL::Create()
{
    if (!m_Ready)
    {
        // Reset the keyboard state
        memset(m_KeyState, 0, MAX_KEYS);

        // Prepare the friendly name for each key
        MakeKeyFriendlyNames();

        // Create all joysticks that are installed on the system
        for (int i = 0; i < SDL_NumJoysticks(); i++)
        {
            auto* pJoystick = new SJoystick;

            if (pJoystick == nullptr)
            {
                // Log failure
                theLog.WriteLine("SDLInput        => !!! Could not allocate memory for a joystick.");

                // Get out
                return false;
            }

            // Reset the joystick state
            memset(&pJoystick->State, 0, sizeof(pJoystick->State));

            // The joystick is not opened yet
            pJoystick->Opened = false;

            // Set the joystick's device to nullptr (will be created later)
            pJoystick->pDevice = nullptr;

            m_pJoysticks.push_back(pJoystick); // the joystick is not opened

            theLog.WriteLine("SDLInput        => A joystick was added.");
        }

        m_Ready = true;
    }

    SDL_JoystickEventState(SDL_ENABLE);

    // Everything went right
    return true;
}

void CInputSDL::Destroy()
{
    if (m_Ready)
    {
        // Scan the joysticks that were created
        for (unsigned int Index = 0; Index < m_pJoysticks.size(); Index++)
        {
            // Release the joystick SDLinput device
            SDL_JoystickClose(m_pJoysticks[Index]->pDevice);
            m_pJoysticks[Index]->pDevice = nullptr;

            // Delete the joystick variable
            delete m_pJoysticks[Index];

            // Log we released a joystick
            theLog.WriteLine("SDLInput        => A joystick was released.");
        }

        // Remove the joystick pointers from the container
        m_pJoysticks.clear();

        // Log we released the SDLInput object
        theLog.WriteLine("SDLInput        => SDLInput object was released.");
    }
}

bool CInputSDL::UpdateDevice(SDL_Joystick* pDevice, void* pState) const { return true; }

bool CInputSDL::UpdateDevice(void* pState)
{
    int stateSize;
    const Uint8* keyState = SDL_GetKeyboardState(&stateSize);

    for (int i = 0; i < stateSize; i++)
        SetKey(i, keyState[i] == 1);

    return true;
}

void CInputSDL::UpdateKeyboard() { UpdateDevice(m_KeyState); }

void CInputSDL::UpdateJoystick(int Joystick) const { }

void CInputSDL::MakeKeyFriendlyNames()
{
    int c;

    for (int Key = 0; Key < MAX_KEYS; Key++)
    {
        switch (Key)
        {
        case KEYBOARD_ESCAPE:
            strcpy(m_KeyFriendlyName[Key], "ESCAPE");
            break;
        case KEYBOARD_1:
            strcpy(m_KeyFriendlyName[Key], "1");
            break;
        case KEYBOARD_2:
            strcpy(m_KeyFriendlyName[Key], "2");
            break;
        case KEYBOARD_3:
            strcpy(m_KeyFriendlyName[Key], "3");
            break;
        case KEYBOARD_4:
            strcpy(m_KeyFriendlyName[Key], "4");
            break;
        case KEYBOARD_5:
            strcpy(m_KeyFriendlyName[Key], "5");
            break;
        case KEYBOARD_6:
            strcpy(m_KeyFriendlyName[Key], "6");
            break;
        case KEYBOARD_7:
            strcpy(m_KeyFriendlyName[Key], "7");
            break;
        case KEYBOARD_8:
            strcpy(m_KeyFriendlyName[Key], "8");
            break;
        case KEYBOARD_9:
            strcpy(m_KeyFriendlyName[Key], "9");
            break;
        case KEYBOARD_0:
            strcpy(m_KeyFriendlyName[Key], "0");
            break;
        case KEYBOARD_MINUS:
            strcpy(m_KeyFriendlyName[Key], "MINUS");
            break;
        case KEYBOARD_EQUALS:
            strcpy(m_KeyFriendlyName[Key], "EQUALS");
            break;
        case KEYBOARD_BACK:
            strcpy(m_KeyFriendlyName[Key], "BACKSPACE");
            break;
        case KEYBOARD_TAB:
            strcpy(m_KeyFriendlyName[Key], "TAB");
            break;
        case KEYBOARD_Q:
            strcpy(m_KeyFriendlyName[Key], "Q");
            break;
        case KEYBOARD_W:
            strcpy(m_KeyFriendlyName[Key], "W");
            break;
        case KEYBOARD_E:
            strcpy(m_KeyFriendlyName[Key], "E");
            break;
        case KEYBOARD_R:
            strcpy(m_KeyFriendlyName[Key], "R");
            break;
        case KEYBOARD_T:
            strcpy(m_KeyFriendlyName[Key], "T");
            break;
        case KEYBOARD_Y:
            strcpy(m_KeyFriendlyName[Key], "Y");
            break;
        case KEYBOARD_U:
            strcpy(m_KeyFriendlyName[Key], "U");
            break;
        case KEYBOARD_I:
            strcpy(m_KeyFriendlyName[Key], "I");
            break;
        case KEYBOARD_O:
            strcpy(m_KeyFriendlyName[Key], "O");
            break;
        case KEYBOARD_P:
            strcpy(m_KeyFriendlyName[Key], "P");
            break;
        case KEYBOARD_RETURN:
            strcpy(m_KeyFriendlyName[Key], "RETURN");
            break;
        case KEYBOARD_LCONTROL:
            strcpy(m_KeyFriendlyName[Key], "L.CTRL");
            break;
        case KEYBOARD_A:
            strcpy(m_KeyFriendlyName[Key], "A");
            break;
        case KEYBOARD_S:
            strcpy(m_KeyFriendlyName[Key], "S");
            break;
        case KEYBOARD_D:
            strcpy(m_KeyFriendlyName[Key], "D");
            break;
        case KEYBOARD_F:
            strcpy(m_KeyFriendlyName[Key], "F");
            break;
        case KEYBOARD_G:
            strcpy(m_KeyFriendlyName[Key], "G");
            break;
        case KEYBOARD_H:
            strcpy(m_KeyFriendlyName[Key], "H");
            break;
        case KEYBOARD_J:
            strcpy(m_KeyFriendlyName[Key], "J");
            break;
        case KEYBOARD_K:
            strcpy(m_KeyFriendlyName[Key], "K");
            break;
        case KEYBOARD_L:
            strcpy(m_KeyFriendlyName[Key], "L");
            break;
        case KEYBOARD_SEMICOLON:
            strcpy(m_KeyFriendlyName[Key], "SEMICOLON");
            break;
        case KEYBOARD_APOSTROPHE:
            strcpy(m_KeyFriendlyName[Key], "APOSTR.");
            break;
        case KEYBOARD_GRAVE:
            strcpy(m_KeyFriendlyName[Key], "E-GRAVE");
            break;
        case KEYBOARD_LSHIFT:
            strcpy(m_KeyFriendlyName[Key], "L-SHIFT");
            break;
        case KEYBOARD_BACKSLASH:
            strcpy(m_KeyFriendlyName[Key], "BACKSLASH");
            break;
        case KEYBOARD_Z:
            strcpy(m_KeyFriendlyName[Key], "Z");
            break;
        case KEYBOARD_X:
            strcpy(m_KeyFriendlyName[Key], "X");
            break;
        case KEYBOARD_C:
            strcpy(m_KeyFriendlyName[Key], "C");
            break;
        case KEYBOARD_V:
            strcpy(m_KeyFriendlyName[Key], "V");
            break;
        case KEYBOARD_B:
            strcpy(m_KeyFriendlyName[Key], "B");
            break;
        case KEYBOARD_N:
            strcpy(m_KeyFriendlyName[Key], "N");
            break;
        case KEYBOARD_M:
            strcpy(m_KeyFriendlyName[Key], "M");
            break;
        case KEYBOARD_COMMA:
            strcpy(m_KeyFriendlyName[Key], "COMMA");
            break;
        case KEYBOARD_PERIOD:
            strcpy(m_KeyFriendlyName[Key], "PERIOD");
            break;
        case KEYBOARD_SLASH:
            strcpy(m_KeyFriendlyName[Key], "SLASH");
            break;
        case KEYBOARD_RSHIFT:
            strcpy(m_KeyFriendlyName[Key], "R-SHIFT");
            break;
        case KEYBOARD_MULTIPLY:
            strcpy(m_KeyFriendlyName[Key], "NUM-MUL");
            break;
        case KEYBOARD_LMENU:
            strcpy(m_KeyFriendlyName[Key], "L-ALT");
            break;
        case KEYBOARD_SPACE:
            strcpy(m_KeyFriendlyName[Key], "SPACE");
            break;
        case KEYBOARD_CAPITAL:
            strcpy(m_KeyFriendlyName[Key], "CAPITAL");
            break;
        case KEYBOARD_F1:
            strcpy(m_KeyFriendlyName[Key], "F1");
            break;
        case KEYBOARD_F2:
            strcpy(m_KeyFriendlyName[Key], "F2");
            break;
        case KEYBOARD_F3:
            strcpy(m_KeyFriendlyName[Key], "F3");
            break;
        case KEYBOARD_F4:
            strcpy(m_KeyFriendlyName[Key], "F4");
            break;
        case KEYBOARD_F5:
            strcpy(m_KeyFriendlyName[Key], "F5");
            break;
        case KEYBOARD_F6:
            strcpy(m_KeyFriendlyName[Key], "F6");
            break;
        case KEYBOARD_F7:
            strcpy(m_KeyFriendlyName[Key], "F7");
            break;
        case KEYBOARD_F8:
            strcpy(m_KeyFriendlyName[Key], "F8");
            break;
        case KEYBOARD_F9:
            strcpy(m_KeyFriendlyName[Key], "F9");
            break;
        case KEYBOARD_F10:
            strcpy(m_KeyFriendlyName[Key], "F10");
            break;
        case KEYBOARD_NUMLOCK:
            strcpy(m_KeyFriendlyName[Key], "NUMLOCK");
            break;
        case KEYBOARD_SCROLL:
            strcpy(m_KeyFriendlyName[Key], "SCROLL");
            break;
        case KEYBOARD_NUMPAD7:
            strcpy(m_KeyFriendlyName[Key], "NUM-7");
            break;
        case KEYBOARD_NUMPAD8:
            strcpy(m_KeyFriendlyName[Key], "NUM-8");
            break;
        case KEYBOARD_NUMPAD9:
            strcpy(m_KeyFriendlyName[Key], "NUM-9");
            break;
        case KEYBOARD_SUBTRACT:
            strcpy(m_KeyFriendlyName[Key], "NUM-MINUS");
            break;
        case KEYBOARD_NUMPAD4:
            strcpy(m_KeyFriendlyName[Key], "NUM-4");
            break;
        case KEYBOARD_NUMPAD5:
            strcpy(m_KeyFriendlyName[Key], "NUM-5");
            break;
        case KEYBOARD_NUMPAD6:
            strcpy(m_KeyFriendlyName[Key], "NUM-6");
            break;
        case KEYBOARD_ADD:
            strcpy(m_KeyFriendlyName[Key], "NUM-ADD");
            break;
        case KEYBOARD_NUMPAD1:
            strcpy(m_KeyFriendlyName[Key], "NUM-1");
            break;
        case KEYBOARD_NUMPAD2:
            strcpy(m_KeyFriendlyName[Key], "NUM-2");
            break;
        case KEYBOARD_NUMPAD3:
            strcpy(m_KeyFriendlyName[Key], "NUM-3");
            break;
        case KEYBOARD_NUMPAD0:
            strcpy(m_KeyFriendlyName[Key], "NUM-0");
            break;
        case KEYBOARD_DECIMAL:
            strcpy(m_KeyFriendlyName[Key], "NUM-DOT");
            break;
        case KEYBOARD_F11:
            strcpy(m_KeyFriendlyName[Key], "F11");
            break;
        case KEYBOARD_F12:
            strcpy(m_KeyFriendlyName[Key], "F12");
            break;
        case KEYBOARD_NUMPADENTER:
            strcpy(m_KeyFriendlyName[Key], "NUM-ENTER");
            break;
        case KEYBOARD_RCONTROL:
            strcpy(m_KeyFriendlyName[Key], "R-CTRL");
            break;
        case KEYBOARD_DIVIDE:
            strcpy(m_KeyFriendlyName[Key], "NUM-SLASH");
            break;
        case KEYBOARD_SYSRQ:
            strcpy(m_KeyFriendlyName[Key], "SYSTEM");
            break;
        case KEYBOARD_RMENU:
            strcpy(m_KeyFriendlyName[Key], "R-ALT");
            break;
        case KEYBOARD_PAUSE:
            strcpy(m_KeyFriendlyName[Key], "PAUSE");
            break;
        case KEYBOARD_HOME:
            strcpy(m_KeyFriendlyName[Key], "HOME");
            break;
        case KEYBOARD_UP:
            strcpy(m_KeyFriendlyName[Key], "UP");
            break;
        case KEYBOARD_PRIOR:
            strcpy(m_KeyFriendlyName[Key], "PAGEUP");
            break;
        case KEYBOARD_LEFT:
            strcpy(m_KeyFriendlyName[Key], "LEFT");
            break;
        case KEYBOARD_RIGHT:
            strcpy(m_KeyFriendlyName[Key], "RIGHT");
            break;
        case KEYBOARD_END:
            strcpy(m_KeyFriendlyName[Key], "END");
            break;
        case KEYBOARD_DOWN:
            strcpy(m_KeyFriendlyName[Key], "DOWN");
            break;
        case KEYBOARD_NEXT:
            strcpy(m_KeyFriendlyName[Key], "PAGEDOWN");
            break;
        case KEYBOARD_INSERT:
            strcpy(m_KeyFriendlyName[Key], "INSERT");
            break;
        case KEYBOARD_DELETE:
            strcpy(m_KeyFriendlyName[Key], "DELETE");
            break;
        case KEYBOARD_LWIN:
            strcpy(m_KeyFriendlyName[Key], "L-WIN");
            break;
        case KEYBOARD_RWIN:
            strcpy(m_KeyFriendlyName[Key], "R-WIN");
            break;
        case KEYBOARD_APPS:
            strcpy(m_KeyFriendlyName[Key], "APP-MENU");
            break;

        default:

#ifdef WIN32
            c = _snprintf(m_KeyFriendlyName[Key], sizeof(m_KeyFriendlyName[Key]), "UNKNOWN KEY %d", Key);
#else
            c = snprintf(m_KeyFriendlyName[Key], sizeof(m_KeyFriendlyName[Key]), "UNKNOWN KEY %d", Key);
#endif

            if (c == -1)
            {
                c = sizeof(m_KeyFriendlyName[Key]) - 1;
            }

            if (c >= 0 && c < MAX_PATH)
            {
                m_KeyFriendlyName[Key][c] = '\0';
            }

            break;
        }
    }
}

void CInputSDL::SetWindowHandle(HWND hWnd) { m_hWnd = hWnd; }

void CInputSDL::SetInstanceHandle(HINSTANCE hInstance) { m_hInstance = hInstance; }

void CInputSDL::OpenKeyboard() { m_KeyboardOpened = true; }

bool CInputSDL::IsKeyboardOpened() const
{
    // Return the opened state of the keyboard
    return m_KeyboardOpened;
}

void CInputSDL::CloseKeyboard() { m_KeyboardOpened = false; }

bool CInputSDL::GetKey(int Key) const
{
    // Assert the key number is correct
    ASSERT(Key >= 0 && Key < MAX_KEYS);

    // Return the state of the key
    return (m_KeyState[Key] & 0x80) != 0;
}

void CInputSDL::SetKey(int Key, bool KeySet)
{
    // Assert the key number is correct
    ASSERT(Key >= 0 && Key < MAX_KEYS);

    // Set/remove state of the key
    if (KeySet)
    {
        m_KeyState[Key] |= 0x80;
    }
    else
    {
        m_KeyState[Key] &= ~0x80;
    }

    return;
}

const char* CInputSDL::GetKeyFriendlyName(int Key) const
{
    // Assert the key number is correct
    ASSERT(Key >= 0 && Key < MAX_KEYS);

    // Return the name of the key
    return m_KeyFriendlyName[Key];
}

int CInputSDL::GetJoystickCount() const
{
    // Return the number of joysticks installed on the system
    return m_pJoysticks.size();
}

void CInputSDL::OpenJoystick(int Joystick)
{

    // Check if the joystick number is correct
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if (m_pJoysticks[Joystick]->Opened)
            return;

        // Try to acquire the joystick
        m_pJoysticks[Joystick]->pDevice = SDL_JoystickOpen(Joystick);

        // Set the opened state according to the return value
        m_pJoysticks[Joystick]->Opened = (m_pJoysticks[Joystick]->pDevice != nullptr);
    }
}

bool CInputSDL::IsJoystickOpened(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the opened state of this joystick
    return m_pJoysticks[Joystick]->Opened;
}

void CInputSDL::CloseJoystick(int Joystick)
{
    // Check if the joystick number is correct
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        // Release access to this joystick
        SDL_JoystickClose(m_pJoysticks[Joystick]->pDevice);

        // We are sure this joystick is not opened */
        m_pJoysticks[Joystick]->Opened = false;
        m_pJoysticks[Joystick]->pDevice = nullptr;
    }
}

int CInputSDL::GetJoystickAxisX(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the value of the X axis of this joystick
    return m_pJoysticks[Joystick]->State.lX;
}

int CInputSDL::GetJoystickAxisY(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the value of the Y axis of this joystick
    return m_pJoysticks[Joystick]->State.lY;
}

bool CInputSDL::GetJoystickButton(int Joystick, int Button)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Assert the button number is correct
    ASSERT(Button >= 0 && Button < MAX_JOYSTICK_BUTTONS);

    // Return the state of the specified button on this joystick
    return (m_pJoysticks[Joystick]->State.rgbButtons[Button] & 0x80) != 0;
}

void CInputSDL::SetJoystickAxisX(int Joystick, int AxisX)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Set the value of the X axis of this joystick
    m_pJoysticks[Joystick]->State.lX = AxisX;

    return;
}

void CInputSDL::SetJoystickAxisY(int Joystick, int AxisY)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Set the value of the Y axis of this joystick
    m_pJoysticks[Joystick]->State.lY = AxisY;

    return;
}

void CInputSDL::SetJoystickButton(int Joystick, int Button, bool onoff)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Assert the button number is correct
    ASSERT(Button >= 0 && Button < MAX_JOYSTICK_BUTTONS);

    // Return the state of the specified button on this joystick
    if (onoff)
    {
        m_pJoysticks[Joystick]->State.rgbButtons[Button] |= 0x80;
    }
    else
    {
        m_pJoysticks[Joystick]->State.rgbButtons[Button] &= ~0x80;
    }

    return;
}

bool CInputSDL::TestUp(int Joystick)
{
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {
        m_joystickCount++;

        if (m_joystickCount > 200 && m_pJoysticks[Joystick]->State.lY < -JOYSTICK_AXIS_THRESHOLD)
        {
            m_joystickCount = 0;
            return true;
        }
    }

    return false;
}

bool CInputSDL::TestDown(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        m_joystickCount++;

        if (m_joystickCount > 200 && m_pJoysticks[Joystick]->State.lY > +JOYSTICK_AXIS_THRESHOLD)
        {
            m_joystickCount = 0;
            return true;
        }
    }

    return false;
}

bool CInputSDL::TestLeft(int Joystick)
{
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {
        m_joystickCount++;

        if (m_joystickCount > 200 && m_pJoysticks[Joystick]->State.lX < -JOYSTICK_AXIS_THRESHOLD)
        {
            m_joystickCount = 0;
            return true;
        }
    }

    return false;
}

bool CInputSDL::TestRight(int Joystick)
{
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {
        m_joystickCount++;

        if (m_joystickCount > 200 && m_pJoysticks[Joystick]->State.lX > +JOYSTICK_AXIS_THRESHOLD)
        {
            m_joystickCount = 0;
            return true;
        }
    }

    return false;
}

bool CInputSDL::TestNext(int Joystick)
{
    return Joystick >= 0 && Joystick < (int)m_pJoysticks.size()
        && (m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_MENU_NEXT - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0;
}

bool CInputSDL::TestPrevious(int Joystick)
{
    return Joystick >= 0 && Joystick < (int)m_pJoysticks.size()
        && (m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_MENU_PREVIOUS - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80)
        != 0;
}

bool CInputSDL::TestBreak(int Joystick)
{
    return Joystick >= 0 && Joystick < (int)m_pJoysticks.size()
        && (m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_BREAK - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0;
}

bool CInputSDL::TestStart(int Joystick)
{
    return Joystick >= 0 && Joystick < (int)m_pJoysticks.size()
        && (m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_START - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0;
}
