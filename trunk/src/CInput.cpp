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
 *  \file CInput.cpp
 *  \brief Input
 */

#include "StdAfx.h"
#include "CInput.h"
#include "COptions.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CInput::CInput (void)
{
    // Initialize the pointers to NULL so that we
    // can easily detect the ones we forgot to set.
    m_pOptions = NULL;
    m_pTimer = NULL;
    m_PlayerInput = NULL;

    // Set the input object used by the main input
    m_MainInput.SetInput(&m_input);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CInput::~CInput (void)
{
    // Do nothing
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CInput::Create (void)
{
    // Check if all the objects to communicate with are set
    ASSERT (m_pOptions != NULL);
    ASSERT (m_pTimer != NULL);

    // If we could not create the DirectInput interface
    if (!m_input.Create())
    {
        // Get out
        return false;
    }

    // Initialize the main input
    m_MainInput.Create();

    // Allocate a player input object for every available ones
    m_PlayerInput = new CPlayerInput [GetPlayerInputCount()];

    // Initialize every player input object
    for (int PlayerInput = 0 ; PlayerInput < GetPlayerInputCount() ; PlayerInput++)
    {
        m_PlayerInput[PlayerInput].SetDirectInput (&m_input);
        m_PlayerInput[PlayerInput].SetOptions (m_pOptions);
        m_PlayerInput[PlayerInput].Create (PlayerInput);
    }

    return true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CInput::Destroy (void)
{
    // Destroy every player input object
    for (int PlayerInput = 0 ; PlayerInput < GetPlayerInputCount() ; PlayerInput++)
    {
        m_PlayerInput[PlayerInput].Destroy();
    }

    // Deallocate every player input object
    delete [] m_PlayerInput;

    // Uninitialize the main input
    m_MainInput.Destroy();

    // Destroy the DirectInput interface
    m_input.Destroy ();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
