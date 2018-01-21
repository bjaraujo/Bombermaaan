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

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MAX_JOYSTICK_BUTTONS   32                       //!< Maximum number of buttons a joystick can have
#define MAX_KEYS               SDLK_LAST                //!< Maximum number of keys a keyboard can have

#define KEYBOARD_ESCAPE        SDLK_ESCAPE              //!< ID for the escape key
#define KEYBOARD_1             SDLK_1                   //!< ID for the 1 key
#define KEYBOARD_2             SDLK_2
#define KEYBOARD_3             SDLK_3
#define KEYBOARD_4             SDLK_4
#define KEYBOARD_5             SDLK_5
#define KEYBOARD_6             SDLK_6
#define KEYBOARD_7             SDLK_7
#define KEYBOARD_8             SDLK_8
#define KEYBOARD_9             SDLK_9
#define KEYBOARD_0             SDLK_0
#define KEYBOARD_MINUS         SDLK_MINUS               //!< - on main keyboard
#define KEYBOARD_EQUALS        SDLK_EQUALS
#define KEYBOARD_BACK          SDLK_BACKSPACE           //!< backspace
#define KEYBOARD_TAB           SDLK_TAB
#define KEYBOARD_Q             SDLK_q
#define KEYBOARD_W             SDLK_w
#define KEYBOARD_E             SDLK_e
#define KEYBOARD_R             SDLK_r
#define KEYBOARD_T             SDLK_t
#define KEYBOARD_Y             SDLK_y
#define KEYBOARD_U             SDLK_u
#define KEYBOARD_I             SDLK_i
#define KEYBOARD_O             SDLK_o
#define KEYBOARD_P             SDLK_p
#define KEYBOARD_LBRACKET      SDLK_LEFTPAREN
#define KEYBOARD_RBRACKET      SDLK_RIGHTPAREN
#define KEYBOARD_RETURN        SDLK_RETURN              //!< Enter on main keyboard
#define KEYBOARD_LCONTROL      SDLK_LCTRL
#define KEYBOARD_A             SDLK_a
#define KEYBOARD_S             SDLK_s
#define KEYBOARD_D             SDLK_d
#define KEYBOARD_F             SDLK_f
#define KEYBOARD_G             SDLK_g
#define KEYBOARD_H             SDLK_h
#define KEYBOARD_J             SDLK_j
#define KEYBOARD_K             SDLK_k
#define KEYBOARD_L             SDLK_l
#define KEYBOARD_SEMICOLON     SDLK_SEMICOLON
#define KEYBOARD_APOSTROPHE    SDLK_QUOTE
#define KEYBOARD_GRAVE         SDLK_BACKQUOTE           //!< accent grave
#define KEYBOARD_LSHIFT        SDLK_LSHIFT
#define KEYBOARD_BACKSLASH     SDLK_BACKSLASH
#define KEYBOARD_Z             SDLK_z
#define KEYBOARD_X             SDLK_x
#define KEYBOARD_C             SDLK_c
#define KEYBOARD_V             SDLK_v
#define KEYBOARD_B             SDLK_b
#define KEYBOARD_N             SDLK_n
#define KEYBOARD_M             SDLK_m
#define KEYBOARD_COMMA         SDLK_COMMA
#define KEYBOARD_PERIOD        SDLK_PERIOD              //!< . on main keyboard
#define KEYBOARD_SLASH         SDLK_SLASH               //!< / on main keyboard
#define KEYBOARD_RSHIFT        SDLK_RSHIFT
#define KEYBOARD_MULTIPLY      SDLK_KP_MULTIPLY         //!< * on numeric keypad
#define KEYBOARD_LMENU         SDLK_LALT                //!< left Alt
#define KEYBOARD_SPACE         SDLK_SPACE
#define KEYBOARD_CAPITAL       SDLK_CAPSLOCK
#define KEYBOARD_F1            SDLK_F1
#define KEYBOARD_F2            SDLK_F2
#define KEYBOARD_F3            SDLK_F3
#define KEYBOARD_F4            SDLK_F4
#define KEYBOARD_F5            SDLK_F5
#define KEYBOARD_F6            SDLK_F6
#define KEYBOARD_F7            SDLK_F7
#define KEYBOARD_F8            SDLK_F8
#define KEYBOARD_F9            SDLK_F9
#define KEYBOARD_F10           SDLK_F10
#define KEYBOARD_NUMLOCK       SDLK_NUMLOCK
#define KEYBOARD_SCROLL        SDLK_SCROLLOCK           //!< Scroll Lock
#define KEYBOARD_NUMPAD7       SDLK_KP7
#define KEYBOARD_NUMPAD8       SDLK_KP8
#define KEYBOARD_NUMPAD9       SDLK_KP9
#define KEYBOARD_SUBTRACT      SDLK_KP_MINUS            //!< - on numeric keypad
#define KEYBOARD_NUMPAD4       SDLK_KP4
#define KEYBOARD_NUMPAD5       SDLK_KP5
#define KEYBOARD_NUMPAD6       SDLK_KP6
#define KEYBOARD_ADD           SDLK_KP_PLUS             //!< + on numeric keypad
#define KEYBOARD_NUMPAD1       SDLK_KP1
#define KEYBOARD_NUMPAD2       SDLK_KP2
#define KEYBOARD_NUMPAD3       SDLK_KP3
#define KEYBOARD_NUMPAD0       SDLK_KP0
#define KEYBOARD_DECIMAL       SDLK_KP_PERIOD           //!< . on numeric keypad
#define KEYBOARD_F11           SDLK_F11
#define KEYBOARD_F12           SDLK_F12
#define KEYBOARD_NUMPADENTER   SDLK_KP_ENTER            //!< Enter on numeric keypad
#define KEYBOARD_RCONTROL      SDLK_RCTRL
#define KEYBOARD_DIVIDE        SDLK_KP_DIVIDE           //!< / on numeric keypad
#define KEYBOARD_SYSRQ         SDLK_SYSREQ
#define KEYBOARD_RMENU         SDLK_RALT                //!< right Alt
#define KEYBOARD_PAUSE         SDLK_BREAK               //!< Pause
#define KEYBOARD_HOME          SDLK_HOME                //!< Home on arrow keypad
#define KEYBOARD_UP            SDLK_UP                  //!< UpArrow on arrow keypad
#define KEYBOARD_PRIOR         SDLK_PAGEUP              //!< PgUp on arrow keypad
#define KEYBOARD_LEFT          SDLK_LEFT                //!< LeftArrow on arrow keypad
#define KEYBOARD_RIGHT         SDLK_RIGHT               //!< RightArrow on arrow keypad
#define KEYBOARD_END           SDLK_END                 //!< End on arrow keypad
#define KEYBOARD_DOWN          SDLK_DOWN                //!< DownArrow on arrow keypad
#define KEYBOARD_NEXT          SDLK_PAGEDOWN            //!< PgDn on arrow keypad
#define KEYBOARD_INSERT        SDLK_INSERT              //!< Insert on arrow keypad
#define KEYBOARD_DELETE        SDLK_DELETE              //!< Delete on arrow keypad
#define KEYBOARD_LWIN          SDLK_LSUPER              //!< Left Windows key
#define KEYBOARD_RWIN          SDLK_RSUPER              //!< Right Windows key
#define KEYBOARD_APPS          SDLK_MENU                //!< AppMenu key

