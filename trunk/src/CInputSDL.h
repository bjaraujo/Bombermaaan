/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2008 Markus Drescher

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
 *  \file CInputSDL.h
 *  \brief Header file of the SDL input
 */

#ifndef __CINPUTSDL_H__
#define __CINPUTSDL_H__

#include "SDL.h"
#include "StdAfx.h"

#define MAX_JOYSTICK_BUTTONS 32 //!< Maximum number of buttons a joystick can have
#define MAX_KEYS SDL_NUM_SCANCODES //!< Maximum number of keys a keyboard can have

#define KEYBOARD_ESCAPE SDL_SCANCODE_ESCAPE //!< ID for the escape key
#define KEYBOARD_1 SDL_SCANCODE_1 //!< ID for the 1 key
#define KEYBOARD_2 SDL_SCANCODE_2
#define KEYBOARD_3 SDL_SCANCODE_3
#define KEYBOARD_4 SDL_SCANCODE_4
#define KEYBOARD_5 SDL_SCANCODE_5
#define KEYBOARD_6 SDL_SCANCODE_6
#define KEYBOARD_7 SDL_SCANCODE_7
#define KEYBOARD_8 SDL_SCANCODE_8
#define KEYBOARD_9 SDL_SCANCODE_9
#define KEYBOARD_0 SDL_SCANCODE_0
#define KEYBOARD_MINUS SDL_SCANCODE_MINUS //!< - on main keyboard
#define KEYBOARD_EQUALS SDL_SCANCODE_EQUALS
#define KEYBOARD_BACK SDL_SCANCODE_BACKSPACE //!< backspace
#define KEYBOARD_TAB SDL_SCANCODE_TAB
#define KEYBOARD_Q SDL_SCANCODE_Q
#define KEYBOARD_W SDL_SCANCODE_W
#define KEYBOARD_E SDL_SCANCODE_E
#define KEYBOARD_R SDL_SCANCODE_R
#define KEYBOARD_T SDL_SCANCODE_T
#define KEYBOARD_Y SDL_SCANCODE_Y
#define KEYBOARD_U SDL_SCANCODE_U
#define KEYBOARD_I SDL_SCANCODE_I
#define KEYBOARD_O SDL_SCANCODE_O
#define KEYBOARD_P SDL_SCANCODE_P
#define KEYBOARD_RETURN SDL_SCANCODE_RETURN //!< Enter on main keyboard
#define KEYBOARD_LCONTROL SDL_SCANCODE_LCTRL
#define KEYBOARD_A SDL_SCANCODE_A
#define KEYBOARD_S SDL_SCANCODE_S
#define KEYBOARD_D SDL_SCANCODE_D
#define KEYBOARD_F SDL_SCANCODE_F
#define KEYBOARD_G SDL_SCANCODE_G
#define KEYBOARD_H SDL_SCANCODE_H
#define KEYBOARD_J SDL_SCANCODE_J
#define KEYBOARD_K SDL_SCANCODE_K
#define KEYBOARD_L SDL_SCANCODE_L
#define KEYBOARD_SEMICOLON SDL_SCANCODE_SEMICOLON
#define KEYBOARD_APOSTROPHE SDL_SCANCODE_APOSTROPHE
#define KEYBOARD_GRAVE SDL_SCANCODE_GRAVE //!< accent grave
#define KEYBOARD_LSHIFT SDL_SCANCODE_LSHIFT
#define KEYBOARD_BACKSLASH SDL_SCANCODE_BACKSLASH
#define KEYBOARD_Z SDL_SCANCODE_Z
#define KEYBOARD_X SDL_SCANCODE_X
#define KEYBOARD_C SDL_SCANCODE_C
#define KEYBOARD_V SDL_SCANCODE_V
#define KEYBOARD_B SDL_SCANCODE_B
#define KEYBOARD_N SDL_SCANCODE_N
#define KEYBOARD_M SDL_SCANCODE_M
#define KEYBOARD_COMMA SDL_SCANCODE_COMMA
#define KEYBOARD_PERIOD SDL_SCANCODE_PERIOD //!< . on main keyboard
#define KEYBOARD_SLASH SDL_SCANCODE_SLASH //!< / on main keyboard
#define KEYBOARD_RSHIFT SDL_SCANCODE_RSHIFT
#define KEYBOARD_MULTIPLY SDL_SCANCODE_KP_MULTIPLY //!< * on numeric keypad
#define KEYBOARD_LMENU SDL_SCANCODE_LALT //!< left Alt
#define KEYBOARD_SPACE SDL_SCANCODE_SPACE
#define KEYBOARD_CAPITAL SDL_SCANCODE_CAPSLOCK
#define KEYBOARD_F1 SDL_SCANCODE_F1
#define KEYBOARD_F2 SDL_SCANCODE_F2
#define KEYBOARD_F3 SDL_SCANCODE_F3
#define KEYBOARD_F4 SDL_SCANCODE_F4
#define KEYBOARD_F5 SDL_SCANCODE_F5
#define KEYBOARD_F6 SDL_SCANCODE_F6
#define KEYBOARD_F7 SDL_SCANCODE_F7
#define KEYBOARD_F8 SDL_SCANCODE_F8
#define KEYBOARD_F9 SDL_SCANCODE_F9
#define KEYBOARD_F10 SDL_SCANCODE_F10
#define KEYBOARD_NUMLOCK SDL_SCANCODE_NUMLOCKCLEAR
#define KEYBOARD_SCROLL SDL_SCANCODE_SCROLLLOCK //!< Scroll Lock
#define KEYBOARD_NUMPAD7 SDL_SCANCODE_KP_7
#define KEYBOARD_NUMPAD8 SDL_SCANCODE_KP_8
#define KEYBOARD_NUMPAD9 SDL_SCANCODE_KP_9
#define KEYBOARD_SUBTRACT SDL_SCANCODE_KP_MINUS //!< - on numeric keypad
#define KEYBOARD_NUMPAD4 SDL_SCANCODE_KP_4
#define KEYBOARD_NUMPAD5 SDL_SCANCODE_KP_5
#define KEYBOARD_NUMPAD6 SDL_SCANCODE_KP_6
#define KEYBOARD_ADD SDL_SCANCODE_KP_PLUS //!< + on numeric keypad
#define KEYBOARD_NUMPAD1 SDL_SCANCODE_KP_1
#define KEYBOARD_NUMPAD2 SDL_SCANCODE_KP_2
#define KEYBOARD_NUMPAD3 SDL_SCANCODE_KP_3
#define KEYBOARD_NUMPAD0 SDL_SCANCODE_KP_0
#define KEYBOARD_DECIMAL SDL_SCANCODE_KP_PERIOD //!< . on numeric keypad
#define KEYBOARD_F11 SDL_SCANCODE_F11
#define KEYBOARD_F12 SDL_SCANCODE_F12
#define KEYBOARD_NUMPADENTER SDL_SCANCODE_KP_ENTER //!< Enter on numeric keypad
#define KEYBOARD_RCONTROL SDL_SCANCODE_RCTRL
#define KEYBOARD_DIVIDE SDL_SCANCODE_KP_DIVIDE //!< / on numeric keypad
#define KEYBOARD_SYSRQ SDL_SCANCODE_SYSREQ
#define KEYBOARD_RMENU SDL_SCANCODE_RALT //!< right Alt
#define KEYBOARD_PAUSE SDL_SCANCODE_PAUSE //!< Pause
#define KEYBOARD_HOME SDL_SCANCODE_HOME //!< Home on arrow keypad
#define KEYBOARD_UP SDL_SCANCODE_UP //!< UpArrow on arrow keypad
#define KEYBOARD_PRIOR SDL_SCANCODE_PAGEUP //!< PgUp on arrow keypad
#define KEYBOARD_LEFT SDL_SCANCODE_LEFT //!< LeftArrow on arrow keypad
#define KEYBOARD_RIGHT SDL_SCANCODE_RIGHT //!< RightArrow on arrow keypad
#define KEYBOARD_END SDL_SCANCODE_END //!< End on arrow keypad
#define KEYBOARD_DOWN SDL_SCANCODE_DOWN //!< DownArrow on arrow keypad
#define KEYBOARD_NEXT SDL_SCANCODE_PAGEDOWN //!< PgDn on arrow keypad
#define KEYBOARD_INSERT SDL_SCANCODE_INSERT //!< Insert on arrow keypad
#define KEYBOARD_DELETE SDL_SCANCODE_DELETE //!< Delete on arrow keypad
#define KEYBOARD_LWIN SDL_SCANCODE_LGUI //!< Left Windows key
#define KEYBOARD_RWIN SDL_SCANCODE_RGUI //!< Right Windows key
#define KEYBOARD_APPS SDL_SCANCODE_MENU //!< AppMenu key

