/************************************************************************************

    Copyright (C) 2017 Billy Araujo

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
 *  \file CAllegroInput.h
 *  \brief Header file of the Allegro input
 */

#ifndef __CALLEGROINPUT_H__
#define __CALLEGROINPUT_H__

#include "allegro.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MAX_JOYSTICK_BUTTONS   32                      //!< Maximum number of buttons a joystick can have
#define MAX_KEYS               ALLEGROK_LAST               //!< Maximum number of keys a keyboard can have

#define KEYBOARD_ESCAPE        ALLEGROK_ESCAPE              //!< ID for the escape key
#define KEYBOARD_1             ALLEGROK_1                   //!< ID for the 1 key
#define KEYBOARD_2             ALLEGROK_2
#define KEYBOARD_3             ALLEGROK_3
#define KEYBOARD_4             ALLEGROK_4
#define KEYBOARD_5             ALLEGROK_5
#define KEYBOARD_6             ALLEGROK_6
#define KEYBOARD_7             ALLEGROK_7
#define KEYBOARD_8             ALLEGROK_8
#define KEYBOARD_9             ALLEGROK_9
#define KEYBOARD_0             ALLEGROK_0
#define KEYBOARD_MINUS         ALLEGROK_MINUS               //!< - on main keyboard 
#define KEYBOARD_EQUALS        ALLEGROK_EQUALS
#define KEYBOARD_BACK          ALLEGROK_BACKSPACE           //!< backspace 
#define KEYBOARD_TAB           ALLEGROK_TAB
#define KEYBOARD_Q             ALLEGROK_q
#define KEYBOARD_W             ALLEGROK_w
#define KEYBOARD_E             ALLEGROK_e
#define KEYBOARD_R             ALLEGROK_r
#define KEYBOARD_T             ALLEGROK_t
#define KEYBOARD_Y             ALLEGROK_y
#define KEYBOARD_U             ALLEGROK_u
#define KEYBOARD_I             ALLEGROK_i
#define KEYBOARD_O             ALLEGROK_o
#define KEYBOARD_P             ALLEGROK_p                  
#define KEYBOARD_LBRACKET      ALLEGROK_LEFTPAREN
#define KEYBOARD_RBRACKET      ALLEGROK_RIGHTPAREN
#define KEYBOARD_RETURN        ALLEGROK_RETURN              //!< Enter on main keyboard 
#define KEYBOARD_LCONTROL      ALLEGROK_LCTRL
#define KEYBOARD_A             ALLEGROK_a
#define KEYBOARD_S             ALLEGROK_s
#define KEYBOARD_D             ALLEGROK_d
#define KEYBOARD_F             ALLEGROK_f                    
#define KEYBOARD_G             ALLEGROK_g                    
#define KEYBOARD_H             ALLEGROK_h                    
#define KEYBOARD_J             ALLEGROK_j                    
#define KEYBOARD_K             ALLEGROK_k                    
#define KEYBOARD_L             ALLEGROK_l                    
#define KEYBOARD_SEMICOLON     ALLEGROK_SEMICOLON            
#define KEYBOARD_APOSTROPHE    ALLEGROK_QUOTE
#define KEYBOARD_GRAVE         ALLEGROK_BACKQUOTE           //!< accent grave 
#define KEYBOARD_LSHIFT        ALLEGROK_LSHIFT               
#define KEYBOARD_BACKSLASH     ALLEGROK_BACKSLASH            
#define KEYBOARD_Z             ALLEGROK_z                    
#define KEYBOARD_X             ALLEGROK_x                    
#define KEYBOARD_C             ALLEGROK_c                    
#define KEYBOARD_V             ALLEGROK_v                    
#define KEYBOARD_B             ALLEGROK_b                    
#define KEYBOARD_N             ALLEGROK_n                    
#define KEYBOARD_M             ALLEGROK_m                    
#define KEYBOARD_COMMA         ALLEGROK_COMMA                
#define KEYBOARD_PERIOD        ALLEGROK_PERIOD              //!< . on main keyboard 
#define KEYBOARD_SLASH         ALLEGROK_SLASH               //!< / on main keyboard 
#define KEYBOARD_RSHIFT        ALLEGROK_RSHIFT               
#define KEYBOARD_MULTIPLY      ALLEGROK_KP_MULTIPLY         //!< * on numeric keypad 
#define KEYBOARD_LMENU         ALLEGROK_LALT                //!< left Alt 
#define KEYBOARD_SPACE         ALLEGROK_SPACE                
#define KEYBOARD_CAPITAL       ALLEGROK_CAPSLOCK
#define KEYBOARD_F1            ALLEGROK_F1                   
#define KEYBOARD_F2            ALLEGROK_F2                   
#define KEYBOARD_F3            ALLEGROK_F3                   
#define KEYBOARD_F4            ALLEGROK_F4                   
#define KEYBOARD_F5            ALLEGROK_F5                   
#define KEYBOARD_F6            ALLEGROK_F6                   
#define KEYBOARD_F7            ALLEGROK_F7                   
#define KEYBOARD_F8            ALLEGROK_F8                   
#define KEYBOARD_F9            ALLEGROK_F9                   
#define KEYBOARD_F10           ALLEGROK_F10                  
#define KEYBOARD_NUMLOCK       ALLEGROK_NUMLOCK              
#define KEYBOARD_SCROLL        ALLEGROK_SCROLLOCK           //!< Scroll Lock 
#define KEYBOARD_NUMPAD7       ALLEGROK_KP7              
#define KEYBOARD_NUMPAD8       ALLEGROK_KP8              
#define KEYBOARD_NUMPAD9       ALLEGROK_KP9
#define KEYBOARD_SUBTRACT      ALLEGROK_KP_MINUS            //!< - on numeric keypad 
#define KEYBOARD_NUMPAD4       ALLEGROK_KP4
#define KEYBOARD_NUMPAD5       ALLEGROK_KP5
#define KEYBOARD_NUMPAD6       ALLEGROK_KP6
#define KEYBOARD_ADD           ALLEGROK_KP_PLUS             //!< + on numeric keypad 
#define KEYBOARD_NUMPAD1       ALLEGROK_KP1              
#define KEYBOARD_NUMPAD2       ALLEGROK_KP2              
#define KEYBOARD_NUMPAD3       ALLEGROK_KP3              
#define KEYBOARD_NUMPAD0       ALLEGROK_KP0              
#define KEYBOARD_DECIMAL       ALLEGROK_KP_PERIOD           //!< . on numeric keypad 
#define KEYBOARD_F11           ALLEGROK_F11                  
#define KEYBOARD_F12           ALLEGROK_F12                  
#define KEYBOARD_NUMPADENTER   ALLEGROK_KP_ENTER            //!< Enter on numeric keypad 
#define KEYBOARD_RCONTROL      ALLEGROK_RCTRL
#define KEYBOARD_DIVIDE        ALLEGROK_KP_DIVIDE           //!< / on numeric keypad 
#define KEYBOARD_SYSRQ         ALLEGROK_SYSREQ                
#define KEYBOARD_RMENU         ALLEGROK_RALT                //!< right Alt 
#define KEYBOARD_PAUSE         ALLEGROK_BREAK               //!< Pause 
#define KEYBOARD_HOME          ALLEGROK_HOME                //!< Home on arrow keypad 
#define KEYBOARD_UP            ALLEGROK_UP                  //!< UpArrow on arrow keypad 
#define KEYBOARD_PRIOR         ALLEGROK_PAGEUP              //!< PgUp on arrow keypad 
#define KEYBOARD_LEFT          ALLEGROK_LEFT                //!< LeftArrow on arrow keypad 
#define KEYBOARD_RIGHT         ALLEGROK_RIGHT               //!< RightArrow on arrow keypad 
#define KEYBOARD_END           ALLEGROK_END                 //!< End on arrow keypad 
#define KEYBOARD_DOWN          ALLEGROK_DOWN                //!< DownArrow on arrow keypad 
#define KEYBOARD_NEXT          ALLEGROK_PAGEDOWN            //!< PgDn on arrow keypad 
#define KEYBOARD_INSERT        ALLEGROK_INSERT              //!< Insert on arrow keypad 
#define KEYBOARD_DELETE        ALLEGROK_DELETE              //!< Delete on arrow keypad 
#define KEYBOARD_LWIN          ALLEGROK_LSUPER              //!< Left Windows key 
#define KEYBOARD_RWIN          ALLEGROK_RSUPER              //!< Right Windows key 
#define KEYBOARD_APPS          ALLEGROK_MENU                //!< AppMenu key 

