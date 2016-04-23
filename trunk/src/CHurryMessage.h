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
 *  \file CHurryMessage.h
 *  \brief Header file of the hurry message
 */

#ifndef __CHURRYMESSAGE_H__
#define __CHURRYMESSAGE_H__

#include "CScroller.h"

class CDisplay;
class CSound;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Manages the hurry message (match end almost reached) during a match
class CHurryMessage
{
private:

    CDisplay*   m_pDisplay;
    CSound*     m_pSound;
    CScroller   m_Scroller;

public:

                CHurryMessage (CDisplay* pDisplay, CSound* pSound);
                ~CHurryMessage (void);
    bool        Update (float DeltaTime);
    void        Display (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CHURRYMESSAGE_H__
