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
 *  \file WinMain.cpp
 *  \brief Entry point of the program for both Linux and Windows
 */

#ifdef CRASH_REPORT
#include "CrashRpt.h"
#include <tchar.h>
#endif

#ifdef _DEBUG_MEMORY
#include <vld.h> 
#endif

#include "StdAfx.h"
#include "CGame.h"

#include "Bombermaaan.h"

/**
 *  \mainpage Bombermaaan source code documentation
 *
 *  This is the source code documentation of Bombermaaan created by Doxygen.
 *
 *  Bombermaaan is a classic Bomberman game, also similar to Dynablaster.
 */


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief This is the main function of the executable file.
 */

#ifdef WIN32
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{

#ifdef CRASH_REPORT
    CR_INSTALL_INFO info;
    memset(&info, 0, sizeof(CR_INSTALL_INFO));
    info.cb = sizeof(CR_INSTALL_INFO);                        // Size of the structure
    info.pszAppName = _T("Bombermaaan");                      // App name
    info.pszAppVersion = _T(APP_VERSION_INFO);                // App version
    info.pszEmailSubject = _T("Bombermaaan Error Report");    // Email subject
    info.pszEmailTo = _T("bjaraujo@github.com");              // Email recipient address
    info.dwFlags = CR_INST_NO_GUI | CR_INST_DONT_SEND_REPORT;

    // Install crash handlers
    int nInstResult = crInstall(&info);
    assert(nInstResult == 0);

    // Check result
    if (nInstResult != 0)
    {
        TCHAR buff[256];
        crGetLastErrorMsg(buff, 256); // Get last error
        _tprintf(_T("%s\n"), buff); // and output it to the screen
        return FALSE;
    }
#endif

#ifndef WIN32
    HINSTANCE hInstance = 0;
    char **lpCmdline = argv;
#endif

    // Create the CGame instance    
    CGame Game(hInstance, lpCmdline);

    // If creating the game failed
#ifdef WIN32
    if (!Game.Create(lpCmdline))
#else
    if (!Game.Create(lpCmdline, argc))
#endif
    {
        // Get out, failure
        return -1;
    }

    //! Show the game window (CGame::Show())
    Game.Show();

    // Update the game (CGame::MesagePump())
    Game.MessagePump();

    // Destroy everything (CGame::Destroy())
    Game.Destroy();

#ifdef CRASH_REPORT
    int nUninstRes = crUninstall(); // Uninstall exception handlers
    assert(nUninstRes == 0);
#endif

    // Everything went right
    return 0;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