#define JOYSTICK_UP 0
#define JOYSTICK_DOWN 1
#define JOYSTICK_LEFT 2
#define JOYSTICK_RIGHT 3
#define JOYSTICK_BUTTON(x) (4 + (x))

#define NUMBER_OF_JOYSTICK_DIRECTIONS 4 //!< Number of joystick directions (up down left right)

#define JOYSTICK_AXIS_THRESHOLD 3200

//! The third joystick button can be used for leaving the winner screen
#define JOYSTICK_BUTTON_MENU_PREVIOUS JOYSTICK_BUTTON(1)
#define JOYSTICK_BUTTON_MENU_NEXT JOYSTICK_BUTTON(0)

//! The 9th joystick button (this usually is the "start" button) can be used for leaving the winner screen
//! On xbox controller, the start button could be the 7th button (see tracker item #2907122)
#define JOYSTICK_BUTTON_BREAK JOYSTICK_BUTTON(8) // pause
#define JOYSTICK_BUTTON_START JOYSTICK_BUTTON(9) // start

typedef struct SDLJOYSTATE
{
    LONG lX; /* x-axis position              */
    LONG lY; /* y-axis position              */
    LONG lZ; /* z-axis position              */
    LONG lRx; /* x-axis rotation              */
    LONG lRy; /* y-axis rotation              */
    LONG lRz; /* z-axis rotation              */
    LONG rglSlider[2]; /* extra axes positions         */
    DWORD rgdwPOV[4]; /* POV directions               */
    BYTE rgbButtons[32]; /* 32 buttons                   */
} SDLJOYSTATE, *LPSDLJOYSTATE;

