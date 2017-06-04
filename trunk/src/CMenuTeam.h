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
 *  \file CMenuTeam.h
 *  \brief Header file of the team menu
 */

#ifndef __CMENUTEAM_H__
#define __CMENUTEAM_H__

#include "CMenuBase.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Change time options, start of arena closure and number of winning matches
class CMenuTeam : public CMenuBase
{
private:

    int             m_CursorPlayer;             //!< Cursor used to remember on what player it's pointing to

    void            OnCreate (void);
    void            OnDestroy (void);
    void            OnUp (void);
    void            OnDown (void);
    void            OnLeft (void);
    void            OnRight (void);
    void            OnPrevious (void);
    void            OnNext (void);
    void            OnUpdate (void);
    void            OnDisplay (void);

public:

    CMenuTeam (void);
    virtual ~CMenuTeam (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CMENUTEAM_H__
