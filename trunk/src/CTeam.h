/************************************************************************************

    Copyright (C) 2016 Billy Araujo

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
 *  \file CTeam.h
 *  \brief Header file of a team
 */

#ifndef __CTEAM_H__
#define __CTEAM_H__

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CTeam
{
private:
    
    int             m_TeamId;
    bool            m_Victorious;                   //!< Has the team won the current match?
        
public:

                    CTeam();                           //!< Constructor. Initialize some members.
    virtual         ~CTeam();                          //!< Destructor. Does nothing.
    
    void            SetTeamId(int TeamId);
    int             GetTeamId();
    void            SetVictorious(bool Victorious);
    bool            IsVictorious(void);
    
};

#endif  // __CTEAM_H__
