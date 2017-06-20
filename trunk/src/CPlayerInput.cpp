// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CPlayerInput.cpp
 *  \brief Player's input device
 */

#include "StdAfx.h"
#include "CPlayerInput.h"
#include "COptions.h"
#include "CInput.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CPlayerInput::CPlayerInput (void)
{
    m_PlayerInput = -1;
    m_pDirectInput = NULL;
    m_pOptions = NULL;
    m_Name[0] = '\0';
    m_ControlName[0] = '\0';
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CPlayerInput::~CPlayerInput (void)
{
    // Nothing to do
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CPlayerInput::Create (int PlayerInput)
{   
    ASSERT (m_pDirectInput != NULL);
    ASSERT (m_pOptions != NULL);
    ASSERT (PlayerInput >= 0);

    m_PlayerInput = PlayerInput;

    CreateName();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CPlayerInput::CreateName (void)
{
    // If the player input is one of the keyboard configurations
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        // Make a name for the specified keyboard configuration
        sprintf (m_Name, "KEYBOARD %d", m_PlayerInput + 1);
    }
    // If the player input is a joystick
    else
    {
        // Make a name for the specified joystick configuration
        sprintf (m_Name, "JOYSTICK %d", m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS + 1);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CPlayerInput::Destroy (void)
{
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CPlayerInput::Open (void)
{   
    // If the player input is one of the keyboard configurations
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        // Get access to the system keyboard
        m_pDirectInput->OpenKeyboard ();
    }
    // If the player input is a joystick
    else
    {
        // Get access to the specified joystick
        m_pDirectInput->OpenJoystick (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CPlayerInput::IsOpened (void)
{
    // If the player input is one of the keyboard configurations
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        // Return whether the system keyboard is opened or not
        return m_pDirectInput->IsKeyboardOpened();
    }
    // If the player input is a joystick
    else
    {
        // Return whether the specified joystick is opened or not
        return m_pDirectInput->IsJoystickOpened (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CPlayerInput::Close (void)
{
    // If the player input is one of the keyboard configurations
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        // Release access to the system keyboard
        m_pDirectInput->CloseKeyboard ();
    }
    // If the player input is a joystick
    else
    {   
        // Release access to the specified joystick
        m_pDirectInput->CloseJoystick (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CPlayerInput::Update (void)
{
    // If the player input is one of the keyboard configurations
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        // Update the state of the system keyboard
        m_pDirectInput->UpdateKeyboard ();
    }
    // If the player input is a joystick
    else
    {   
        // Update the state of the specified joystick
        m_pDirectInput->UpdateJoystick (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS);
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

int CPlayerInput::GetActivatedControl (void)
{
    // If the player input is one of the keyboard configurations
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        for (int Key = 0 ; Key < MAX_KEYS ; Key++)
        {
            if (m_pDirectInput->GetKey (Key))
            {
                return Key;
            }
        }        
    }
    // If the player input is a joystick
    else
    {
        int AxisX = m_pDirectInput->GetJoystickAxisX (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS);
        int AxisY = m_pDirectInput->GetJoystickAxisY (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS);

        if (AxisY < -JOYSTICK_AXIS_THRESHOLD)
        {
            return JOYSTICK_UP;
        }
        else if (AxisY > +JOYSTICK_AXIS_THRESHOLD)
        {
            return JOYSTICK_DOWN;
        }
        else if (AxisX < -JOYSTICK_AXIS_THRESHOLD)
        {
            return JOYSTICK_LEFT;
        }
        else if (AxisX > +JOYSTICK_AXIS_THRESHOLD)
        {
            return JOYSTICK_RIGHT;
        }
        else
        {
            for (int Button = 0 ; Button < MAX_JOYSTICK_BUTTONS ; Button++)
            {
                if (m_pDirectInput->GetJoystickButton (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS, Button))
                {
                    return JOYSTICK_BUTTON(Button);
                }
            }
        }
    }

    return NO_ACTIVATED_CONTROL;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

const char* CPlayerInput::GetControlName (int Control)
{
    // If the player input is one of the keyboard configurations
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        strcpy (m_ControlName, m_pDirectInput->GetKeyFriendlyName(Control));
    }
    // If the player input is a joystick
    else
    {
        if (Control < NUMBER_OF_JOYSTICK_DIRECTIONS)
        {
            switch (Control)
            {
                case JOYSTICK_UP    : strcpy (m_ControlName, "UP");    break;
                case JOYSTICK_DOWN  : strcpy (m_ControlName, "DOWN");  break;
                case JOYSTICK_LEFT  : strcpy (m_ControlName, "LEFT");  break;
                case JOYSTICK_RIGHT : strcpy (m_ControlName, "RIGHT"); break;
            }
        }
        else
        {
            sprintf (m_ControlName, "BUTTON %d", Control - NUMBER_OF_JOYSTICK_DIRECTIONS + 1);
        }
    }

    return m_ControlName;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CPlayerInput::TestControl (int Control)
{
    // If the player input is one of the keyboard configurations
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        // Return the state of the key bound to the required control
        return m_pDirectInput->GetKey (m_pOptions->GetControl(m_PlayerInput, Control));
    }
    // If the player input is a joystick
    else
    {
        int OptControl = m_pOptions->GetControl(m_PlayerInput, Control);
        if (OptControl < NUMBER_OF_JOYSTICK_DIRECTIONS)
        {
            switch (OptControl)
            {   
                // Changed checks from 0 to 100/-100 so the gamepad is not too sensitive
                case JOYSTICK_UP    : 
                    return (m_pDirectInput->GetJoystickAxisY(m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS) < -JOYSTICK_AXIS_THRESHOLD);
                case JOYSTICK_DOWN  : 
                    return (m_pDirectInput->GetJoystickAxisY(m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS) > +JOYSTICK_AXIS_THRESHOLD);
                case JOYSTICK_LEFT  : 
                    return (m_pDirectInput->GetJoystickAxisX(m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS) < -JOYSTICK_AXIS_THRESHOLD);
                case JOYSTICK_RIGHT : 
                    return (m_pDirectInput->GetJoystickAxisX(m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS) > +JOYSTICK_AXIS_THRESHOLD);
            }
        }
        else
        {
            return m_pDirectInput->GetJoystickButton (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS, OptControl - NUMBER_OF_JOYSTICK_DIRECTIONS);
        }
    }

    ASSERT(false);
    return false;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief Check for a joystick button controlling the menu
 *
 *  This function can be used to check for a joystick button (used for menu control).
 *  Implemented only for leaving the winner screen so far (2008-10-21).
 */

bool CPlayerInput::TestMenuControl (int MenuControl)
{
    // If the player input is one of the keyboard configurations, leave!
    // This function is only for joystick control. Keyboard controls are handled by MainInput.
    if (m_PlayerInput < NUMBER_OF_KEYBOARD_CONFIGURATIONS)
    {
        return false;
    }

    // If the player input is a joystick
    return m_pDirectInput->GetJoystickButton (m_PlayerInput - NUMBER_OF_KEYBOARD_CONFIGURATIONS, MenuControl - NUMBER_OF_JOYSTICK_DIRECTIONS);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