#define JOYSTICK_UP         0
#define JOYSTICK_DOWN       1
#define JOYSTICK_LEFT       2
#define JOYSTICK_RIGHT      3
#define JOYSTICK_BUTTON(x)  (4 + (x))

#define NUMBER_OF_JOYSTICK_DIRECTIONS     4     //!< Number of joystick directions (up down left right)

#define JOYSTICK_AXIS_THRESHOLD 3200

//! The third joystick button can be used for leaving the winner screen
#define JOYSTICK_BUTTON_MENU_PREVIOUS   JOYSTICK_BUTTON(1)
#define JOYSTICK_BUTTON_MENU_NEXT        JOYSTICK_BUTTON(0)

//! The 9th joystick button (this usually is the "start" button) can be used for leaving the winner screen
//! On xbox controller, the start button could be the 7th button (see tracker item #2907122)
#define JOYSTICK_BUTTON_BREAK       JOYSTICK_BUTTON(8) // pause
#define JOYSTICK_BUTTON_START       JOYSTICK_BUTTON(9) // start

typedef struct AllegroJOYSTATE {
    LONG    lX;                     /* x-axis position              */
    LONG    lY;                     /* y-axis position              */
    LONG    lZ;                     /* z-axis position              */
    LONG    lRx;                    /* x-axis rotation              */
    LONG    lRy;                    /* y-axis rotation              */
    LONG    lRz;                    /* z-axis rotation              */
    LONG    rglSlider[2];           /* extra axes positions         */
    DWORD   rgdwPOV[4];             /* POV directions               */
    BYTE    rgbButtons[32];         /* 32 buttons                   */
} AllegroJOYSTATE, *LPAllegroJOYSTATE;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The SJoystick structure contains information about one joystick created by AllegroInput.

