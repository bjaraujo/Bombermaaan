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
 *  \file CInputDX.h
 *  \brief Header file of the direct input on Windows
 */

#ifndef __CInputDX_H__
#define __CInputDX_H__

#define DIRECTINPUT_VERSION 0x0800 // Use DirectInput 7
#include <DINPUT.H>

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MAX_JOYSTICK_BUTTONS    32                      //!< Maximum number of buttons a joystick can have
#define MAX_KEYS                256                     //!< Maximum number of keys a keyboard can have

#define KEYBOARD_ESCAPE        DIK_ESCAPE               //!< ID for the escape key
#define KEYBOARD_1             DIK_1                    //!< ID for the 1 key
#define KEYBOARD_2             DIK_2                    //!< ...
#define KEYBOARD_3             DIK_3                     
#define KEYBOARD_4             DIK_4                     
#define KEYBOARD_5             DIK_5                     
#define KEYBOARD_6             DIK_6                     
#define KEYBOARD_7             DIK_7                     
#define KEYBOARD_8             DIK_8                     
#define KEYBOARD_9             DIK_9                     
#define KEYBOARD_0             DIK_0                    
#define KEYBOARD_MINUS         DIK_MINUS                //!< - on main keyboard 
#define KEYBOARD_EQUALS        DIK_EQUALS               
#define KEYBOARD_BACK          DIK_BACK                 //!< backspace 
#define KEYBOARD_TAB           DIK_TAB                  
#define KEYBOARD_Q             DIK_Q                    
#define KEYBOARD_W             DIK_W                    
#define KEYBOARD_E             DIK_E                    
#define KEYBOARD_R             DIK_R                    
#define KEYBOARD_T             DIK_T                    
#define KEYBOARD_Y             DIK_Y                    
#define KEYBOARD_U             DIK_U                    
#define KEYBOARD_I             DIK_I                    
#define KEYBOARD_O             DIK_O                    
#define KEYBOARD_P             DIK_P                    
#define KEYBOARD_LBRACKET      DIK_LBRACKET             
#define KEYBOARD_RBRACKET      DIK_RBRACKET             
#define KEYBOARD_RETURN        DIK_RETURN               //!< Enter on main keyboard 
#define KEYBOARD_LCONTROL      DIK_LCONTROL             
#define KEYBOARD_A             DIK_A                    
#define KEYBOARD_S             DIK_S                    
#define KEYBOARD_D             DIK_D                    
#define KEYBOARD_F             DIK_F                    
#define KEYBOARD_G             DIK_G                    
#define KEYBOARD_H             DIK_H                    
#define KEYBOARD_J             DIK_J                    
#define KEYBOARD_K             DIK_K                    
#define KEYBOARD_L             DIK_L                    
#define KEYBOARD_SEMICOLON     DIK_SEMICOLON            
#define KEYBOARD_APOSTROPHE    DIK_APOSTROPHE           
#define KEYBOARD_GRAVE         DIK_GRAVE                //!< accent grave 
#define KEYBOARD_LSHIFT        DIK_LSHIFT               
#define KEYBOARD_BACKSLASH     DIK_BACKSLASH            
#define KEYBOARD_Z             DIK_Z                    
#define KEYBOARD_X             DIK_X                    
#define KEYBOARD_C             DIK_C                    
#define KEYBOARD_V             DIK_V                    
#define KEYBOARD_B             DIK_B                    
#define KEYBOARD_N             DIK_N                    
#define KEYBOARD_M             DIK_M                    
#define KEYBOARD_COMMA         DIK_COMMA                
#define KEYBOARD_PERIOD        DIK_PERIOD               //!< . on main keyboard 
#define KEYBOARD_SLASH         DIK_SLASH                //!< / on main keyboard 
#define KEYBOARD_RSHIFT        DIK_RSHIFT               
#define KEYBOARD_MULTIPLY      DIK_MULTIPLY             //!< * on numeric keypad 
#define KEYBOARD_LMENU         DIK_LMENU                //!< left Alt 
#define KEYBOARD_SPACE         DIK_SPACE                
#define KEYBOARD_CAPITAL       DIK_CAPITAL              
#define KEYBOARD_F1            DIK_F1                   
#define KEYBOARD_F2            DIK_F2                   
#define KEYBOARD_F3            DIK_F3                   
#define KEYBOARD_F4            DIK_F4                   
#define KEYBOARD_F5            DIK_F5                   
#define KEYBOARD_F6            DIK_F6                   
#define KEYBOARD_F7            DIK_F7                   
#define KEYBOARD_F8            DIK_F8                   
#define KEYBOARD_F9            DIK_F9                   
#define KEYBOARD_F10           DIK_F10                  
#define KEYBOARD_NUMLOCK       DIK_NUMLOCK              
#define KEYBOARD_SCROLL        DIK_SCROLL               //!< Scroll Lock 
#define KEYBOARD_NUMPAD7       DIK_NUMPAD7              
#define KEYBOARD_NUMPAD8       DIK_NUMPAD8              
#define KEYBOARD_NUMPAD9       DIK_NUMPAD9              
#define KEYBOARD_SUBTRACT      DIK_SUBTRACT             //!< - on numeric keypad 
#define KEYBOARD_NUMPAD4       DIK_NUMPAD4              
#define KEYBOARD_NUMPAD5       DIK_NUMPAD5              
#define KEYBOARD_NUMPAD6       DIK_NUMPAD6              
#define KEYBOARD_ADD           DIK_ADD                  //!< + on numeric keypad 
#define KEYBOARD_NUMPAD1       DIK_NUMPAD1              
#define KEYBOARD_NUMPAD2       DIK_NUMPAD2              
#define KEYBOARD_NUMPAD3       DIK_NUMPAD3              
#define KEYBOARD_NUMPAD0       DIK_NUMPAD0              
#define KEYBOARD_DECIMAL       DIK_DECIMAL              //!< . on numeric keypad 
#define KEYBOARD_F11           DIK_F11                  
#define KEYBOARD_F12           DIK_F12                  
#define KEYBOARD_NUMPADENTER   DIK_NUMPADENTER          //!< Enter on numeric keypad 
#define KEYBOARD_RCONTROL      DIK_RCONTROL             
#define KEYBOARD_DIVIDE        DIK_DIVIDE               //!< / on numeric keypad 
#define KEYBOARD_SYSRQ         DIK_SYSRQ                
#define KEYBOARD_RMENU         DIK_RMENU                //!< right Alt 
#define KEYBOARD_PAUSE         DIK_PAUSE                //!< Pause 
#define KEYBOARD_HOME          DIK_HOME                 //!< Home on arrow keypad 
#define KEYBOARD_UP            DIK_UP                   //!< UpArrow on arrow keypad 
#define KEYBOARD_PRIOR         DIK_PRIOR                //!< PgUp on arrow keypad 
#define KEYBOARD_LEFT          DIK_LEFT                 //!< LeftArrow on arrow keypad 
#define KEYBOARD_RIGHT         DIK_RIGHT                //!< RightArrow on arrow keypad 
#define KEYBOARD_END           DIK_END                  //!< End on arrow keypad 
#define KEYBOARD_DOWN          DIK_DOWN                 //!< DownArrow on arrow keypad 
#define KEYBOARD_NEXT          DIK_NEXT                 //!< PgDn on arrow keypad 
#define KEYBOARD_INSERT        DIK_INSERT               //!< Insert on arrow keypad 
#define KEYBOARD_DELETE        DIK_DELETE               //!< Delete on arrow keypad 
#define KEYBOARD_LWIN          DIK_LWIN                 //!< Left Windows key 
#define KEYBOARD_RWIN          DIK_RWIN                 //!< Right Windows key 
#define KEYBOARD_APPS          DIK_APPS                 //!< AppMenu key 

