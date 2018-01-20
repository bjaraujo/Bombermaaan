// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CInputDX.cpp
 *  \brief Handling input devices on Windows
 */

#include "StdAfx.h"
#include "CInputDX.h"

static const char* GetDirectInputError(HRESULT hRet);

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define JOYSTICK_DEAD_ZONE      10          //!< Percentage of the dead zone to set for each joystick 
#define JOYSTICK_MINIMUM_AXIS   -1000       //!< Minimum axis value to set for each joystick
#define JOYSTICK_MAXIMUM_AXIS   +1000       //!< Maximum axis value to set for each joystick

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CInputDX::CInputDX(void)
{
    m_pDI = NULL;
    m_hWnd = NULL;
    m_hInstance = NULL;
    m_Ready = false;
    m_pKeyboard = NULL;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CInputDX::~CInputDX(void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

struct SEnumParam
{
    LPDIRECTINPUT8 pDI;
    vector<LPDIRECTINPUTDEVICE8> pDevices;
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

static BOOL CALLBACK CreateInputDevice(LPCDIDEVICEINSTANCE pDeviceInstance, LPVOID pParameter)
{
    // The DirectInput device that will be created
    LPDIRECTINPUTDEVICE8 pDevice;

    // The parameter is a SEnumParam
    SEnumParam *pEnumParam = (SEnumParam *)pParameter;

    // Create the directinput device
    HRESULT hRet = pEnumParam->pDI->CreateDevice(pDeviceInstance->guidInstance, // Device unique identifier
        &pDevice,                    // Give pointer to device
        NULL);
    // If it failed
    if (hRet != DI_OK)
    {
        // Log failure
        theLog.WriteLine("DirectInput     => !!! Could not create input device object.");
        theLog.WriteLine("DirectInput     => !!! DirectInput error is : %s.", GetDirectInputError(hRet));
    }
    // If it was successful
    else
    {
        // Add the directinput device 
        pEnumParam->pDevices.push_back(pDevice);
    }

    // Continue enumerating input devices        
    return DIENUM_CONTINUE;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CInputDX::Create(void)
{
    if (!m_Ready)
    {
        // Create the directinput object
        HRESULT hRet = DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);

        // If it failed
        if (hRet != DI_OK)
        {
            // Log failure
            theLog.WriteLine("DirectInput     => !!! Could not create DirectInput object.");
            theLog.WriteLine("DirectInput     => !!! DirectInput error is : %s.", GetDirectInputError(hRet));

            // Get out
            return false;
        }
        // If it was successful
        else
        {
            // Log success
            theLog.WriteLine("DirectInput     => DirectInput object was created successfully.");
        }

        hRet = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);

        if (hRet != DI_OK)
        {
            // Log failure
            theLog.WriteLine("DirectInput     => !!! Could not create keyboard.");
            theLog.WriteLine("DirectInput     => !!! DirectInput error is : %s.", GetDirectInputError(hRet));

            // Get out
            return false;
        }
        else
        {
            // Log success
            theLog.WriteLine("DirectInput     => Keyboard was created.");
        }

        // Set the keyboard data format
        hRet = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);

        // If it failed
        if (hRet != DI_OK)
        {
            // Log failure
            theLog.WriteLine("DirectInput     => !!! Could not set data format (keyboard).");
            theLog.WriteLine("DirectInput     => !!! DirectInput error is : %s.", GetDirectInputError(hRet));

            // Get out
            return false;
        }

        // Set the keyboard's cooperative level
        hRet = m_pKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

        // If it failed
        if (hRet != DI_OK)
        {
            // Log failure
            theLog.WriteLine("DirectInput     => !!! Could not set cooperative level (keyboard).");
            theLog.WriteLine("DirectInput     => !!! DirectInput error is : %s.", GetDirectInputError(hRet));

            // Get out
            return false;
        }

        // Reset the keyboard state
        ZeroMemory(m_KeyState, MAX_KEYS);

        // The keyboard is not opened yet
        m_KeyboardOpened = false;

        // We need this variable to get information about each key of the keyboard
        DIDEVICEOBJECTINSTANCE DeviceObjectInstance;

        // Scan the keys the keyboard could have
        for (int Key = 0; Key < MAX_KEYS; Key++)
        {
            // Initialize the device object instance and get the key's information
            ZeroMemory(&DeviceObjectInstance, sizeof(DIDEVICEOBJECTINSTANCE));
            DeviceObjectInstance.dwSize = sizeof(DIDEVICEOBJECTINSTANCE);
            HRESULT hRet = m_pKeyboard->GetObjectInfo(&DeviceObjectInstance, Key, DIPH_BYOFFSET);
            ASSERT(hRet == DI_OK || hRet == DIERR_OBJECTNOTFOUND);

            // Store the key's real name, that is to say the name given by Windows
            strcpy(m_KeyRealName[Key], (hRet != DIERR_OBJECTNOTFOUND ? DeviceObjectInstance.tszName : "Key does not exist"));
        }

        // Prepare the friendly name for each key
        MakeKeyFriendlyNames();

        // Prepare the structure to pass when enumerating the devices
        SEnumParam EnumParam;
        EnumParam.pDI = m_pDI;

        // Create all joysticks that are installed in Windows
        hRet = m_pDI->EnumDevices(DI8DEVTYPE_JOYSTICK, CreateInputDevice, &EnumParam, DIEDFL_ALLDEVICES);

        // Assert it's not an unexpected return value
        ASSERT(hRet == DI_OK);

        // The EnumParam contains the input devices, browse them
        for (unsigned int Index = 0; Index < EnumParam.pDevices.size(); Index++)
        {
            LPDIRECTINPUTDEVICE8 pDevice = EnumParam.pDevices[Index];

            // Set the joystick data format
            hRet = pDevice->SetDataFormat(&c_dfDIJoystick);

            // If it failed
            if (hRet != DI_OK)
            {
                // Log failure
                theLog.WriteLine("DirectInput     => !!! Could not set data format (joystick).");
                theLog.WriteLine("DirectInput     => !!! DirectInput error is : %s.", GetDirectInputError(hRet));

                // Get out
                return false;
            }

            // Set the joystick cooperative level
            hRet = pDevice->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

            // If it failed
            if (hRet != DI_OK)
            {
                // Log failure
                theLog.WriteLine("DirectInput     => !!! Could not set cooperative level (joystick).");
                theLog.WriteLine("DirectInput     => !!! DirectInput error is : %s.", GetDirectInputError(hRet));

                // Get out
                return false;
            }

            // We need to prepare this in order to set the joystick dead zone
            DIPROPDWORD PropertyDword;
            PropertyDword.diph.dwSize = sizeof(PropertyDword);
            PropertyDword.diph.dwHeaderSize = sizeof(PropertyDword.diph);
            PropertyDword.diph.dwObj = 0;									// Apply to the
            PropertyDword.diph.dwHow = DIPH_DEVICE;							// entire device
            PropertyDword.dwData = 10000 * JOYSTICK_DEAD_ZONE / 100;

            // Set the property
            hRet = pDevice->SetProperty(DIPROP_DEADZONE, &PropertyDword.diph);

            // Assert it's not an unexpected error
            ASSERT(hRet == DI_OK || hRet == DI_PROPNOEFFECT);

            // We need to prepare this in order to set the joystick range
            DIPROPRANGE PropertyRange;
            PropertyRange.diph.dwSize = sizeof(PropertyRange);
            PropertyRange.diph.dwHeaderSize = sizeof(PropertyRange.diph);
            PropertyRange.diph.dwObj = 0;									// Apply to the
            PropertyRange.diph.dwHow = DIPH_DEVICE;							// entire device
            PropertyRange.lMin = JOYSTICK_MINIMUM_AXIS;
            PropertyRange.lMax = JOYSTICK_MAXIMUM_AXIS;

            // Set the property
            hRet = pDevice->SetProperty(DIPROP_RANGE, &PropertyRange.diph);

            // Assert it's not an unexpected error
            ASSERT(hRet == DI_OK || hRet == DI_PROPNOEFFECT);

            // Allocate memory for a joystick
            SJoystick *pJoystick = new SJoystick;

            // If allocation failed
            if (pJoystick == NULL)
            {
                // Log failure
                theLog.WriteLine("DirectInput     => !!! Could not allocate memory for a joystick.");

                // Get out
                return false;
            }

            // Reset the joystick state
            ZeroMemory(&pJoystick->State, sizeof(pJoystick->State));

            // The joystick is not opened yet
            pJoystick->Opened = false;

            // Set the joystick's directinput device
            pJoystick->pDevice = pDevice;

            // Add the joystick to the container
            m_pJoysticks.push_back(pJoystick);

            // Log we added one joystick
            theLog.WriteLine("DirectInput     => One joystick was created.");
        }

        m_Ready = true;
    }

    // Everything went right
    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CInputDX::Destroy(void)
{
    if (m_Ready)
    {
        // If the DirectInput object is created
        if (m_pDI != NULL)
        {
            // If the keyboard directinput device is created
            if (m_pKeyboard != NULL)
            {
                // Release the device
                m_pKeyboard->Release();
                m_pKeyboard = NULL;
            }

            // Log we released the keyboard
            theLog.WriteLine("DirectInput     => Keyboard was released.");

            // Scan the joysticks that were created
            for (unsigned int Index = 0; Index < m_pJoysticks.size(); Index++)
            {
                // Release the joystick directinput device
                m_pJoysticks[Index]->pDevice->Release();
                m_pJoysticks[Index]->pDevice = NULL;

                // Delete the joystick variable
                delete m_pJoysticks[Index];

                // Log we released a joystick
                theLog.WriteLine("DirectInput     => A joystick was released.");
            }

            // Remove the joystick pointers from the container
            m_pJoysticks.clear();

            // Release the DirectInput object
            m_pDI->Release();
            m_pDI = NULL;

            // Log we released the DirectInput object
            theLog.WriteLine("DirectInput     => DirectInput object was released.");
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CInputDX::UpdateDevice(LPDIRECTINPUTDEVICE8 pDevice, void *pState, int StateSize)
{
    HRESULT hRet;
    bool Opened = false;

    // Try to get the input device state until it works or 
    // until it is impossible to re-obtain access to the device.
    while (true)
    {
        // Poll the device (some joysticks need this)
        pDevice->Poll();

        // Try to get the input device state
        hRet = pDevice->GetDeviceState(StateSize, pState);

        // Assert it's not an unexpected return value
        ASSERT(hRet == DI_OK || hRet == DIERR_INPUTLOST || DIERR_NOTACQUIRED);

        // If it was successful
        if (hRet == DI_OK)
        {
            // Save the current opened state
            Opened = true;

            // It's ok, stop
            break;
        }
        // If the input was lost or if the device was not acquired
        else if (hRet == DIERR_INPUTLOST || hRet == DIERR_NOTACQUIRED)
        {
            // Try to re-obtain access to the device.
            HRESULT hRetAcquire = pDevice->Acquire();

            // Assert it's not an unexpected return value
            ASSERT(hRetAcquire == DI_OK || hRetAcquire == S_FALSE || hRetAcquire == DIERR_OTHERAPPHASPRIO);

            // Save the current opened state (added dummy && operator which reflects the assertion (to remove g++ warning)
            Opened = ((hRet == DI_OK || hRet == S_FALSE) &&
                (hRetAcquire == DI_OK || hRetAcquire == S_FALSE || hRetAcquire == DIERR_OTHERAPPHASPRIO));

            // Get out, we cannot do better than trying to reacquire the device
            break;
        }
    }

    // Return the current opened state
    return Opened;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


void CInputDX::UpdateKeyboard(void)
{
    // Update the device and get the latest real opened state
    m_KeyboardOpened = UpdateDevice(m_pKeyboard, m_KeyState, MAX_KEYS);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CInputDX::UpdateJoystick(int Joystick)
{
    // Check if the joystick number is correct
    ASSERT(Joystick >= 0 && Joystick < static_cast<int>(m_pJoysticks.size()));

    // Check if the joystick is at least supposed to be opened
    ASSERT(m_pJoysticks[Joystick]->Opened);

    // Update the device and get the latest real opened state
    m_pJoysticks[Joystick]->Opened = UpdateDevice(m_pJoysticks[Joystick]->pDevice,
        &m_pJoysticks[Joystick]->State,
        sizeof(m_pJoysticks[Joystick]->State));
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CInputDX::MakeKeyFriendlyNames(void)
{
    for (int Key = 0; Key < MAX_KEYS; Key++)
    {
        switch (Key)
        {
        case KEYBOARD_ESCAPE: strcpy(m_KeyFriendlyName[Key], "ESCAPE"); break;
        case KEYBOARD_1: strcpy(m_KeyFriendlyName[Key], "1"); break;
        case KEYBOARD_2: strcpy(m_KeyFriendlyName[Key], "2"); break;
        case KEYBOARD_3: strcpy(m_KeyFriendlyName[Key], "3"); break;
        case KEYBOARD_4: strcpy(m_KeyFriendlyName[Key], "4"); break;
        case KEYBOARD_5: strcpy(m_KeyFriendlyName[Key], "5"); break;
        case KEYBOARD_6: strcpy(m_KeyFriendlyName[Key], "6"); break;
        case KEYBOARD_7: strcpy(m_KeyFriendlyName[Key], "7"); break;
        case KEYBOARD_8: strcpy(m_KeyFriendlyName[Key], "8"); break;
        case KEYBOARD_9: strcpy(m_KeyFriendlyName[Key], "9"); break;
        case KEYBOARD_0: strcpy(m_KeyFriendlyName[Key], "0"); break;
        case KEYBOARD_MINUS: strcpy(m_KeyFriendlyName[Key], "MINUS"); break;
        case KEYBOARD_EQUALS: strcpy(m_KeyFriendlyName[Key], "EQUALS"); break;
        case KEYBOARD_BACK: strcpy(m_KeyFriendlyName[Key], "BACKSPACE"); break;
        case KEYBOARD_TAB: strcpy(m_KeyFriendlyName[Key], "TAB"); break;
        case KEYBOARD_Q: strcpy(m_KeyFriendlyName[Key], "Q"); break;
        case KEYBOARD_W: strcpy(m_KeyFriendlyName[Key], "W"); break;
        case KEYBOARD_E: strcpy(m_KeyFriendlyName[Key], "E"); break;
        case KEYBOARD_R: strcpy(m_KeyFriendlyName[Key], "R"); break;
        case KEYBOARD_T: strcpy(m_KeyFriendlyName[Key], "T"); break;
        case KEYBOARD_Y: strcpy(m_KeyFriendlyName[Key], "Y"); break;
        case KEYBOARD_U: strcpy(m_KeyFriendlyName[Key], "U"); break;
        case KEYBOARD_I: strcpy(m_KeyFriendlyName[Key], "I"); break;
        case KEYBOARD_O: strcpy(m_KeyFriendlyName[Key], "O"); break;
        case KEYBOARD_P: strcpy(m_KeyFriendlyName[Key], "P"); break;
        case KEYBOARD_LBRACKET: strcpy(m_KeyFriendlyName[Key], "L-BRACKET"); break;
        case KEYBOARD_RBRACKET: strcpy(m_KeyFriendlyName[Key], "R-BRACKET"); break;
        case KEYBOARD_RETURN: strcpy(m_KeyFriendlyName[Key], "RETURN"); break;
        case KEYBOARD_LCONTROL: strcpy(m_KeyFriendlyName[Key], "L.CTRL"); break;
        case KEYBOARD_A: strcpy(m_KeyFriendlyName[Key], "A"); break;
        case KEYBOARD_S: strcpy(m_KeyFriendlyName[Key], "S"); break;
        case KEYBOARD_D: strcpy(m_KeyFriendlyName[Key], "D"); break;
        case KEYBOARD_F: strcpy(m_KeyFriendlyName[Key], "F"); break;
        case KEYBOARD_G: strcpy(m_KeyFriendlyName[Key], "G"); break;
        case KEYBOARD_H: strcpy(m_KeyFriendlyName[Key], "H"); break;
        case KEYBOARD_J: strcpy(m_KeyFriendlyName[Key], "J"); break;
        case KEYBOARD_K: strcpy(m_KeyFriendlyName[Key], "K"); break;
        case KEYBOARD_L: strcpy(m_KeyFriendlyName[Key], "L"); break;
        case KEYBOARD_SEMICOLON: strcpy(m_KeyFriendlyName[Key], "SEMICOLON"); break;
        case KEYBOARD_APOSTROPHE: strcpy(m_KeyFriendlyName[Key], "APOSTR."); break;
        case KEYBOARD_GRAVE: strcpy(m_KeyFriendlyName[Key], "E-GRAVE"); break;
        case KEYBOARD_LSHIFT: strcpy(m_KeyFriendlyName[Key], "L-SHIFT"); break;
        case KEYBOARD_BACKSLASH: strcpy(m_KeyFriendlyName[Key], "BACKSLASH"); break;
        case KEYBOARD_Z: strcpy(m_KeyFriendlyName[Key], "Z"); break;
        case KEYBOARD_X: strcpy(m_KeyFriendlyName[Key], "X"); break;
        case KEYBOARD_C: strcpy(m_KeyFriendlyName[Key], "C"); break;
        case KEYBOARD_V: strcpy(m_KeyFriendlyName[Key], "V"); break;
        case KEYBOARD_B: strcpy(m_KeyFriendlyName[Key], "B"); break;
        case KEYBOARD_N: strcpy(m_KeyFriendlyName[Key], "N"); break;
        case KEYBOARD_M: strcpy(m_KeyFriendlyName[Key], "M"); break;
        case KEYBOARD_COMMA: strcpy(m_KeyFriendlyName[Key], "COMMA"); break;
        case KEYBOARD_PERIOD: strcpy(m_KeyFriendlyName[Key], "PERIOD"); break;
        case KEYBOARD_SLASH: strcpy(m_KeyFriendlyName[Key], "SLASH"); break;
        case KEYBOARD_RSHIFT: strcpy(m_KeyFriendlyName[Key], "R-SHIFT"); break;
        case KEYBOARD_MULTIPLY: strcpy(m_KeyFriendlyName[Key], "NUM-MUL"); break;
        case KEYBOARD_LMENU: strcpy(m_KeyFriendlyName[Key], "L-ALT"); break;
        case KEYBOARD_SPACE: strcpy(m_KeyFriendlyName[Key], "SPACE"); break;
        case KEYBOARD_CAPITAL: strcpy(m_KeyFriendlyName[Key], "CAPITAL"); break;
        case KEYBOARD_F1: strcpy(m_KeyFriendlyName[Key], "F1"); break;
        case KEYBOARD_F2: strcpy(m_KeyFriendlyName[Key], "F2"); break;
        case KEYBOARD_F3: strcpy(m_KeyFriendlyName[Key], "F3"); break;
        case KEYBOARD_F4: strcpy(m_KeyFriendlyName[Key], "F4"); break;
        case KEYBOARD_F5: strcpy(m_KeyFriendlyName[Key], "F5"); break;
        case KEYBOARD_F6: strcpy(m_KeyFriendlyName[Key], "F6"); break;
        case KEYBOARD_F7: strcpy(m_KeyFriendlyName[Key], "F7"); break;
        case KEYBOARD_F8: strcpy(m_KeyFriendlyName[Key], "F8"); break;
        case KEYBOARD_F9: strcpy(m_KeyFriendlyName[Key], "F9"); break;
        case KEYBOARD_F10: strcpy(m_KeyFriendlyName[Key], "F10"); break;
        case KEYBOARD_NUMLOCK: strcpy(m_KeyFriendlyName[Key], "NUMLOCK"); break;
        case KEYBOARD_SCROLL: strcpy(m_KeyFriendlyName[Key], "SCROLL"); break;
        case KEYBOARD_NUMPAD7: strcpy(m_KeyFriendlyName[Key], "NUM-7"); break;
        case KEYBOARD_NUMPAD8: strcpy(m_KeyFriendlyName[Key], "NUM-8"); break;
        case KEYBOARD_NUMPAD9: strcpy(m_KeyFriendlyName[Key], "NUM-9"); break;
        case KEYBOARD_SUBTRACT: strcpy(m_KeyFriendlyName[Key], "NUM-MINUS"); break;
        case KEYBOARD_NUMPAD4: strcpy(m_KeyFriendlyName[Key], "NUM-4"); break;
        case KEYBOARD_NUMPAD5: strcpy(m_KeyFriendlyName[Key], "NUM-5"); break;
        case KEYBOARD_NUMPAD6: strcpy(m_KeyFriendlyName[Key], "NUM-6"); break;
        case KEYBOARD_ADD: strcpy(m_KeyFriendlyName[Key], "NUM-ADD"); break;
        case KEYBOARD_NUMPAD1: strcpy(m_KeyFriendlyName[Key], "NUM-1"); break;
        case KEYBOARD_NUMPAD2: strcpy(m_KeyFriendlyName[Key], "NUM-2"); break;
        case KEYBOARD_NUMPAD3: strcpy(m_KeyFriendlyName[Key], "NUM-3"); break;
        case KEYBOARD_NUMPAD0: strcpy(m_KeyFriendlyName[Key], "NUM-0"); break;
        case KEYBOARD_DECIMAL: strcpy(m_KeyFriendlyName[Key], "NUM-DOT"); break;
        case KEYBOARD_F11: strcpy(m_KeyFriendlyName[Key], "F11"); break;
        case KEYBOARD_F12: strcpy(m_KeyFriendlyName[Key], "F12"); break;
        case KEYBOARD_NUMPADENTER: strcpy(m_KeyFriendlyName[Key], "NUM-ENTER"); break;
        case KEYBOARD_RCONTROL: strcpy(m_KeyFriendlyName[Key], "R-CTRL"); break;
        case KEYBOARD_DIVIDE: strcpy(m_KeyFriendlyName[Key], "NUM-SLASH"); break;
        case KEYBOARD_SYSRQ: strcpy(m_KeyFriendlyName[Key], "SYSTEM"); break;
        case KEYBOARD_RMENU: strcpy(m_KeyFriendlyName[Key], "R-ALT"); break;
        case KEYBOARD_PAUSE: strcpy(m_KeyFriendlyName[Key], "PAUSE"); break;
        case KEYBOARD_HOME: strcpy(m_KeyFriendlyName[Key], "HOME"); break;
        case KEYBOARD_UP: strcpy(m_KeyFriendlyName[Key], "UP"); break;
        case KEYBOARD_PRIOR: strcpy(m_KeyFriendlyName[Key], "PAGEUP"); break;
        case KEYBOARD_LEFT: strcpy(m_KeyFriendlyName[Key], "LEFT"); break;
        case KEYBOARD_RIGHT: strcpy(m_KeyFriendlyName[Key], "RIGHT"); break;
        case KEYBOARD_END: strcpy(m_KeyFriendlyName[Key], "END"); break;
        case KEYBOARD_DOWN: strcpy(m_KeyFriendlyName[Key], "DOWN"); break;
        case KEYBOARD_NEXT: strcpy(m_KeyFriendlyName[Key], "PAGEDOWN"); break;
        case KEYBOARD_INSERT: strcpy(m_KeyFriendlyName[Key], "INSERT"); break;
        case KEYBOARD_DELETE: strcpy(m_KeyFriendlyName[Key], "DELETE"); break;
        case KEYBOARD_LWIN: strcpy(m_KeyFriendlyName[Key], "L-WIN"); break;
        case KEYBOARD_RWIN: strcpy(m_KeyFriendlyName[Key], "R-WIN"); break;
        case KEYBOARD_APPS: strcpy(m_KeyFriendlyName[Key], "APP-MENU"); break;

        default: strcpy(m_KeyFriendlyName[Key], m_KeyRealName[Key]); break;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// WARNING : Only necessary DirectInput return values are here.
// It you get a "Unknown DirectInput error!", then maybe it's
// an error return value that wasn't added to the switch statement.

static const char* GetDirectInputError(HRESULT hRet)
{
    switch (hRet)
    {
    case DI_OK: return "DI_OK";
    case DIERR_ACQUIRED: return "DIERR_ACQUIRED";
    case DIERR_BETADIRECTINPUTVERSION: return "DIERR_BETADIRECTINPUTVERSION";
    case DIERR_DEVICENOTREG: return "DIERR_DEVICENOTREG";
    case DIERR_INVALIDPARAM: return "DIERR_INVALIDPARAM";
    case DIERR_NOINTERFACE: return "DIERR_NOINTERFACE";
    case DIERR_NOTINITIALIZED: return "DIERR_NOTINITIALIZED";
    case DIERR_OLDDIRECTINPUTVERSION: return "DIERR_OLDDIRECTINPUTVERSION";
    case DIERR_OUTOFMEMORY: return "DIERR_OUTOFMEMORY";
    case E_HANDLE: return "E_HANDLE";
    default: return "Unknown DirectInput error!";
    }
}

/*

data format

DIERR_ACQUIRED
DIERR_INVALIDPARAM
DIERR_NOTINITIALIZED

coop level

DIERR_INVALIDPARAM
DIERR_NOTINITIALIZED
E_HANDLE

create device ex

DIERR_DEVICENOTREG
DIERR_INVALIDPARAM
DIERR_NOINTERFACE
DIERR_NOTINITIALIZED
DIERR_OUTOFMEMORY

dinputcreateex

DIERR_BETADIRECTINPUTVERSION
DIERR_INVALIDPARAM
DIERR_OLDDIRECTINPUTVERSION
DIERR_OUTOFMEMORY

*/

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

