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
 *  \file CModeScreen.cpp
 *  \brief The screen basics
 */

#include "CModeScreen.h"
#include "CDisplay.h"
#include "CGame.h"
#include "CInput.h"
#include "COptions.h"
#include "StdAfx.h"

#define BLACKSCREEN_DURATION 0.750f // Duration (in seconds) of each of the two black screens

CModeScreen::CModeScreen()
{
    // Initialize the pointers to nullptr so that we
    // can easily detect the ones we forgot to set.
    m_pDisplay = nullptr;
    m_pInput = nullptr;
    m_pOptions = nullptr;
    m_pTimer = nullptr;
    m_pSound = nullptr;
}

CModeScreen::~CModeScreen() = default;

void CModeScreen::Create()
{
    // Check if all the objects to communicate with are set
    ASSERT(m_pDisplay != nullptr);
    ASSERT(m_pInput != nullptr);
    ASSERT(m_pOptions != nullptr);
    ASSERT(m_pTimer != nullptr);
    ASSERT(m_pSound != nullptr);

    OpenInput();
}

void CModeScreen::Destroy() { CloseInput(); }