#define JOYSTICK_UP         0
#define JOYSTICK_DOWN       1
#define JOYSTICK_LEFT       2
#define JOYSTICK_RIGHT      3
#define JOYSTICK_BUTTON(x)  (4 + x)

#define NUMBER_OF_JOYSTICK_DIRECTIONS     4     //!< Number of joystick directions (up down left right)

#define JOYSTICK_AXIS_THRESHOLD 3200

//! The third joystick button can be used for leaving the winner screen
#define JOYSTICK_BUTTON_MENU_PREVIOUS   JOYSTICK_BUTTON(1)
#define JOYSTICK_BUTTON_MENU_NEXT		JOYSTICK_BUTTON(0)

//! The 9th joystick button (this usually is the "start" button) can be used for leaving the winner screen
//! On xbox controller, the start button could be the 7th button (see tracker item #2907122)
#define JOYSTICK_BUTTON_BREAK       JOYSTICK_BUTTON(8) // pause
#define JOYSTICK_BUTTON_START       JOYSTICK_BUTTON(9) // start

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The SJoystick structure contains information about one joystick created by DirectInput.

struct SJoystick
{
    LPDIRECTINPUTDEVICE8    pDevice;        //!< Pointer on the DirectInput input device
    DIJOYSTATE              State;          //!< Most recent state of the joystick, filled on last update
    bool                    Opened;         //!< Is the joystick _supposed_ to be opened? Updated on each general update.
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CInputDX
{
private:

    bool                    m_Ready;
    HINSTANCE               m_hInstance;    //!< Handle to the application instance (needed to use DirectInput)
    HWND                    m_hWnd;         //!< Handle to the window (needed to use DirectInput)
    LPDIRECTINPUT8          m_pDI;          //!< DirectInput object pointer
  
    LPDIRECTINPUTDEVICE8    m_pKeyboard;                    //!< DirectInput device of the system keyboard
    bool                    m_KeyboardOpened;               //!< Is the system keyboard _supposed_ to be opened?
    char                    m_KeyState [MAX_KEYS];          //!< State of each key on the system keyboard
    char                    m_KeyRealName [MAX_KEYS][MAX_PATH]; //!< Real name (the one given by Windows) of each key on the system keyboard
    char                    m_KeyFriendlyName [MAX_KEYS][MAX_PATH]; //!< More friendly name for each key

    vector<SJoystick*>      m_pJoysticks;   //!< All joystick DirectInput devices installed in Windows

    bool                    UpdateDevice (LPDIRECTINPUTDEVICE8 pDevice, void *pState, int StateSize);
    void                    MakeKeyFriendlyNames (void);
    
public:

                            CInputDX (void);
                            ~CInputDX (void);
    inline void             SetWindowHandle     (HWND hWnd);
    inline void             SetInstanceHandle   (HINSTANCE hInstance);
    bool                    Create              (void);
    void                    Destroy             (void);
    inline void             OpenKeyboard        (void);
    inline bool             IsKeyboardOpened    (void);
    inline void             CloseKeyboard       (void);
    void                    UpdateKeyboard      (void);
    inline bool             GetKey              (int Key);
    inline const char*      GetKeyRealName      (int Key);
    inline const char*      GetKeyFriendlyName  (int Key);
    inline int              GetJoystickCount    (void);
    inline void             OpenJoystick        (int Joystick);
    inline bool             IsJoystickOpened    (int Joystick);
    inline void             CloseJoystick       (int Joystick);
    void                    UpdateJoystick      (int Joystick);
    inline int              GetJoystickAxisX    (int Joystick);
    inline int              GetJoystickAxisY    (int Joystick);
    inline bool             GetJoystickButton   (int Joystick, int Button);
};

typedef CInputDX InputClass;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CInputDX::SetWindowHandle (HWND hWnd)
{
    m_hWnd = hWnd;
}

inline void CInputDX::SetInstanceHandle (HINSTANCE hInstance)
{
    m_hInstance = hInstance;
}

inline void CInputDX::OpenKeyboard (void)
{
    // Try to acquire the keyboard
    HRESULT hRet = m_pKeyboard->Acquire ();
    
    // Assert it's not an unexpected return value
    ASSERT (hRet == DI_OK || hRet == S_FALSE || hRet == DIERR_OTHERAPPHASPRIO);

    // Set the opened state according to the return value
    m_KeyboardOpened = (hRet == DI_OK || hRet == S_FALSE);
}

inline bool CInputDX::IsKeyboardOpened (void)
{
    // Return the opened state of the keyboard
    return m_KeyboardOpened;
}

inline void CInputDX::CloseKeyboard (void)
{
    // Release access to keyboard
    m_pKeyboard->Unacquire ();

    // We are sure the keyboard is not opened
    m_KeyboardOpened = false;
}

inline bool CInputDX::GetKey (int Key)
{
    // Assert the key number is correct
    ASSERT (Key >= 0 && Key < MAX_KEYS);

    // Return the state of the key
    return (m_KeyState[Key] & 0x80) != 0;
}

inline const char* CInputDX::GetKeyRealName (int Key)
{
    // Assert the key number is correct
    ASSERT (Key >= 0 && Key < MAX_KEYS);

    // Return the name of the key
    return m_KeyRealName[Key];
}

inline const char* CInputDX::GetKeyFriendlyName (int Key)
{
    // Assert the key number is correct
    ASSERT (Key >= 0 && Key < MAX_KEYS);

    // Return the name of the key
    return m_KeyFriendlyName[Key];
}

inline int CInputDX::GetJoystickCount (void)
{
    // Return the number of joysticks installed in Windows
    return m_pJoysticks.size ();
}

inline void CInputDX::OpenJoystick (int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < static_cast<int>(m_pJoysticks.size()));

