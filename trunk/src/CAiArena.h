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
 *  \file CAiArena.h
 *  \brief Header file of the AI arena manager
 */

#ifndef __CAIARENA_H__
#define __CAIARENA_H__

#include "CArena.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describe the type of danger on a block
enum EDanger
{
    DANGER_MORTAL, //!< This block is currently mortal
    DANGER_SOON,   //!< This block will be mortal very soon
    DANGER_NONE    //!< This block is not dangerous for the moment
};

//! Maximum number of dead ends in the arena
#define MAX_DEAD_END        50

//! Describes the coordinates of a block (used for dead ends' exits)
struct SBlock
{
    int BlockX;
    int BlockY;
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

class CAiArena
{
private:
    
    CArena*         m_pArena;
    CDisplay*       m_pDisplay;
    EDanger         m_Danger [ARENA_WIDTH][ARENA_HEIGHT];           // Danger type of each block of the arena
    float           m_DangerTimeLeft [ARENA_WIDTH][ARENA_HEIGHT];
    int             m_DeadEnd [ARENA_WIDTH][ARENA_HEIGHT];          // If this block is not in a dead end, this contains -1. Otherwise it contains the number of the dead end where this square is.
    SBlock          m_DeadEndExit [MAX_DEAD_END]; //*** inutile *** // Block position of the exit of each dead end (currently unused)
    int             m_SoftWallNear [ARENA_WIDTH][ARENA_HEIGHT];     // Number of soft walls near this square if there is no wall in this square
    bool            m_WallBurn [ARENA_WIDTH][ARENA_HEIGHT];         // True if a soft wall is burning or is going to burn very soon in this square
                                                        
public:                                                 
                                                        
                    CAiArena (void);
    virtual         ~CAiArena (void);
    inline void     SetArena (CArena* pArena);
    inline CArena * GetArena (void);
    inline void     SetDisplay (CDisplay* pDisplay);
    inline EDanger  GetDanger           (int BlockX, int BlockY);
    inline float    GetDangerTimeLeft   (int BlockX, int BlockY);
    inline int      GetDeadEnd          (int BlockX, int BlockY);
    inline SBlock&  GetDeadEndExit      (int Exit);
    inline int      GetSoftWallNear     (int BlockX, int BlockY);
    inline bool     GetWallBurn         (int BlockX, int BlockY);
    void            Create (void);
    void            Destroy (void);
    void            Update (float DeltaTime);
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CAiArena::SetArena (CArena* pArena)
{
    ASSERT (pArena != NULL);
    m_pArena = pArena;
}

inline CArena* CAiArena::GetArena (void)
{
    ASSERT (m_pArena != NULL);
    return m_pArena;
}

inline void CAiArena::SetDisplay (CDisplay *pDisplay)
{
    ASSERT (pDisplay != NULL);
    // Save the display object pointer to pass to elements
    m_pDisplay = pDisplay;
}

inline EDanger CAiArena::GetDanger (int BlockX, int BlockY)
{
    return m_Danger[BlockX][BlockY];
}

inline float CAiArena::GetDangerTimeLeft (int BlockX, int BlockY)
{
    return m_DangerTimeLeft[BlockX][BlockY];
}

inline int CAiArena::GetDeadEnd (int BlockX, int BlockY)
{
    return m_DeadEnd[BlockX][BlockY];
}

inline SBlock& CAiArena::GetDeadEndExit (int Exit)
{
    return m_DeadEndExit[Exit];
}

inline int CAiArena::GetSoftWallNear (int BlockX, int BlockY)
{
    return m_SoftWallNear[BlockX][BlockY];
}

inline bool CAiArena::GetWallBurn (int BlockX, int BlockY)
{
    return m_WallBurn[BlockX][BlockY];
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
