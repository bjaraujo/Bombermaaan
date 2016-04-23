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
 *  \file CAiManager.h
 *  \brief Header file of the AI manager
 */

#ifndef __CAIMANAGER_H__
#define __CAIMANAGER_H__

#include "CAiArena.h"

class CArena;
class COptions;
class CAiBomber;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CAiManager
{
private:

    CAiBomber*      m_pBombers [MAX_PLAYERS];
    CAiArena        m_Arena;
    CDisplay*       m_pDisplay;
                                                        
public:                                                 
                                                        
                    CAiManager (void);
    virtual         ~CAiManager (void);
    inline void     SetArena (CArena* pArena);
    inline void     SetDisplay (CDisplay* pDisplay);
    void            Create (COptions* pOptions);
    void            Destroy (void);
    void            Update (float DeltaTime);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CAiManager::SetArena (CArena* pArena)
{
    ASSERT(pArena != NULL);
    m_Arena.SetArena(pArena);
}

inline void CAiManager::SetDisplay (CDisplay *pDisplay)
{
    // Save the display object pointer to pass to elements
    m_pDisplay = pDisplay;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
