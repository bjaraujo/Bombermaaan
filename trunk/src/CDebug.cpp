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
 *  \file CDebug.cpp
 *  \brief Utilities for debugging
 */

#include "StdAfx.h"
#include "CDebug.h"
#include "CGame.h"
#include "CTimer.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDebug::CDebug()
{
    m_pTimer = NULL;
    m_pGame = NULL;

    m_pMatch = NULL;
    m_GameSpeed = 0.0f;
    m_CanBombersDie = true;
    m_CanBombersBeSick = true;
    m_CanBombersKick = true;

    for (int i = 0; i < 5; i++)
        m_IsComputerConsoleActive[i] = false;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDebug::~CDebug()
{

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDebug::Create()
{
    ASSERT(m_pTimer != NULL);
    ASSERT(m_pGame != NULL);
    ASSERT(m_pMatch != NULL);

    // Set initial game debug variables
    m_GameSpeed = 1.0f;
    m_CanBombersDie = true;
    m_CanBombersBeSick = true;
    m_CanBombersKick = true;

    for (int i = 0; i < 5; i++)
        m_IsComputerConsoleActive[i] = false;

    // Actually set the speed of the game
    m_pTimer->SetSpeed (m_GameSpeed);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CDebug::Destroy()
{
    // Reset the game speed to normal
    m_pTimer->SetSpeed (1.0f);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CDebug& CDebug::GetInstance()
{
   static CDebug Debug;

   return Debug;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  @brief   Handles a key code for debug purposes
 *
 *  While holding the CTRL key:
 *
 *  CONTROL GAME SPEED
 *  The enter key on the numeric pad sets the game speed to normal
 *  The * on the numeric pad makes the game very fast
 *  The / on the numeric pad makes the game very slow
 *  The + on the numeric pad increases the game speed
 *  The - on the numeric pad decreases the game speed
 *
 *  START OR RESTART A MATCH
 *  Press the F1 key
 *  You may run out of bombs if you do this to often. Example: one bomber is holding a bomb
 *  while restarting the match by pressing Ctrl+F1. This is because m_BombsInUse of CArena is
 *  not reduced.
 *  I guess it can't happen in a 'normal' game because the bomber throws his bomb when he dies or
 *  he is the winner of the match. At least I couldn't recreate this issue in a 'normal' game.
 *
 *  TOGGLE THE BOMBERS' INVULNERABILITY
 *  Press the F2 key
 *
 *  TOGGLE THE DEBUGGING INFORMATION OF EACH COMPUTER PLAYER
 *  Press the number of the player (0-4)
 *
 *  @param   VirtualKeyCode  Key to handle
 *  @param   Modifier        Modifier (SDL only)
 */

void CDebug::HandleKey (DWORD VirtualKeyCode, DWORD Modifier)
{

#ifdef WIN32
    if (GetKeyState(VK_CONTROL) & 0x80)
#else
    if (Modifier & KMOD_CTRL)
#endif
    {
        switch (VirtualKeyCode)
        {

#ifdef DIRECTX
        case VK_MULTIPLY:
#else
        case SDLK_KP_MULTIPLY:
#endif
            {
                m_GameSpeed = 5.0f;

                // Set the new game speed
                m_pTimer->SetSpeed (m_GameSpeed);

                break;
            }

#ifdef DIRECTX
            case VK_DIVIDE:
#else
            case SDLK_KP_DIVIDE:
#endif
            {
                m_GameSpeed = 0.2f;

                // Set the timer speed
                m_pTimer->SetSpeed (m_GameSpeed);

                break;
            }

#ifdef DIRECTX
            case VK_RETURN:
#else
            case SDLK_RETURN:
#endif
            {
                m_GameSpeed = 1.0f;

                // Set the timer speed
                m_pTimer->SetSpeed (m_GameSpeed);

                break;
            }

#ifdef DIRECTX
            case VK_ADD:
#else
            case SDLK_PLUS:
#endif
            {
                m_GameSpeed += 0.2f;

                if (m_GameSpeed > 5.0f)
                {
                    m_GameSpeed = 5.0f;
                }

                // Set the timer speed
                m_pTimer->SetSpeed (m_GameSpeed);

                break;
            }

#ifdef DIRECTX
            case VK_SUBTRACT:
#else
            case SDLK_MINUS:
#endif
            {
                m_GameSpeed -= 0.2f;

                if (m_GameSpeed < 0.0f)
                {
                    m_GameSpeed = 0.0f;
                }

                // Set the timer speed
                m_pTimer->SetSpeed (m_GameSpeed);

                break;
            }

#ifdef DIRECTX
            case VK_F1:
#else
            case SDLK_F1:
#endif
            {
                m_pGame->SwitchToGameMode (GAMEMODE_MATCH);

                break;
            }

#ifdef DIRECTX
            case VK_F2:
#else
            case SDLK_F2:
#endif
            {
                // Make the bombers invulnerable or not
                m_CanBombersDie = !m_CanBombersDie;

                break;
            }

#ifdef _DEBUG_FLAG_1
#ifdef DIRECTX
            case VK_F5:
#else
            case SDLK_F5:
#endif
                theConsole.Write("CDebug::HandleKey(...): Ctrl+F5 was pressed. Writing bombs to log...\n");
                m_pMatch->_Debug_WriteBombsToLog();
                break;
#endif

            case VK_NUMPAD0 : m_IsComputerConsoleActive[0] = !m_IsComputerConsoleActive[0]; break;
            case VK_NUMPAD1 : m_IsComputerConsoleActive[1] = !m_IsComputerConsoleActive[1]; break;
            case VK_NUMPAD2 : m_IsComputerConsoleActive[2] = !m_IsComputerConsoleActive[2]; break;
            case VK_NUMPAD3 : m_IsComputerConsoleActive[3] = !m_IsComputerConsoleActive[3]; break;
            case VK_NUMPAD4 : m_IsComputerConsoleActive[4] = !m_IsComputerConsoleActive[4]; break;
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
