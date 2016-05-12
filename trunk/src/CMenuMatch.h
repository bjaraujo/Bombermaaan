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
 *  \file CMenuMatch.h
 *  \brief Header file of the match menu
 */

#ifndef __CMENUMATCH_H__
#define __CMENUMATCH_H__

#include "CMenuBase.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#define MENUMATCH_OPTIONS_COUNT     4

enum EMatchOption
{
    OPTION_MODE,
    OPTION_BATTLE,
    OPTION_TIME,
    OPTION_TIMEUP
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Change time options, start of arena closure and number of winning matches
class CMenuMatch : public CMenuBase
{
private:

    EMatchOption    m_Options [MENUMATCH_OPTIONS_COUNT];    //!< Options of this menu in the right order
    int             m_CursorOption;                         //!< Option the cursor hand is currently pointing to

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

    CMenuMatch (void);
    virtual ~CMenuMatch (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CMENUMATCH_H__
