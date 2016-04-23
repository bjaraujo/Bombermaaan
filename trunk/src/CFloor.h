/************************************************************************************

    Copyright (C) 2000-2002, 2007 Thibaut Tollemer
    Copyright (C) 2007, 2008 Bernd Arnold

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
 *  \file CFloor.h
 *  \brief Header file of the floor
 */

#ifndef __CFLOOR_H__
#define __CFLOOR_H__

#include "CElement.h"

class CArena;
class CDisplay;
class CArenaSnapshot;

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! Describes actions on floors (moving bombs so far).
enum EFloorAction
{
    FLOORACTION_NONE,               //!< There is no special action.
    FLOORACTION_MOVEBOMB_RIGHT,     //!< Bombs start moving right
    FLOORACTION_MOVEBOMB_DOWN,      //!< Bombs start moving down
    FLOORACTION_MOVEBOMB_LEFT,      //!< Bombs start moving left
    FLOORACTION_MOVEBOMB_UP         //!< Bombs start moving up
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

//! An element in the arena which represents a bomb.
class CFloor : public CElement
{
private:

    int             m_iX;                               //!< Integer position X (in pixels) in the arena
    int             m_iY;                               //!< Integer position Y (in pixels) in the arena
    int             m_BlockX;                           //!< Position X (in blocks) in the arena grid
    int             m_BlockY;                           //!< Position Y (in blocks) in the arena grid
    bool            m_Dead;                             //!< Should the floor be deleted by the arena?
    EFloorAction    m_FloorAction;                      //!< Action the floor does to objects touching it
                                                        
public:                                                 
                                                        
                    CFloor (void);                      //!< Constructor. Initialize the base class.
    virtual         ~CFloor (void);                     //!< Destructor. Uninitialize the base class.
    void            Create (int BlockX, int BlockY, EFloorAction floorAction);    //!< Initialize the floor.
    void            Destroy (void);                     //!< Uninitialize the floor.
    bool            Update (float DeltaTime);           //!< Update the element. Return whether the element should be deleted by the arena.
    void            Display (void);                     //!< Display the floor.
    void            OnWriteSnapshot (CArenaSnapshot& Snapshot);
    void            OnReadSnapshot (CArenaSnapshot& Snapshot);
    void            Crush (void);                       //!< Make the floor react when the floor is crushed by a wall.
    inline int      GetBlockX (void);                   //!< Return the block position X of the floor
    inline int      GetBlockY (void);                   //!< Return the block position Y of the floor
    inline EFloorAction GetFloorAction( void );         //!< Return the action of the floor
    inline bool     HasAction( void );                  //!< Return if the block has a action
};

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

inline int CFloor::GetBlockX (void) 
{ 
    return m_BlockX; 
}

inline int CFloor::GetBlockY (void) 
{ 
    return m_BlockY;
}

inline EFloorAction CFloor::GetFloorAction(void)
{ 
    return m_FloorAction;
}

inline bool CFloor::HasAction( void )
{
    return GetFloorAction() != FLOORACTION_NONE;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

#endif
