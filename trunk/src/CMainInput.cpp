// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold

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
 *  \file CMainInput.cpp
 *  \brief The main input device for menu and system controls
 */

#include "StdAfx.h"
#include "CMainInput.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MENU_CONTROL_DELAY      0.400f  //!< Time (in seconds) that should elapse before the control starts repeating
#define MENU_CONTROL_RATE       0.180f  //!< Time (in seconds) that should elapse between two repetitions when repeating

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMainInput::CMainInput (void)
{
    m_pTimer = NULL;
    m_pDirectInput = NULL;
    
    m_MenuControls[MENU_UP].Key       = KEYBOARD_UP;
    m_MenuControls[MENU_DOWN].Key     = KEYBOARD_DOWN;
    m_MenuControls[MENU_LEFT].Key     = KEYBOARD_LEFT;
    m_MenuControls[MENU_RIGHT].Key    = KEYBOARD_RIGHT;
    m_MenuControls[MENU_PREVIOUS].Key = KEYBOARD_BACK;
    m_MenuControls[MENU_NEXT1].Key    = KEYBOARD_RETURN;
    m_MenuControls[MENU_NEXT2].Key    = KEYBOARD_NUMPADENTER;
    m_MenuControls[MENU_NEXT3].Key    = KEYBOARD_SPACE;
    
    for (int Control = 0; Control < NUMBER_OF_SYSTEM_CONTROLS; Control++)
    {
        m_SystemControls[Control].State = false;
        m_SystemControls[Control].Pressing = false;
    }    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CMainInput::~CMainInput (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMainInput::Create (void)
{
    ASSERT (m_pTimer != NULL);
    ASSERT (m_pDirectInput != NULL);

    // Set the key index to use for each menu control
    m_MenuControls[MENU_UP].Key       = KEYBOARD_UP;
    m_MenuControls[MENU_DOWN].Key     = KEYBOARD_DOWN;
    m_MenuControls[MENU_LEFT].Key     = KEYBOARD_LEFT;
    m_MenuControls[MENU_RIGHT].Key    = KEYBOARD_RIGHT;
    m_MenuControls[MENU_PREVIOUS].Key = KEYBOARD_BACK;
    m_MenuControls[MENU_NEXT1].Key    = KEYBOARD_RETURN;
    m_MenuControls[MENU_NEXT2].Key    = KEYBOARD_NUMPADENTER;
    m_MenuControls[MENU_NEXT3].Key    = KEYBOARD_SPACE;

    int Control;

    // Set the 'zero' state for each menu control
    for (Control = 0 ; Control < NUMBER_OF_MENU_CONTROLS ; Control++)
    {
        m_MenuControls[Control].PressTime  = 0.0f;
        m_MenuControls[Control].RepeatTime = 0.0f;
        m_MenuControls[Control].Active     = false;
    }

    m_SystemControls[SYSTEM_PAUSE].Key = KEYBOARD_P;
    m_SystemControls[SYSTEM_BREAK].Key = KEYBOARD_ESCAPE;

    for (Control = 0 ; Control < NUMBER_OF_SYSTEM_CONTROLS ; Control++)
    {
        m_SystemControls[Control].State = false;
        m_SystemControls[Control].Pressing = false;
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMainInput::Destroy (void)
{
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMainInput::Open (void)
{
    m_pDirectInput->OpenKeyboard ();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CMainInput::IsOpened (void)
{
    return m_pDirectInput->IsKeyboardOpened ();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMainInput::Close (void)
{
    m_pDirectInput->CloseKeyboard ();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CMainInput::Update (void)
{
    // Update the keyboard state
    m_pDirectInput->UpdateKeyboard ();

    // Contains whether the currently tested menu control is active 
    // on the device (not taking control repetition into account)
    bool ControlActiveOnDevice;
    
    int Control;

    // Test each menu control
    for (Control = 0 ; Control < NUMBER_OF_MENU_CONTROLS ; Control++)
    {   
        // Get the ABSOLUTE menu control state : is the menu control active on the device?
        ControlActiveOnDevice = m_pDirectInput->GetKey (m_MenuControls[Control].Key);
        
        // If the menu control is active on the device
        if (ControlActiveOnDevice)
        {
            // If the menu control was not active last time
            if (m_MenuControls[Control].PressTime == 0.0f)
            {
                // The menu control is active right now (but only for once)
                m_MenuControls[Control].Active = true;
                // Increase the time since the menu control has been active
                m_MenuControls[Control].PressTime += m_pTimer->GetDeltaTime ();
            }
            // If the menu control has been active but the repeat delay has not elapsed yet
            else if (m_MenuControls[Control].PressTime < MENU_CONTROL_DELAY)
            {
                // The menu control is not active until the repeat delay has not elapsed
                m_MenuControls[Control].Active = false;
                // Increase the time since the menu control has been active
                m_MenuControls[Control].PressTime += m_pTimer->GetDeltaTime ();
                
            }
            // If the menu control has been active and the repeat delay has entirely elapsed
            else
            {
                // If the menu control has a repeat rate
                if (MENU_CONTROL_RATE != 0.0f)
                {
                    // If the menu control must start a repetition
                    if (m_MenuControls[Control].RepeatTime == 0.0f)
                    {
                        // The menu control is active right now (but only for once)
                        m_MenuControls[Control].Active = true;
                        // Increase the time since the menu control repetition has started
                        m_MenuControls[Control].RepeatTime += m_pTimer->GetDeltaTime ();
                    }
                    // If the menu control has been repeating but the repeat rate has not elapsed yet
                    else if (m_MenuControls[Control].RepeatTime < MENU_CONTROL_RATE)
                    {
                        // The menu control is not active until a new repetition of this menu control starts
                        m_MenuControls[Control].Active = false;
                        // Increase the time since the menu control current repetition has started
                        m_MenuControls[Control].RepeatTime += m_pTimer->GetDeltaTime ();
                    }
                    // If the menu control has been repeating and the repeat 
                    // rate of the current repetition has entirely elapsed
                    else 
                    {
                        // The menu control is not active until a new repetition of this menu control starts
                        m_MenuControls[Control].Active = false;
                        // Next time, start a new repetition for the current menu control
                        m_MenuControls[Control].RepeatTime = 0.0f;
                    }
                }
                // If the menu control doesn't have a repeat rate
                else
                {
                    // The menu control is ALWAYS active
                    m_MenuControls[Control].Active = true;
                }
            }
        }
        // If the menu control is not active on the device
        else
        {
            // Set the 'zero' state for the current menu control
            m_MenuControls[Control].PressTime = 0.0f;
            m_MenuControls[Control].RepeatTime = 0.0f;
            m_MenuControls[Control].Active = false;
        }
    }

    // State is true only when the control becomes active

    for (Control = 0 ; Control < NUMBER_OF_SYSTEM_CONTROLS ; Control++)
    {
        if (m_pDirectInput->GetKey (m_SystemControls[Control].Key) && 
            !m_SystemControls[Control].State &&
            !m_SystemControls[Control].Pressing)
        {
            m_SystemControls[Control].State = true;
            m_SystemControls[Control].Pressing = true;
        }
        else if (!m_pDirectInput->GetKey (m_SystemControls[Control].Key) && 
                 m_SystemControls[Control].Pressing)
        {
            m_SystemControls[Control].State = false;
            m_SystemControls[Control].Pressing = false;
        }
        else
        {
            m_SystemControls[Control].State = false;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
