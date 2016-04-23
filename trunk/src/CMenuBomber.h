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
 *  \file CMenuBomber.h
 *  \brief Header file of the bomber menu
 */

#ifndef __CMENUBOMBER_H__
#define __CMENUBOMBER_H__

#include "CMenuBase.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Handles the menu where bombers can be set to manual/computer/off
class CMenuBomber : public CMenuBase
{
private:

    int             m_CursorPlayer;             //!< Cursor used to remember on what player it's pointing to
    bool            m_Blinking[MAX_PLAYERS];    //!< Is the bomber head (given its index) currently blinking?
    float           m_BlinkTimer[MAX_PLAYERS];  //!< Time left to wait before toggling the corresponding blink state

    void            OnCreate (void);
    void            OnDestroy (void);
    void            OnUpdate (void);
    void            OnDisplay (void);
    void            OnUp (void);
    void            OnDown (void);
    void            OnLeft (void);
    void            OnRight (void);
    void            OnPrevious (void);
    void            OnNext (void);
    
public:

    CMenuBomber (void);
    virtual ~CMenuBomber (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CMENUBOMBER_H__
