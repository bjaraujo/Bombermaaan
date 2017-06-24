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
 *  \file CMenu.h
 *  \brief Header file of the menu
 */

#ifndef __CMENU_H__
#define __CMENU_H__

#include "CMenuBomber.h"
#include "CMenuInput.h"
#include "CMenuMatch.h"
#include "CMenuTeam.h"
#include "CMenuLevel.h"
#include "CFont.h"
#include "CModeScreen.h"

class CDisplay;
class CInput;
class COptions;
class CTimer;
class CFont;
class CSound;
class CScores;
class CMosaic;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

enum EMenuMode
{
    MENUMODE_BOMBER,        //!< Which bombers are playing or not?
    MENUMODE_INPUT,         //!< What player input configuration to use for each human player?
    MENUMODE_MATCH,         //!< Match setup : arena times, win matchs...
    MENUMODE_TEAM,          //!< Team setup : set teams...
    MENUMODE_LEVEL          //!< Choose level layout to use
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// EMenuAction is now in STDAFX.H

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CMenu : public CModeScreen
{
private:

    CScores*        m_pScores;              //!< Link to the scores object to use
    CFont           m_Font;                 //!< Font object
    EMenuMode       m_MenuMode;             //!< Current menu mode (= current menu screen)
    CMenuBomber     m_MenuBomber;           //!< Menu screen object corresponding to MENUMODE_BOMBER
    CMenuInput      m_MenuInput;            //!< Menu screen object corresponding to MENUMODE_INPUT
    CMenuMatch      m_MenuMatch;            //!< Menu screen object corresponding to MENUMODE_MATCH
    CMenuTeam       m_MenuTeam;             //!< Menu screen object corresponding to MENUMODE_TEAM
    CMenuLevel      m_MenuLevel;            //!< Menu screen object corresponding to MENUMODE_LEVEL
    float           m_GameModeTime;         //!< Time (in seconds) that elapsed since this game mode has started
    bool            m_HaveToExit;           //!< Do we have to exit this game mode?
    EGameMode       m_ExitGameMode;         //!< Game mode to ask for when exiting (after black screen)
    float           m_ExitGameModeTime;     //!< Game mode time when we realized we have to exit (used for blackscreen)
    bool            m_SongStarted;          //!< Did we start playing the song after the black screen?
    CMosaic*        m_pMosaic;              //!< Mosaic object used for the animated mosaic background of the menu screen
                    
    void            StartMenuMode (EMenuMode MenuMode);
    void            FinishMenuMode (void);
                    
public:             
                    
    CMenu (void);   
    virtual ~CMenu (void);  
                    
    inline void     SetDisplay (CDisplay *pDisplay);    //!< Set link to the display object to use
    inline void     SetInput (CInput *pInput);          //!< Set link to the input object to use
    inline void     SetTimer (CTimer *pTimer);          //!< Set link to the timer object to use
    inline void     SetSound (CSound *pSound);          //!< Set link to the sound object to use
    inline void     SetOptions (COptions *pOptions);    //!< Set link to the options object to use
    inline void     SetScores (CScores *pScores);       //!< Set link to the scores object to use
    void            Create (void);                      //!< Initialize the object
    void            Destroy (void);                     //!< Uninitialize the object
    void            OpenInput (void);                   //!< Get access to the input this object needs
    void            CloseInput (void);                  //!< Release access to the input this object needs
    EGameMode       Update (void);                      //!< Update the object and return what game mode should be set
    void            Display (void);                     //!< Display on the screen
    void            SetMenuMode (EMenuMode MenuMode);   //!< Set the current menu mode
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CMenu::SetDisplay (CDisplay *pDisplay)
{
    CModeScreen::SetDisplay (pDisplay);
    m_Font.SetDisplay (pDisplay);
    m_MenuBomber.SetDisplay (pDisplay);
    m_MenuInput.SetDisplay (pDisplay);
    m_MenuMatch.SetDisplay (pDisplay);
    m_MenuTeam.SetDisplay(pDisplay);
    m_MenuLevel.SetDisplay(pDisplay);
}

inline void CMenu::SetInput (CInput *pInput)
{
    CModeScreen::SetInput (pInput);
    m_MenuBomber.SetInput (pInput);
    m_MenuInput.SetInput (pInput);
    m_MenuMatch.SetInput (pInput);
    m_MenuTeam.SetInput(pInput);
    m_MenuLevel.SetInput(pInput);
}

inline void CMenu::SetOptions (COptions *pOptions)
{
    CModeScreen::SetOptions (pOptions);
    m_MenuBomber.SetOptions (pOptions);
    m_MenuInput.SetOptions (pOptions);
    m_MenuMatch.SetOptions (pOptions);
    m_MenuTeam.SetOptions(pOptions);
    m_MenuLevel.SetOptions (pOptions);
}

inline void CMenu::SetTimer (CTimer *pTimer)
{
    CModeScreen::SetTimer (pTimer);
    m_MenuBomber.SetTimer (pTimer);
    m_MenuInput.SetTimer (pTimer);
    m_MenuMatch.SetTimer (pTimer);
    m_MenuTeam.SetTimer(pTimer);
    m_MenuLevel.SetTimer(pTimer);
}

inline void CMenu::SetSound (CSound *pSound)
{
    CModeScreen::SetSound (pSound);
    m_MenuBomber.SetSound (pSound);
    m_MenuInput.SetSound (pSound);
    m_MenuMatch.SetSound (pSound);
    m_MenuTeam.SetSound(pSound);
    m_MenuLevel.SetSound(pSound);
}

inline void CMenu::SetScores (CScores *pScores)
{
    m_pScores = pScores;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CMENU_H__
