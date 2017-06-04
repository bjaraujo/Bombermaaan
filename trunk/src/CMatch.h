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
 *  \file CMatch.h
 *  \brief Header file of a match
 */

#ifndef __CMATCH_H__
#define __CMATCH_H__

class CDisplay;
class CInput;
class COptions;
class CScores;
class CTimer;
class CSound;
class CPauseMessage;
class CHurryMessage;

#include "CBoard.h"
#include "CArena.h"
#include "CClock.h"
#include "CAiManager.h"
#include "CModeScreen.h"

#include "CSound.h"

#ifdef NETWORK_MODE
    #include "CNetwork.h"
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define NO_WINNER_TEAM      -1      //<! Value for a winner team number if there is no winner

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! The match screen, managing the arena and the board.

class CMatch : public CModeScreen
{
private:

    CBoard          m_Board;                    //!< Board object
    CClock          m_Clock;                    //!< Clock object
    CArena          m_Arena;                    //!< Arena object

    CTeam           m_Teams[MAX_TEAMS];         //!< Teams object 

#ifdef NETWORK_MODE
    CNetwork*       m_pNetwork;                 //!< Network pointer
#endif

    CAiManager      m_AiManager;                //!< Computer brain
    //! @todo Check why m_NoComputer is there (didn't find references)
    bool            m_NoComputer;               //!< True if no computer is playing in this match
    bool            m_MatchOver;                //!< Is match over? (ie. there is a result : winner or draw game)
    int             m_WinnerTeam;               //!< Number of the team that won if there is a winner
    ESong           m_CurrentSong;              //!< Current song being played
    bool            m_IsSongPlaying;            //!< Is the match song playing?
    bool            m_NoticedTimeUp;            //!< Did we notice that time is up and do what is necessary?
    CPauseMessage*  m_pPauseMessage;            //!< Pause message object, instanciated when the match is paused
    CHurryMessage*  m_pHurryMessage;            //!< Hurry up message object, instanciated when the arena starts to close
    float           m_ModeTime;                 //!< Time (in seconds) that elapsed since the mode has started
    float           m_ExitModeTime;             //!< Mode time when we have to start the last black screen
    bool            m_HaveToExit;               //!< Do we have to exit this mode?
    bool            m_computerPlayersPresent;   //!< True, when there are AI players
    bool            m_ForceDrawGame;            //!< Force a draw game when only AI bombers are alive?

    void            CreateMainComponents(void);
    void            DestroyHurryUpMessage(void);
    void            DestroyPauseMessage(void);
    void            DestroyMainComponents(void);
    void            PlaySong(void);
    void            StopSong(void);
    void            ProcessPlayerCommands(void);
    void            UpdateMatch(void);
    void            ManagePauseMessage(void);
    void            ManageHurryUpMessage(void);
    void            ManageMatchOver(void);
    void            DisplayMatchScreen(void);
    void            DisplayHurryUpMessage(void);
    void            DisplayPauseMessage(void);

public:

    CMatch();                           //!< Constructor. Initialize some members.
    virtual         ~CMatch();                          //!< Destructor. Does nothing.
    inline void     SetDisplay(CDisplay *pDisplay);    //!< Set link to the display object to use
    inline void     SetOptions(COptions *pOptions);    //!< Set link to the options object to use
    inline void     SetScores(CScores *pScores);       //!< Set link to the scores object to use
    inline void     SetTimer(CTimer *pTimer);          //!< Set link to the timer object to use
    inline void     SetSound(CSound *pSound);          //!< Set link to the sound object to use
    
#ifdef NETWORK_MODE
    inline void     SetNetwork(CNetwork *pNetwork);     //!< Set link to the network object to use
#endif

    void            Create(void);                      //!< Initialize the object
    void            Destroy(void);                     //!< Uninitialize the object
    void            OpenInput(void);                   //!< Get access to the input this object needs
    void            CloseInput(void);                  //!< Release access to the input this object needs
    EGameMode       Update(void);                      //!< Update the object and return what game mode should be set
    void            Display(void);                     //!< Display on the screen

    inline int      GetWinnerTeam(void);                //!< Get the number of the team that won this match
    inline bool     IsPlayerWinner(int Player);         //!< Get the if player has won this match

#ifdef _DEBUG_FLAG_1
    void            _Debug_WriteBombsToLog();           //!< Only used for debugging!
#endif

};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CMatch::SetDisplay(CDisplay *pDisplay)
{
    CModeScreen::SetDisplay(pDisplay);

    m_Board.SetDisplay(pDisplay);
    m_Arena.SetDisplay(pDisplay);
}

inline void CMatch::SetOptions(COptions *pOptions)
{
    CModeScreen::SetOptions(pOptions);

    m_Board.SetOptions(pOptions);
    m_Arena.SetOptions(pOptions);
}

inline void CMatch::SetScores(CScores *pScores)
{
    m_Board.SetScores(pScores);
}

inline void CMatch::SetTimer(CTimer *pTimer)
{
    CModeScreen::SetTimer(pTimer);

    m_Board.SetTimer(pTimer);
}

inline void CMatch::SetSound(CSound *pSound)
{
    CModeScreen::SetSound(pSound);

    m_Arena.SetSound(pSound);
}

#ifdef NETWORK_MODE
inline void CMatch::SetNetwork(CNetwork *pNetwork)
{

    m_pNetwork = pNetwork;

}
#endif

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CMatch::GetWinnerTeam(void)
{
    return m_WinnerTeam;
}

inline bool CMatch::IsPlayerWinner(int Player)
{

    if (m_Arena.GetBomber(Player).GetTeam()->GetTeamId() == m_WinnerTeam)
        return true;
    else
        return false;

}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CMATCH_H__
