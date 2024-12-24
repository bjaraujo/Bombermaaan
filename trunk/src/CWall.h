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
 *  \file CWall.h
 *  \brief Header file of the arena's wall element
 */

#ifndef __CWALL_H__
#define __CWALL_H__

#include "CElement.h"

class CArena;
class CDisplay;
class CArenaSnapshot;

//! Describe a wall type

enum EWallType
{
    WALL_HARD, //!< Hard walls cannot be burnt by flames
    WALL_SOFT, //!< Soft walls can be burnt by flames
    WALL_FALLING //!< Falling walls come from the sky and crush what's on the ground
};

//! An element in the arena which represents a wall.

class CWall : public CElement
{
private:
    int m_iX; //!< Integer position X (in pixels) in the arena
    int m_iY; //!< Integer position Y (in pixels) in the arena
    float m_fX; //!< Float position X (in pixels) in the arena (only used for falling walls because they move)
    float m_fY; //!< Float position Y (in pixels) in the arena (only used for falling walls because they move)
    int m_BlockX; //!< Position X (in blocks) in the arena grid
    int m_BlockY; //!< Position Y (in blocks) in the arena grid
    int m_Sprite; //!< Current sprite to use when displaying
    float m_Timer; //!< Time counter for animation
    bool m_Dead; //!< Is the wall dead? (can it be deleted by the arena?)
    bool m_Burning; //!< Is the wall burning?
    EWallType m_Type; //!< Type of the wall

public:
    CWall(); //!< Constructor (initialize the base class)
    virtual ~CWall(); //!< Destructor (uninitialize the base class)
    void Create(int BlockX, int BlockY, EWallType Type); //!< Initialize the wall
    void Destroy(); //!< Uninitialize the wall
    bool Update(float DeltaTime); //!< Update the wall. Return whether the element should be deleted by the arena.
    void Display(); //!< Display the wall
    void OnWriteSnapshot(CArenaSnapshot& Snapshot);
    void OnReadSnapshot(CArenaSnapshot& Snapshot);
    void Burn(); //!< Make the wall react when it is burnt by an explosion
    void Crush(); //!< Make the wall react when it is crushed by another wall
    inline int GetBlockX(); //!< Return the block position X of the wall
    inline int GetBlockY(); //!< Return the block position Y of the wall
    inline bool IsBurning(); //!< Return whether the wall is burning
    inline int GetType(); //!< Return the type of the wall
};

inline int CWall::GetBlockX() { return m_BlockX; }

inline int CWall::GetBlockY() { return m_BlockY; }

inline bool CWall::IsBurning() { return m_Burning; }

inline int CWall::GetType() { return m_Type; }

#endif // __CWALL_H__