#define JOYSTICK_UP         0
#define JOYSTICK_DOWN       1
#define JOYSTICK_LEFT       2
#define JOYSTICK_RIGHT      3
#define JOYSTICK_BUTTON(x)  (4 + (x))

#define NUMBER_OF_JOYSTICK_DIRECTIONS     4     //!< Number of joystick directions (up down left right)

#define JOYSTICK_AXIS_THRESHOLD 3200

//! The third joystick button can be used for leaving the winner screen
#define JOYSTICK_BUTTON_MENU_PREVIOUS   JOYSTICK_BUTTON(1)
#define JOYSTICK_BUTTON_MENU_NEXT       JOYSTICK_BUTTON(0)

//! The 9th joystick button (this usually is the "start" button) can be used for leaving the winner screen
//! On xbox controller, the start button could be the 7th button (see tracker item #2907122)
#define JOYSTICK_BUTTON_BREAK       JOYSTICK_BUTTON(8) // pause
#define JOYSTICK_BUTTON_START       JOYSTICK_BUTTON(9) // start

typedef struct SDLJOYSTATE {
    LONG    lX;                     /* x-axis position              */
    LONG    lY;                     /* y-axis position              */
    LONG    lZ;                     /* z-axis position              */
    LONG    lRx;                    /* x-axis rotation              */
    LONG    lRy;                    /* y-axis rotation              */
    LONG    lRz;                    /* z-axis rotation              */
    LONG    rglSlider[2];           /* extra axes positions         */
    DWORD   rgdwPOV[4];             /* POV directions               */
    BYTE    rgbButtons[32];         /* 32 buttons                   */
} SDLJOYSTATE, *LPSDLJOYSTATE;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The SJoystick structure contains information about one joystick created by SDLInput.