    // Try to acquire the joystick
    HRESULT hRet = m_pJoysticks[Joystick]->pDevice->Acquire ();
    
    // Assert it's not an unexpected return value
    ASSERT (hRet == DI_OK || hRet == S_FALSE || hRet == DIERR_OTHERAPPHASPRIO);

    // Set the opened state according to the return value
    m_pJoysticks[Joystick]->Opened = (hRet == DI_OK || hRet == S_FALSE);
}

inline bool CInputDX::IsJoystickOpened (int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < static_cast<int>(m_pJoysticks.size()));

    // Return the opened state of this joystick
    return m_pJoysticks[Joystick]->Opened;
}

inline void CInputDX::CloseJoystick (int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < static_cast<int>(m_pJoysticks.size()));

    // Release access to this joystick
    m_pJoysticks[Joystick]->pDevice->Unacquire ();

    // We are sure this joystick is not opened
    m_pJoysticks[Joystick]->Opened = false;
}

inline int CInputDX::GetJoystickAxisX (int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < static_cast<int>(m_pJoysticks.size()));

    // Return the value of the X axis of this joystick
    return m_pJoysticks[Joystick]->State.lX;
}

inline int CInputDX::GetJoystickAxisY (int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < static_cast<int>(m_pJoysticks.size()));

    // Return the value of the Y axis of this joystick
    return m_pJoysticks[Joystick]->State.lY;
}

inline bool CInputDX::GetJoystickButton (int Joystick, int Button)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < static_cast<int>(m_pJoysticks.size()));

    // Assert the button number is correct
    ASSERT (Button >= 0 && Button < MAX_JOYSTICK_BUTTONS);

    // Return the state of the specified button on this joystick
    return (m_pJoysticks[Joystick]->State.rgbButtons[Button] & 0x80) != 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CInputDX_H__
