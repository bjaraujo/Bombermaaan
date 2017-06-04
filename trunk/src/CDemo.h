/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2008 Bernd Arnold

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
 *  \file CDemo.h
 *  \brief Header file of the demo
 */

#ifndef __CDEMO_H__
#define __CDEMO_H__

class CDisplay;
class CInput;
class CScores;
class CTimer;
class CSound;
class CPauseMessage;
class CHurryMessage;

#include "CBoard.h"
#include "CArena.h"
#include "CClock.h"
#include "CAiManager.h"
#include "CFont.h"
#include "CModeScreen.h"
#include "COptions.h"

#include "CSound.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The demo screen, showing a match between computer players.

class CDemo : public CModeScreen
{
private:

    COptions        m_Options;                  //!< Our own customized options based on the real options
    CBoard          m_Board;                    //!< Board object
    CClock          m_Clock;                    //!< Clock object
    CArena          m_Arena;                    //!< Arena object

    CTeam           m_Teams[MAX_TEAMS];         //!< Teams object 

    CAiManager      m_AiManager;                //!< Computer brain
    bool            m_MatchOver;                //!< Is match over? (ie. there is a result : winner or draw game)
    ESong           m_CurrentSong;              //!< Current song being played
    bool            m_IsSongPlaying;            //!< Is the match song playing?
    bool            m_NoticedTimeUp;            //!< Did we notice that time is up and do what is necessary?
    CHurryMessage*  m_pHurryMessage;            //!< Hurry up message object, instanciated when the arena starts to close
    CFont           m_Font;                     //!< Font object needed to draw the DEMO text on the screen
    float           m_DemoTextTime;             //!< Time we have spent drawing (or not) the demo text
    bool            m_DrawDemoText;             //!< Do we currently need to draw the demo text or not draw it?
    float           m_ModeTime;                 //!< Time (in seconds) that elapsed since the mode has started
    float           m_ExitModeTime;             //!< Mode time when we have to start the last black screen
    int             m_ExitGameMode;             //!< Game mode to ask for when exiting
    bool            m_HaveToExit;               //!< Do we have to exit this mode?

    void            CreateMainComponents (void);
    void            CreateComputerPlayers (void);
    void            CreateFont (void);
    void            SetupOptions (void);
    void            DestroyComputerPlayers (void);
    void            DestroyHurryUpMessage (void);
    void            DestroyMainComponents (void);
    void            PlaySong (void);
    void            StopSong (void);
    void            ProcessPlayerCommands (void);
    void            ManageExit (void);
    void            UpdateMatch (void);
    void            UpdateDemoText (void);
    void            ManageHurryUpMessage (void);
    void            ManageMatchOver (void);
    void            DisplayDemoText (void);
    void            DisplayMatchScreen (void);
    void            DisplayHurryUpMessage (void);
    
public:

                    CDemo();                            //!< Constructor. Initialize some members.
    virtual         ~CDemo();                           //!< Destructor. Does nothing.
    inline void     SetDisplay (CDisplay *pDisplay);    //!< Set link to the display object to use
    inline void     SetOptions (COptions *pOptions);    //!< Set link to the options object to use
    inline void     SetScores (CScores *pScores);       //!< Set link to the scores object to use
    inline void     SetTimer (CTimer *pTimer);          //!< Set link to the timer object to use
    inline void     SetSound (CSound *pSound);          //!< Set link to the sound object to use
    void            Create (void);                      //!< Initialize the object
    void            Destroy (void);                     //!< Uninitialize the object
    void            Reset(void);                        //!< Reset the object
    void            OpenInput (void);                   //!< Get access to the input this object needs
    void            CloseInput (void);                  //!< Release access to the input this object needs
    EGameMode       Update (void);                      //!< Update the object and return what game mode should be set
    void            Display (void);                     //!< Display on the screen
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CDemo::SetDisplay (CDisplay *pDisplay)
{
    CModeScreen::SetDisplay (pDisplay);
    m_Board.SetDisplay (pDisplay);
    m_Arena.SetDisplay (pDisplay);
    m_Font.SetDisplay (pDisplay);
}

inline void CDemo::SetOptions (COptions *pOptions)
{
    ASSERT (pOptions != NULL);
    
    CModeScreen::SetOptions (pOptions);
    
    //! Make a copy of the options object. This copy will be customized to suit our needs.
    m_Options = *pOptions;
    
    //! Give the objects of the demo match this copy of the options object.
    m_Board.SetOptions (&m_Options);
    m_Arena.SetOptions (&m_Options);
}

inline void CDemo::SetScores (CScores *pScores)
{
    m_Board.SetScores (pScores);
}

inline void CDemo::SetTimer (CTimer *pTimer)
{
    CModeScreen::SetTimer (pTimer);
    m_Board.SetTimer (pTimer);
}

inline void CDemo::SetSound (CSound *pSound)
{
    CModeScreen::SetSound (pSound);
    m_Arena.SetSound (pSound);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CDEMO_H__
