/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007 Bernd Arnold

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
 *  \file STDAFX.H
 *  \brief Pre-compiled header file
 */

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#ifdef WIN32
#include <winsock2.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>  // GetWindowStyle(), GetWindowExStyle()
#include <io.h>
#else
#include "winreplace.h"
#include <errno.h>
#include <arpa/inet.h>
#include <sys/io.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <string>
//#include <slist>
//#include <set>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>

using namespace std;

#ifndef ASSERT
#define ASSERT assert
#endif

//! Define this if the 32-pixels-per-block version should be used (needs the RES32 directory, where the DLL is built) - default is 16 pixels per block
#define USE_32_PIXELS_PER_BLOCK

//! Define this if the Ctrl+F5 key should output all bombs to the log file for debugging purposes
#define _DEBUG_FLAG_1

//! Use the STL for TinyXML
#define TIXML_USE_STL

#define MEMSET(_p,_value,_size)     memset (_p, _value, _size)
#define MEMZERO(_p,_size)           memset (_p, 0, _size)
#define MEMCOPY(_dst,_src,_size)    memcpy (_dst, _src, _size)

#define ABS(_x)     ((_x) > 0 ? (_x) : -(_x))
#define MIN(_x,_y)  ((_x) < (_y) ? (_x) : (_y))
#define MAX(_x,_y)  ((_x) > (_y) ? (_x) : (_y))
#define SQRT(_x)    sqrt(_x)

#define SEED_RANDOM(seed)     srand(seed)
#define RANDOM(max)       (rand() % max)

#define PLAYER_WHITE                    0       //!< Player number of the white bomber
#define PLAYER_BLACK                    1       //!< Player number of the black bomber
#define PLAYER_RED                      2       //!< Player number of the red bomber
#define PLAYER_BLUE                     3       //!< Player number of the blue bomber
#define PLAYER_GREEN                    4       //!< Player number of the green bomber
#define MAX_PLAYERS                     5       //!< Maximum number of players
#define MAX_PLAYER_SCORE                5       //!< Maximum score for a player
#define MAX_DRAWGAME_SCORE              5       //!< Maximum score for draw games
#define ARENA_WIDTH                     15      //!< Arena width in blocks
#define ARENA_HEIGHT                    13      //!< Arena height in blocks
#ifdef USE_32_PIXELS_PER_BLOCK
#define BLOCK_SIZE                      32      //!< Block size in pixels
#define BLOCK_POSITION_SHIFT            8       //!< Shift number when translating position<->block : i.e. position >> 4
#else
#define BLOCK_SIZE                      16      //!< Block size in pixels
#define BLOCK_POSITION_SHIFT            4       //!< Shift number when translating position<->block : i.e. position >> 4
#endif
#define VIEW_WIDTH                      (ARENA_WIDTH * BLOCK_SIZE)          // Size of the game view from left to right in pixels (size of client area in windowed mode)
#define VIEW_HEIGHT                     (26 + ARENA_HEIGHT * BLOCK_SIZE)    // Size of the game view from top to bottom in pixels (size of client area in windowed mode)

#include "CLog.h"
#include "CConsole.h"
#include "CDebug.h"
#include "CTimer.h"

#if defined(WIN32) && !defined(CROSS)
#  ifdef USE_32_PIXELS_PER_BLOCK
#    include "..\RES32\resource.h"
#  else
#    include "..\RES\resource.h"
#  endif
#else
#  ifdef USE_32_PIXELS_PER_BLOCK
#    include "../RES32/resource.h"
#  else
#    include "../RES/resource.h"
#  endif
#endif
#include "RESOURCE.H"

// Originally taken from CGame.h
//! Describes how the game should currently be updated
enum EGameMode
{
    GAMEMODE_NONE,              //!< No mode! Nothing to update
    GAMEMODE_TITLE,             //!< Title screen (with the main menu)
    GAMEMODE_DEMO,              //!< Demo screen, showing a match between computer players.
    GAMEMODE_CONTROLS,          //!< Controls screen where controls can be customized
    GAMEMODE_MENU,              //!< Menu screen managing all the menu subscreens which allow to setup and start a new match
    GAMEMODE_MATCH,             //!< Match screen : arena and board update, bombers are playing...
    GAMEMODE_WINNER,            //!< Winner screen : display match winner & stuff about scores, update board...
    GAMEMODE_DRAWGAME,          //!< Draw game screen : simple animated screen, update board...
    GAMEMODE_VICTORY,           //!< Victory screen : display battle winner...
    GAMEMODE_GREETS,            //!< Greets screen where the credits are shown
    GAMEMODE_EXIT               //!< In this mode the game will shutdown and exit to windows
};

// Originally taken from CMenu.h
enum EMenuAction
{
    MENUACTION_NONE,        //!< Nothing to do, stay in current menu mode and game mode
    MENUACTION_PREVIOUS,    //!< Go to previous menu mode or to game title screen if there is no previous menu mode
    MENUACTION_NEXT,        //!< Go to next menu mode or to game match screen if there is no next menu mode
};

#endif