struct SJoystick
{
    SDL_Joystick    *pDevice;        //!< Pointer on the SDLInput input device
    SDLJOYSTATE      State;          //!< Most recent state of the joystick, filled on last update (has been DIJOYSTATE)
    bool             Opened;         //!< Is the joystick _supposed_ to be opened? Updated on each general update.
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CInputSDL
{
private:

    bool                    m_Ready;
    HINSTANCE               m_hInstance;    //!< Handle to the application instance (needed to use SDLInput)
    HWND                    m_hWnd;         //!< Handle to the window (needed to use SDLInput)

    bool                    m_KeyboardOpened;               //!< Is the system keyboard _supposed_ to be opened?
    char                    m_KeyState[MAX_KEYS];          //!< State of each key on the system keyboard
    char                    m_KeyFriendlyName[MAX_KEYS][MAX_PATH]; //!< More friendly name for each key

    std::vector<SJoystick*> m_pJoysticks;   //!< All joystick SDLInput devices installed in the system

    bool                    UpdateDevice(void *pState, int StateSize);
    bool                    UpdateDevice(SDL_Joystick *pDevice, void *pState, int StateSize);
    void                    MakeKeyFriendlyNames(void);

    int                     m_joystickCount;

public:

    CInputSDL(void);
    ~CInputSDL(void);
    inline void             SetWindowHandle(HWND hWnd);
    inline void             SetInstanceHandle(HINSTANCE hInstance);
    bool                    Create(void);
    void                    Destroy(void);
    inline void             OpenKeyboard(void);
    inline bool             IsKeyboardOpened(void);
    inline void             CloseKeyboard(void);
    void                    UpdateKeyboard(void);
    inline bool             GetKey(int Key);
    inline void             SetKey(int Key, bool KeySet);
    inline const char*      GetKeyFriendlyName(int Key);
    inline int              GetJoystickCount(void);
    inline void             OpenJoystick(int Joystick);
    inline bool             IsJoystickOpened(int Joystick);
    inline void             CloseJoystick(int Joystick);
    void                    UpdateJoystick(int Joystick);
    inline int              GetJoystickAxisX(int Joystick);
    inline int              GetJoystickAxisY(int Joystick);
    inline bool             GetJoystickButton(int Joystick, int Button);
    inline void             SetJoystickAxisX(int Joystick, int AxisX);
    inline void             SetJoystickAxisY(int Joystick, int AxisY);
    inline void             SetJoystickButton(int Joystick, int Button, bool onoff);

    inline bool             TestUp(int Joystick);
    inline bool             TestDown(int Joystick);
    inline bool             TestLeft(int Joystick);
    inline bool             TestRight(int Joystick);
    inline bool             TestNext(int Joystick);
    inline bool             TestPrevious(int Joystick);
    inline bool             TestBreak(int Joystick);
    inline bool             TestStart(int Joystick);

};

typedef CInputSDL InputClass;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CInputSDL::SetWindowHandle(HWND hWnd)
{
    m_hWnd = hWnd;
}

inline void CInputSDL::SetInstanceHandle(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
}

inline void CInputSDL::OpenKeyboard(void)
{
    m_KeyboardOpened = true;
}

inline bool CInputSDL::IsKeyboardOpened(void)
{
    // Return the opened state of the keyboard
    return m_KeyboardOpened;
}

inline void CInputSDL::CloseKeyboard(void)
{
    m_KeyboardOpened = false;
}

inline bool CInputSDL::GetKey(int Key)
{
    // Assert the key number is correct
    ASSERT(Key >= 0 && Key < MAX_KEYS);

    // Return the state of the key
    return (m_KeyState[Key] & 0x80) != 0;
}

inline void CInputSDL::SetKey(int Key, bool KeySet)
{
    // Assert the key number is correct
    ASSERT(Key >= 0 && Key < MAX_KEYS);

    // Set/remove state of the key
    if (KeySet) {
        m_KeyState[Key] |= 0x80;
    }
    else {
        m_KeyState[Key] &= ~0x80;
    }

    return;
}

inline const char* CInputSDL::GetKeyFriendlyName(int Key)
{
    // Assert the key number is correct
    ASSERT(Key >= 0 && Key < MAX_KEYS);

    // Return the name of the key
    return m_KeyFriendlyName[Key];
}

inline int CInputSDL::GetJoystickCount(void)
{
    // Return the number of joysticks installed on the system
    return m_pJoysticks.size();
}

inline void CInputSDL::OpenJoystick(int Joystick)
{

    // Check if the joystick number is correct
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if (m_pJoysticks[Joystick]->Opened)
            return;

        // Try to acquire the joystick
        m_pJoysticks[Joystick]->pDevice = SDL_JoystickOpen(Joystick);

        // Set the opened state according to the return value
        m_pJoysticks[Joystick]->Opened = (m_pJoysticks[Joystick]->pDevice != NULL);

    }
}

