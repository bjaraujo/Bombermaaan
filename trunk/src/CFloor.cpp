// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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
 *  \file CFloor.cpp
 *  \brief Floor in the arena
 */

#include "StdAfx.h"
#include "CFloor.h"         // CFloor
#include "CDisplay.h"       // CDisplay
#include "CArena.h"         // CArena and arena values
#include "CArenaSnapshot.h"

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Floor sprite layers
#define FLOOR_SPRITELAYER       0
#define ACTION_SPRITELAYER      1

// Floor sprites
#define FLOORSPRITE_NOSHADOW    0       // Floor with no shadow
#define FLOORSPRITE_SHADOW      1       // Floor with shadow

// Arrow sprites
#define ARENA_FLOOR_ARROW_RIGHT     0
#define ARENA_FLOOR_ARROW_DOWN      1
#define ARENA_FLOOR_ARROW_LEFT      2
#define ARENA_FLOOR_ARROW_UP        3

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CFloor::CFloor (void) : CElement()
{
    
    m_iX = -1;
    m_iY = -1;
    m_BlockX = -1;
    m_BlockY = -1;
    m_Dead = false;
    m_FloorAction = FLOORACTION_NONE;
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

CFloor::~CFloor (void)
{
    
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFloor::Create (int BlockX, int BlockY, EFloorAction floorAction)
{
    CElement::Create();

    m_iX = m_pArena->ToPosition (BlockX);
    m_iY = m_pArena->ToPosition (BlockY);
    m_BlockX = BlockX;
    m_BlockY = BlockY;
    m_Dead = false;
    m_FloorAction = floorAction;
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFloor::Destroy (void)
{
    CElement::Destroy();
}


//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

bool CFloor::Update (float /*DeltaTime*/)
{
    // The arena can destroy this floor if it is dead
    return m_Dead;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Draws a sprite (shadow or not) in the right
// layer in the arena

void CFloor::Display (void)
{
    // If there is no item and (no wall or a burning wall or a falling wall)
    if (!m_pArena->IsItem(m_BlockX,m_BlockY) &&
        (
         !m_pArena->IsWall(m_BlockX,m_BlockY) || 
         m_pArena->IsBurningWall(m_BlockX,m_BlockY) || 
         m_pArena->IsFallingWall(m_BlockX,m_BlockY)
        ))
    {

        // Try to determine if there is a shadow on this floor.
        // This is true if there is a non falling wall above.
        int Sprite;

        // If there can be a block above and if there is a wall above and it's not a falling wall
        if (m_BlockY - 1 >= 0 && 
            m_pArena->IsWall(m_BlockX,m_BlockY-1) && 
            !m_pArena->IsFallingWall(m_BlockX,m_BlockY-1))
        {
            // Then there is a shadow
            Sprite = FLOORSPRITE_SHADOW;
        }
        // No wall above or falling wall above
        else
        {
            // Then there is a no shadow
            Sprite = FLOORSPRITE_NOSHADOW;
        }

        // Add the sprite in the layer. Priority is not used.
        m_pDisplay->DrawSprite (m_iX, 
                                m_iY, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_ARENA_FLOOR,
                                Sprite, 
                                FLOOR_SPRITELAYER,
                                PRIORITY_UNUSED);

        Sprite = -1;

        switch ( m_FloorAction ) {
            case FLOORACTION_MOVEBOMB_RIGHT:    Sprite = ARENA_FLOOR_ARROW_RIGHT;   break;
            case FLOORACTION_MOVEBOMB_DOWN:     Sprite = ARENA_FLOOR_ARROW_DOWN;    break;
            case FLOORACTION_MOVEBOMB_LEFT:     Sprite = ARENA_FLOOR_ARROW_LEFT;    break;
            case FLOORACTION_MOVEBOMB_UP:       Sprite = ARENA_FLOOR_ARROW_UP;      break;
            default: break;
        }

        if ( Sprite != -1 ) {
                    m_pDisplay->DrawSprite (m_iX, 
                                m_iY, 
                                NULL,                            // Draw entire sprite
                                NULL,                            // No need to clip
                                BMP_ARENA_ARROWS,
                                Sprite,
                                ACTION_SPRITELAYER,
                                PRIORITY_UNUSED);
        }
    }
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

void CFloor::OnWriteSnapshot (CArenaSnapshot& Snapshot)
{
    Snapshot.WriteInteger(m_iX);
    Snapshot.WriteInteger(m_iY);
    Snapshot.WriteInteger(m_BlockX);
    Snapshot.WriteInteger(m_BlockY);
    Snapshot.WriteBoolean(m_Dead);
    Snapshot.WriteInteger(m_FloorAction);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//****************************************************************************************************************************

void CFloor::OnReadSnapshot (CArenaSnapshot& Snapshot)
{
    Snapshot.ReadInteger(&m_iX);
    Snapshot.ReadInteger(&m_iY);
    Snapshot.ReadInteger(&m_BlockX);
    Snapshot.ReadInteger(&m_BlockY);
    Snapshot.ReadBoolean(&m_Dead);
    Snapshot.ReadInteger((int*)&m_FloorAction);
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

// Called by hard walls crushing this floor. A hard wall "crushes" the floor
// where it is because the floor should be dead : it cannot be seen anymore.

void CFloor::Crush (void)
{
    m_Dead = true;
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