struct SJoystick
{
    Allegro_Joystick    *pDevice;        //!< Pointer on the AllegroInput input device
    AllegroJOYSTATE      State;          //!< Most recent state of the joystick, filled on last update (has been DIJOYSTATE)
    bool             Opened;         //!< Is the joystick _supposed_ to be opened? Updated on each general update.
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CAllegroInput
{
private:

    bool                    m_Ready;
    HINSTANCE               m_hInstance;    //!< Handle to the application instance (needed to use AllegroInput)
    HWND                    m_hWnd;         //!< Handle to the window (needed to use AllegroInput)

    bool                    m_KeyboardOpened;               //!< Is the system keyboard _supposed_ to be opened?
    char                    m_KeyState[MAX_KEYS];          //!< State of each key on the system keyboard
    char                    m_KeyFriendlyName[MAX_KEYS][MAX_PATH]; //!< More friendly name for each key

    vector<SJoystick*>      m_pJoysticks;   //!< All joystick AllegroInput devices installed in the system

    bool                    UpdateDevice(void *pState, int StateSize);
    bool                    UpdateDevice(Allegro_Joystick *pDevice, void *pState, int StateSize);
    void                    MakeKeyFriendlyNames(void);

    int                     m_joystickCount;

public:

    CAllegroInput(void);
    ~CAllegroInput(void);
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

    inline bool                TestUp(int Joystick);
    inline bool                TestDown(int Joystick);
    inline bool                TestLeft(int Joystick);
    inline bool                TestRight(int Joystick);
    inline bool                TestNext(int Joystick);
    inline bool                TestPrevious(int Joystick);
    inline bool                TestBreak(int Joystick);
    inline bool                TestStart(int Joystick);

};

typedef CAllegroInput InputClass;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CAllegroInput::SetWindowHandle(HWND hWnd)
{
    m_hWnd = hWnd;
}

inline void CAllegroInput::SetInstanceHandle(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
}

inline void CAllegroInput::OpenKeyboard(void)
{
    m_KeyboardOpened = true;
}

inline bool CAllegroInput::IsKeyboardOpened(void)
{
    // Return the opened state of the keyboard
    return m_KeyboardOpened;
}

inline void CAllegroInput::CloseKeyboard(void)
{
    m_KeyboardOpened = false;
}

inline bool CAllegroInput::GetKey(int Key)
{
    // Assert the key number is correct
    ASSERT(Key >= 0 && Key < MAX_KEYS);

    // Return the state of the key
    return (m_KeyState[Key] & 0x80) != 0;
}

inline void CAllegroInput::SetKey(int Key, bool KeySet)
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

inline const char* CAllegroInput::GetKeyFriendlyName(int Key)
{
    // Assert the key number is correct
    ASSERT(Key >= 0 && Key < MAX_KEYS);

    // Return the name of the key
    return m_KeyFriendlyName[Key];
}

inline int CAllegroInput::GetJoystickCount(void)
{
    // Return the number of joysticks installed on the system
    return m_pJoysticks.size();
}

inline void CAllegroInput::OpenJoystick(int Joystick)
{

    // Check if the joystick number is correct
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if (m_pJoysticks[Joystick]->Opened)
            return;

        // Try to acquire the joystick
        m_pJoysticks[Joystick]->pDevice = Allegro_JoystickOpen(Joystick);

        // Set the opened state according to the return value
        m_pJoysticks[Joystick]->Opened = (m_pJoysticks[Joystick]->pDevice != NULL);

    }
}

