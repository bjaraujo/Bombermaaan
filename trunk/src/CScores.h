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
 *  \file CScores.h
 *  \brief Header file for the scores during a game
 */

#ifndef __CSCORES_H__
#define __CSCORES_H__

class COptions;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CScores
{
private:

    COptions*       m_pOptions;                 //!< Link to the options object to use
    int             m_Scores[MAX_PLAYERS];      //!< Score of each player
    int             m_DrawGamesCount;           //!< Number of draw games

public:

                    CScores();
                    ~CScores();
    inline void     SetOptions (COptions *pOptions);    //!< Set link to the options object to use
    void            Reset (void);                       //!< Reset the scores to zero
    void            RaisePlayerScore (int Player);      //!< Add one to the score of the specified player
    void            RaiseDrawGamesCount (void);         //!< Add one to the draw games count
    inline int      GetPlayerScore (int Player);        //!< Get the score of a player
    inline int      GetDrawGamesCount (void);           //!< Get how many draw games there were
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CScores::SetOptions (COptions *pOptions)
{
    m_pOptions = pOptions;
}

inline int CScores::GetPlayerScore (int Player)
{
    return m_Scores[Player];
}

inline int CScores::GetDrawGamesCount (void)
{
    return m_DrawGamesCount;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif  // __CSCORES_H__
