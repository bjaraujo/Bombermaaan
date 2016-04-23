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
 *  \file CPauseMessage.h
 *  \brief Header file of the pause message
 */

#ifndef __CPAUSEMESSAGE_H__
#define __CPAUSEMESSAGE_H__

class CDisplay;
class CSound;

#include "CScroller.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Handles the pause message during a match
class CPauseMessage
{
private:

    CScroller   m_Scroller;
    CDisplay*   m_pDisplay;
    CSound*     m_pSound;
    bool        m_Waiting;
    bool        m_HaveToGetOut;

public:

                CPauseMessage (CDisplay* pDisplay, CSound* pSound);
                ~CPauseMessage (void);
    void        Update (float DeltaTime);
    void        Display (void);
    inline bool IsWaiting (void);
    void        GetOut (void);
    inline bool IsOutOfBounds (void);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline bool CPauseMessage::IsWaiting (void)
{
    return m_Waiting;
}

inline bool CPauseMessage::IsOutOfBounds (void)
{
    return m_Scroller.OutOfBounds ();
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif // __CPAUSEMESSAGE_H__