inline bool CAllegroInput::IsJoystickOpened(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the opened state of this joystick
    return m_pJoysticks[Joystick]->Opened;
}

inline void CAllegroInput::CloseJoystick(int Joystick)
{
    // Check if the joystick number is correct
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        // Release access to this joystick
        Allegro_JoystickClose(m_pJoysticks[Joystick]->pDevice);

        // We are sure this joystick is not opened */
        m_pJoysticks[Joystick]->Opened = false;
        m_pJoysticks[Joystick]->pDevice = NULL;

    }

}

inline int CAllegroInput::GetJoystickAxisX(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the value of the X axis of this joystick
    return m_pJoysticks[Joystick]->State.lX;
}

inline int CAllegroInput::GetJoystickAxisY(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Return the value of the Y axis of this joystick
    return m_pJoysticks[Joystick]->State.lY;
}

inline bool CAllegroInput::GetJoystickButton(int Joystick, int Button)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Assert the button number is correct
    ASSERT(Button >= 0 && Button < MAX_JOYSTICK_BUTTONS);

    // Return the state of the specified button on this joystick
    return (m_pJoysticks[Joystick]->State.rgbButtons[Button] & 0x80) != 0;
}

inline void CAllegroInput::SetJoystickAxisX(int Joystick, int AxisX)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Set the value of the X axis of this joystick
    m_pJoysticks[Joystick]->State.lX = AxisX;

    return;
}

inline void CAllegroInput::SetJoystickAxisY(int Joystick, int AxisY)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < (int)m_pJoysticks.size());

    // Set the value of the Y axis of this joystick
    m_pJoysticks[Joystick]->State.lY = AxisY;

    return;
}

inline void CAllegroInput::SetJoystickButton(int Joystick, int Button, bool onoff)
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

inline bool CAllegroInput::TestUp(int Joystick)
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

inline bool CAllegroInput::TestDown(int Joystick)
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

inline bool CAllegroInput::TestLeft(int Joystick)
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

inline bool CAllegroInput::TestRight(int Joystick)
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

inline bool CAllegroInput::TestNext(int Joystick)
{

    
    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if ((m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_MENU_NEXT - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0)
            return true;

    }

    return false;

}

inline bool CAllegroInput::TestPrevious(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if ((m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_MENU_PREVIOUS - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0)
            return true;

    }

    return false;

}

inline bool CAllegroInput::TestBreak(int Joystick)
{

    if (Joystick >= 0 && Joystick < (int)m_pJoysticks.size())
    {

        if ((m_pJoysticks[Joystick]->State.rgbButtons[JOYSTICK_BUTTON_BREAK - NUMBER_OF_JOYSTICK_DIRECTIONS] & 0x80) != 0)
            return true;

    }

    return false;

}

inline bool CAllegroInput::TestStart(int Joystick)
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

#endif // __CALLEGROINPUT_H__