//! The SJoystick structure contains information about one joystick created by SDLInput.

struct SJoystick
{
    SDL_Joystick* pDevice; //!< Pointer on the SDLInput input device
    SDLJOYSTATE State; //!< Most recent state of the joystick, filled on last update (has been DIJOYSTATE)
    bool Opened; //!< Is the joystick _supposed_ to be opened? Updated on each general update.
};

class CInputSDL
{
private:
    bool m_Ready;
    HINSTANCE m_hInstance; //!< Handle to the application instance (needed to use SDLInput)
    HWND m_hWnd; //!< Handle to the window (needed to use SDLInput)

    bool m_KeyboardOpened; //!< Is the system keyboard _supposed_ to be opened?
    char m_KeyState[MAX_KEYS]; //!< State of each key on the system keyboard
    char m_KeyFriendlyName[MAX_KEYS][MAX_PATH]; //!< More friendly name for each key

    std::vector<SJoystick*> m_pJoysticks; //!< All joystick SDLInput devices installed in the system

    bool UpdateDevice(void* pState);
    bool UpdateDevice(SDL_Joystick* pDevice, void* pState) const;
    void MakeKeyFriendlyNames();

    int m_joystickCount;

public:
    CInputSDL();
    ~CInputSDL();
    void SetWindowHandle(HWND hWnd);
    void SetInstanceHandle(HINSTANCE hInstance);
    bool Create();
    void Destroy();
    void OpenKeyboard();
    bool IsKeyboardOpened() const;
    void CloseKeyboard();
    void UpdateKeyboard();
    bool GetKey(int Key) const;
    void SetKey(int Key, bool KeySet);
    const char* GetKeyFriendlyName(int Key) const;
    int GetJoystickCount() const;
    void OpenJoystick(int Joystick);
    bool IsJoystickOpened(int Joystick);
    void CloseJoystick(int Joystick);
    void UpdateJoystick(int Joystick) const;
    int GetJoystickAxisX(int Joystick);
    int GetJoystickAxisY(int Joystick);
    bool GetJoystickButton(int Joystick, int Button);
    void SetJoystickAxisX(int Joystick, int AxisX);
    void SetJoystickAxisY(int Joystick, int AxisY);
    void SetJoystickButton(int Joystick, int Button, bool onoff);

    bool TestUp(int Joystick);
    bool TestDown(int Joystick);
    bool TestLeft(int Joystick);
    bool TestRight(int Joystick);
    bool TestNext(int Joystick);
    bool TestPrevious(int Joystick);
    bool TestBreak(int Joystick);
    bool TestStart(int Joystick);
};

typedef CInputSDL InputClass;

#endif // __CSDLINPUT_H__