inline bool CInputSDL::IsJoystickOpened(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the opened state of this joystick
    return m_pJoysticks[Joystick]->Opened;
}

inline void CInputSDL::CloseJoystick(int Joystick)
{
    // Check if the joystick number is correct
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        // Release access to this joystick
        SDL_JoystickClose(m_pJoysticks[Joystick]->pDevice);

        // We are sure this joystick is not opened */
        m_pJoysticks[Joystick]->Opened = false;
        m_pJoysticks[Joystick]->pDevice = NULL;

    }

}

inline int CInputSDL::GetJoystickAxisX(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the value of the X axis of this joystick
    return m_pJoysticks[Joystick]->State.lX;
}

inline int CInputSDL::GetJoystickAxisY(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the value of the Y axis of this joystick
    return m_pJoysticks[Joystick]->State.lY;
}

inline bool CInputSDL::GetJoystickButton(int Joystick, int Button)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Assert the button number is correct
    ASSERT(Button >= 0 && Button < MAX_JOYSTICK_BUTTONS);

    // Return the state of the specified button on this joystick
    return (m_pJoysticks[Joystick]->State.rgbButtons[Button] & 0x80) != 0;
}

inline void CInputSDL::SetJoystickAxisX(int Joystick, int AxisX)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Set the value of the X axis of this joystick
    m_pJoysticks[Joystick]->State.lX = AxisX;

    return;
}

inline void CInputSDL::SetJoystickAxisY(int Joystick, int AxisY)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Set the value of the Y axis of this joystick
    m_pJoysticks[Joystick]->State.lY = AxisY;

    return;
}

inline void CInputSDL::SetJoystickButton(int Joystick, int Button, bool onoff)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Assert the button number is correct
    ASSERT(Button >= 0 && Button < MAX_JOYSTICK_BUTTONS);

    // Return the state of the specified button on this joystick
    if (onoff) {
        m_pJoysticks[Joystick]->State.rgbButtons[Button] |= 0x80;
    }
    else {
        m_pJoysticks[Joystick]->State.rgbButtons[Button] &= ~0x80;
    }

    return;
}

inline bool CInputSDL::TestUp(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        m_joystickCount++;

        if (m_joystickCount > 200)
        {

            if (m_pJoysticks[Joystick]->State.lY < -JOYSTICK_AXIS_THRESHOLD)
            {
                m_joystickCount = 0;
                return true;
            }

        }

    }

    return false;

}

inline bool CInputSDL::TestDown(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        m_joystickCount++;

        if (m_joystickCount > 200)
        {

            if (m_pJoysticks[Joystick]->State.lY > +JOYSTICK_AXIS_THRESHOLD)
            {
                m_joystickCount = 0;
                return true;
            }

        }

    }

    return false;

}

inline bool CInputSDL::TestLeft(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        m_joystickCount++;

        if (m_joystickCount > 200)
        {

            if (m_pJoysticks[Joystick]->State.lX < -JOYSTICK_AXIS_THRESHOLD)
            {
                m_joystickCount = 0;
                return true;
            }

        }

    }

    return false;

}

inline bool CInputSDL::TestRight(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        m_joystickCount++;

        if (m_joystickCount > 200)
        {

            if (m_pJoysticks[Joystick]->State.lX > +JOYSTICK_AXIS_THRESHOLD)
            {
                m_joystickCount = 0;
                return true;
            }

        }

    }

    return false;

}

inline bool CInputSDL::TestNext(int Joystick)
{


    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if ((m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_MENU_NEXT - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0)
            return true;

    }

    return false;

}

inline bool CInputSDL::TestPrevious(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if ((m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_MENU_PREVIOUS - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0)
            return true;

    }

    return false;

}

inline bool CInputSDL::TestBreak(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if ((m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_BREAK - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0)
            return true;

    }

    return false;

}

inline bool CInputSDL::TestStart(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if ((m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_START - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0)
            return true;

    }

    return false;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CSDLINPUT_H__
