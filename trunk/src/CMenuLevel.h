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
 *  \file CMenuLevel.h
 *  \brief Header file of the level menu
 */

#ifndef __CMENULEVEL_H__
#define __CMENULEVEL_H__

#include "CMenuBase.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Shows a level with a mini layout picture and lets the user navigate through the different levels
class CMenuLevel : public CMenuBase
{
private:

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

    CMenuLevel (void);
    virtual ~CMenuLevel (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CMENULEVEL_H__
