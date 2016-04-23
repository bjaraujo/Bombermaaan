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
 *  \file CGame.h
 *  \brief Header file of the game
 */

#ifndef __CGAME_H__
#define __CGAME_H__

#include "CWindow.h"
#include "CTimer.h"
#include "COptions.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CDrawGame.h"
#include "CWinner.h"
#include "CVictory.h"
#include "CMatch.h"
#include "CScores.h"
#include "CMenu.h"
#include "CSound.h"
#include "CMenuYesNo.h"
#include "CTitle.h"
#include "CControls.h"
#include "CDemo.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// enum EGameMode is now in STDAFX.H

enum ENetworkMode
{
    NETWORKMODE_LOCAL,
    NETWORKMODE_SERVER,
    NETWORKMODE_CLIENT
};

extern ENetworkMode    NetworkMode;
extern SOCKET          MySocket;
extern SOCKET          ClientSocket;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

/**
 *  \brief The main window class of Bombermaaan
 *
 * The CGame class is a window. It initializes itself
 * using an application instance, then initializes
 * the timer, the input, the display, the game screens
 * objects, etc. It's the core of the game and manages 
 * switches between game modes (see EGameMode).
 */

class CGame : public CWindow
{
private:
    
    EGameMode       m_GameMode;             //!< Current game mode defining what to update
    HMODULE         m_hModule;              //!< Connection to the resources
    HINSTANCE       m_hInstance;            //!< Application instance handle
    CTimer          m_Timer;                //!< Timer object for movement, animation, synchronization...
    CDisplay        m_Display;              //!< Needed to draw sprites and manage display
    CInput          m_Input;                //!< Needed to read the players choices in menus, match, etc
    CSound          m_Sound;                //!< Needed to play sounds and musics
    COptions        m_Options;              //!< Options chosen by the players
    CScores         m_Scores;               //!< Scores object where we keep the player scores and the draw games count
    CDrawGame       m_DrawGame;             //!< Draw game screen object (happens when the previous match was a draw game)
    CWinner         m_Winner;               //!< Winner screen object (happens when a player just won one match)
    CVictory        m_Victory;              //!< Victory screen object (happens when a player has the score to reach)
    CMatch          m_Match;                //!< Match screen object in which the players fight
    CMenu           m_Menu;                 //!< Menu screen object for the user to choose the options
    CTitle          m_Title;                //!< Title screen object with the main menu
    CControls       m_Controls;             //!< Controls screen object
    CDemo           m_Demo;                 //!< Demo screen object in which we show a match betweeen computer players.
    CMenuYesNo      m_MenuYesNo;            //!< Yes/No message box object
        
#ifndef WIN32
    string          m_WindowTitle;
#endif

    void            OnActivateApp  (WPARAM wParam, LPARAM lParam);
    void            OnMove         (WPARAM wParam, LPARAM lParam);
    void            OnKeyDown      (WPARAM wParam, LPARAM lParam);
    void            OnKeyUp        (WPARAM wParam, LPARAM lParam);
    void            OnPaint        (WPARAM wParam, LPARAM lParam);
    bool            OnSysCommand   (WPARAM wParam, LPARAM lParam);
    void            OnSize         (WPARAM wParam, LPARAM lParam);
#ifndef WIN32
    void            OnJoystickAxis (WPARAM wParam, LPARAM lParam);             // SDL_JOYAXISMOTION
    void            OnJoystickButton (WPARAM wParam, LPARAM lParam);       // SDL_JOYBUTTONDOWN/-UP
#endif
    void            OnWindowActive (void);
    void            StartGameMode (EGameMode GameMode);
    void            FinishGameMode (void);
    CModeScreen*    GetGameModeObject (EGameMode GameMode);

public:

#ifdef WIN32
                    CGame (HINSTANCE hInstance, const char* pCommandLine);
#else
					CGame (HINSTANCE hInstance, char **pCommandLine);
#endif

    virtual         ~CGame (void);
#ifdef WIN32
	bool            Create (const char* pCommandLine);
#else
	bool            Create (char **pCommandLine, int pCommandLineCount);
#endif

    void            Destroy (void);
    inline void     SwitchToGameMode (EGameMode GameMode);
};


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CGame::SwitchToGameMode (EGameMode GameMode)
{
    FinishGameMode();
    StartGameMode(GameMode);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CGAME_H__
