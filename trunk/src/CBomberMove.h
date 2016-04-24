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
 *  \file CBomberMove.h
 *  \brief Header file of bomber moves
 */

#ifndef __CBOMBERMOVE_H__
#define __CBOMBERMOVE_H__

class CArena;
class CBomber;
class CArenaSnapshot;

#include "StdAfx.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes a bomber move command

enum EBomberMove
{
    BOMBERMOVE_NONE,                //!< None
    BOMBERMOVE_UP,                  //!< Up only
    BOMBERMOVE_DOWN,                //!< Down only
    BOMBERMOVE_LEFT,                //!< Left only
    BOMBERMOVE_RIGHT,               //!< Right only
    BOMBERMOVE_UPLEFT,              //!< Up and left together
    BOMBERMOVE_UPRIGHT,             //!< Up and right together
    BOMBERMOVE_DOWNLEFT,            //!< Down and left together
    BOMBERMOVE_DOWNRIGHT            //!< Down and right together
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes the type of turning of the bomber                            

enum ETurning
{
    TURNING_NOTTURNING,                 //!< Is not turning for the moment
    TURNING_UPLEFT_UP,                  //!< Turning up/left, up direction was blocked before turning
    TURNING_UPLEFT_LEFT,                //!< Turning up/left, left direction was blocked before turning
    TURNING_UPRIGHT_UP,                               
    TURNING_UPRIGHT_RIGHT,                         
    TURNING_DOWNLEFT_DOWN,                         
    TURNING_DOWNLEFT_LEFT,                         
    TURNING_DOWNRIGHT_DOWN,                       
    TURNING_DOWNRIGHT_RIGHT     
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
                                        
//! Return type of the CanMove method

enum ECanMove
{
    CANMOVE_CANNOT,             //!< The bomber just cannot go in the desired direction
    CANMOVE_FREEWAY,            //!< The way is totally free
    CANMOVE_AVOID,              //!< The bomber can move but has to avoid an obstacle
    CANMOVE_TURN                //!< The bomber can move but has to turn around an obstacle
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! This class manages the moves of a bomber
class CBomberMove
{
private:                            
    
    CArena*             m_pArena;                   //!< Arena where to check for obstacles
    int                 m_Player;                   //!< Player number of the parent bomber. -1 when not initialized.
    float               m_X;                        //!< Float position X in arena (in pixels) from the top left corner of the arena. 
    float               m_Y;                        //!< Float position Y in arena (in pixels) from the top left corner of the arena. 
    int                 m_iX;                       //!< Integer position X in arena (in pixels) from the top left corner of the arena.
    int                 m_iY;                       //!< Integer position Y in arena (in pixels) from the top left corner of the arena.
    int                 m_BlockX;                   //!< Position X in the arena grid (in blocks).
    int                 m_BlockY;                   //!< Position Y in the arena grid (in blocks).
    EBomberMove         m_BomberMove;               //!< Describes the move that the owner player currently wants the bomber to perform.
    EBomberMove         m_LastRealBomberMove;       //!< Describes the last "real" (ie. not BOMBERMOVE_NONE) move the owner player wanted the bomber to perform.
    ETurning            m_Turning;                  //!< Is the bomber turning around a wall and how
    bool                m_CouldMove;                //!< Could the bomber move the last time he tried? (used for Inertia sickness)
                        
    bool                TryMove (float fPixels);                        //!< Make the bomber move in the current direction (bomber move) and return if he could move.
    ETurning            TurnTest (EBomberMove TestMove);                //!< Return what would be the turning state if we tried moving in the specified direction (bomber move).
    ECanMove            CanMove (EBomberMove TestMove);                 //!< Return the appropriate ECanMove value when trying to go in the current direction (bomber move). This is a virtual test.
    bool                IsObstacle (int X, int Y);                      //!< Return whether there is a wall or a bomb on the specified block.
                                                                        
public:                                                                 
                                                                        
                        CBomberMove (void);                             //!< Constructor
                       ~CBomberMove (void);                             //!< Destructor
    void                Create (int BlockX, int BlockY, int Player);    //!< Initialize the object
    void                Destroy (void);                                 //!< Free any memory allocated by this object
    void                Update (float DeltaTime);                       //!< Make the bomber move according to the bomber move command it received.
    void                WriteSnapshot (CArenaSnapshot& Snapshot);
    void                ReadSnapshot (CArenaSnapshot& Snapshot);
    void                Command (EBomberMove BomberMove);               //!< Give a move order to the bomber for next update.
    inline void         SetArena (CArena* pArena);                      //!< Set the arena to use
    inline int          GetBlockX (void);                               //!< Return the block position X of the bomber
    inline int          GetBlockY (void);                               //!< Return the block position Y of the bomber
    inline int          GetX (void);                                    //!< Get the integer X position (in pixels) of the bomber in the arena
    inline int          GetY (void);                                    //!< Get the integer Y position (in pixels) of the bomber in the arena
    inline bool         CouldMove (void);                               //!< Return whether the bomber could move the last time he tried
    inline EBomberMove  GetMove (void);                                 //!< Return the current move order the bomber has to perform
    inline EBomberMove  GetLastRealMove (void);                         //!< Return the direction where the bomber was going the last time he really moved.
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline void CBomberMove::SetArena (CArena* pArena)
{
    ASSERT(pArena != NULL);
    m_pArena = pArena;
}

inline int CBomberMove::GetBlockX (void) 
{ 
    ASSERT(m_BlockX >= 0);
    ASSERT(m_BlockX < ARENA_WIDTH);

    ASSERT(m_BlockY >= 0);
    ASSERT(m_BlockY < ARENA_HEIGHT);
    
    return m_BlockX; 
}           

inline int CBomberMove::GetBlockY (void) 
{ 
    ASSERT(m_BlockX >= 0);
    ASSERT(m_BlockX < ARENA_WIDTH);

    ASSERT(m_BlockY >= 0);
    ASSERT(m_BlockY < ARENA_HEIGHT);

    return m_BlockY; 
}           

inline int CBomberMove::GetX (void) 
{ 
    return m_iX; 
}

inline int CBomberMove::GetY (void) 
{ 
    return m_iY; 
}

inline bool CBomberMove::CouldMove (void)
{
    return m_CouldMove;
}

inline EBomberMove CBomberMove::GetMove (void)
{
    return m_BomberMove;
}

inline EBomberMove CBomberMove::GetLastRealMove (void)
{
    return m_LastRealBomberMove;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
